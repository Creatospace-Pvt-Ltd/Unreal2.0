#include "AuthLibraryIOS.h"

#import "IOS/IOSAppDelegate.h"

#import <FirebaseAuth/FirebaseAuth.h>
#import <GoogleSignIn/GoogleSignIn.h>
#include <objc/objc.h>

#include "Async/Async.h"
#include "FirebaseGoodies.h"
#include "FirebaseGoodiesLog.h"
#include "FirebaseGoodiesSettings.h"
#include "IOSFirebaseAuthCredentials.h"
#include "IOSFirebaseUser.h"

AuthLibraryIOS::~AuthLibraryIOS() {}

static void HandleSignInCallback(FIRAuthDataResult* result, NSError* error) {
  if (error != nil) {
	FString ErrorMessage = FString(error.localizedDescription);
	UE_LOG(LogFirebaseGoodies, Error, TEXT("Auth error: %s"), *ErrorMessage);
	AsyncTask(ENamedThreads::GameThread, [=]() { UFGAuthLibrary::AuthErrorCallback.ExecuteIfBound(ErrorMessage); });
	return;
  }

  IOSFirebaseUser* IosUser = new IOSFirebaseUser(result.user);

  AsyncTask(ENamedThreads::GameThread, [=]()
  {
		UFGFirebaseUser* User = NewObject<UFGFirebaseUser>();
		User->Init(MakeShareable(IosUser));
		UFGAuthLibrary::SignInSuccessCallback.ExecuteIfBound(User);
  });
}

void AuthLibraryIOS::InitListeners(const FAuthVoidDelegate& IdTokenChangedDelegate, const FAuthVoidDelegate& AuthStateChangedDelegate) {
  [[FIRAuth auth] addAuthStateDidChangeListener:^(FIRAuth* _Nonnull auth, FIRUser* _Nullable user) {
	AsyncTask(ENamedThreads::GameThread, [=]() { UFGAuthLibrary::AuthStateChangedCallback.ExecuteIfBound(); });
  }];

  [[FIRAuth auth] addIDTokenDidChangeListener:^(FIRAuth* _Nonnull auth, FIRUser* _Nullable user) {
	AsyncTask(ENamedThreads::GameThread, [=]() { UFGAuthLibrary::IdTokenChangedCallback.ExecuteIfBound(); });
  }];
}

TSharedPtr<IFirebaseUser> AuthLibraryIOS::CurrentUser() { return MakeShareable(new IOSFirebaseUser([[FIRAuth auth] currentUser])); }

void AuthLibraryIOS::CreateUser(const FString& Email, const FString& Password, const FAuthUserDelegate& OnSuccess, const FAuthStringDelegate& OnError) {
  [[FIRAuth auth] createUserWithEmail:Email.GetNSString()
							 password:Password.GetNSString()
						   completion:^(FIRAuthDataResult* _Nullable result, NSError* _Nullable error) {
							 if (error != nil) {
							   FString ErrorMessage = FString(error.localizedDescription);
							   UE_LOG(LogFirebaseGoodies, Error, TEXT("Create user error: %s"), *ErrorMessage);
							   AsyncTask(ENamedThreads::GameThread, [=]() { UFGAuthLibrary::AuthErrorCallback.ExecuteIfBound(ErrorMessage); });
							   return;
							 }

						   	auto IosUser = MakeShareable(new IOSFirebaseUser(result.user));

							 AsyncTask(ENamedThreads::GameThread, [=]()
							 {
								 UFGFirebaseUser* User = NewObject<UFGFirebaseUser>();
							 	 User->Init(IosUser);
								 UFGAuthLibrary::UserCreateSuccessCallback.ExecuteIfBound(User);
							 });
						   }];
}

void AuthLibraryIOS::FetchProvidersForEmail(const FString& Email, const FSignInMethodsFetchedDelegate& OnSuccess, const FAuthStringDelegate& OnError) {
  [[FIRAuth auth] fetchSignInMethodsForEmail:Email.GetNSString()
								  completion:^(NSArray<NSString*>* _Nullable providers, NSError* _Nullable error) {
									if (error != nil) {
									  FString ErrorMessage = FString(error.localizedDescription);
									  UE_LOG(LogFirebaseGoodies, Error, TEXT("Failed to fetch providers: %s"), *ErrorMessage);
									  AsyncTask(ENamedThreads::GameThread, [=]() { UFGAuthLibrary::AuthErrorCallback.ExecuteIfBound(ErrorMessage); });
									  return;
									}

									TArray<FString> FetchedMethods;
									for (NSString* Method in providers) {
									  FetchedMethods.Add(FString(Method));
									}

									AsyncTask(ENamedThreads::GameThread, [=]() { UFGAuthLibrary::SignInMethodsFetchedCallback.ExecuteIfBound(FetchedMethods); });
								  }];
}

void AuthLibraryIOS::SendPasswordReset(const FString& Email, const FAuthVoidDelegate& OnSuccess, const FAuthStringDelegate& OnError) {
  [[FIRAuth auth] sendPasswordResetWithEmail:Email.GetNSString()
								  completion:^(NSError* _Nullable error) {
									if (error != nil) {
									  FString ErrorMessage = FString(error.localizedDescription);
									  UE_LOG(LogFirebaseGoodies, Error, TEXT("Password reset: %s"), *ErrorMessage);
									  AsyncTask(ENamedThreads::GameThread, [=]() { UFGAuthLibrary::AuthErrorCallback.ExecuteIfBound(ErrorMessage); });
									  return;
									}

									AsyncTask(ENamedThreads::GameThread, [=]() { UFGAuthLibrary::PasswordResetSuccessCallback.ExecuteIfBound(); });
								  }];
}

void AuthLibraryIOS::SignInWithEmailAndPassword(const FString& Email, const FString& Password, const FAuthUserDelegate& OnSuccess, const FAuthStringDelegate& OnError) {
  [[FIRAuth auth] signInWithEmail:Email.GetNSString()
						 password:Password.GetNSString()
					   completion:^(FIRAuthDataResult* _Nullable result, NSError* _Nullable error) {
						 HandleSignInCallback(result, error);
					   }];
}

void AuthLibraryIOS::SignInWithToken(const FString& Token, const FAuthUserDelegate& OnSuccess, const FAuthStringDelegate& OnError) {
  [[FIRAuth auth] signInWithCustomToken:Token.GetNSString()
							 completion:^(FIRAuthDataResult* _Nullable result, NSError* _Nullable error) {
							   HandleSignInCallback(result, error);
							 }];
}

void AuthLibraryIOS::SignInAnonymously(const FAuthUserDelegate& OnSuccess, const FAuthStringDelegate& OnError) {
  [[FIRAuth auth] signInAnonymouslyWithCompletion:^(FIRAuthDataResult* _Nullable result, NSError* _Nullable error) {
	HandleSignInCallback(result, error);
  }];
}

void AuthLibraryIOS::SignInWithCredentials(TSharedPtr<IFirebaseAuthCredentials> Credentials, const FAuthUserDelegate& OnSuccess, const FAuthStringDelegate& OnError) {
  const auto IosCredentials = StaticCastSharedPtr<IOSFirebaseAuthCredentials>(Credentials)->GetNativeCredentials();
  [[FIRAuth auth] signInWithCredential:IosCredentials
							completion:^(FIRAuthDataResult* _Nullable result, NSError* _Nullable error) {
							  HandleSignInCallback(result, error);
							}];
}

void AuthLibraryIOS::SignOut() { [[FIRAuth auth] signOut:nil]; }

TSharedPtr<IFirebaseAuthCredentials> AuthLibraryIOS::GetEmailCredentials(const FString& Email, const FString& Password) {
  FIRAuthCredential* IOSCredential = [FIREmailAuthProvider credentialWithEmail:Email.GetNSString() password:Password.GetNSString()];
  return MakeShareable(new IOSFirebaseAuthCredentials(IOSCredential));
}

TSharedPtr<IFirebaseAuthCredentials> AuthLibraryIOS::GetFacebookCredentials(const FString& Token) {
  FIRAuthCredential* IOSCredential = [FIRFacebookAuthProvider credentialWithAccessToken:Token.GetNSString()];
  return MakeShareable(new IOSFirebaseAuthCredentials(IOSCredential));
}

TSharedPtr<IFirebaseAuthCredentials> AuthLibraryIOS::GetGithubCredentials(const FString& Token) {
  FIRAuthCredential* IOSCredential = [FIRGitHubAuthProvider credentialWithToken:Token.GetNSString()];
  return MakeShareable(new IOSFirebaseAuthCredentials(IOSCredential));
}

TSharedPtr<IFirebaseAuthCredentials> AuthLibraryIOS::GetGoogleCredentials(const FString& IdToken, const FString& AccessToken) {
  FIRAuthCredential* IOSCredential = [FIRGoogleAuthProvider credentialWithIDToken:IdToken.GetNSString() accessToken:AccessToken.GetNSString()];
  return MakeShareable(new IOSFirebaseAuthCredentials(IOSCredential));
}

TSharedPtr<IFirebaseAuthCredentials> AuthLibraryIOS::GetAppleCredentials(const FString& IdToken, const FString& RawNonce) {
  FIRAuthCredential* IOSCredential = [FIROAuthProvider credentialWithProviderID:@"apple.com" IDToken:IdToken.GetNSString() rawNonce:RawNonce.GetNSString()];
  return MakeShareable(new IOSFirebaseAuthCredentials(IOSCredential));
}

void AuthLibraryIOS::GetGameCenterCredentials(const FAuthCredentialsDelegate& OnSuccess, const FAuthStringDelegate& OnError) {
  [FIRGameCenterAuthProvider getCredentialWithCompletion:^(FIRAuthCredential* credential, NSError* error) {
	if (error == nil) {
	  auto Credential = MakeShareable(new IOSFirebaseAuthCredentials(credential));

	  AsyncTask(ENamedThreads::GameThread, [=]()
	  {
			UFGAuthCredentials* Credentials = NewObject<UFGAuthCredentials>();
			Credentials->Init(Credential);
			UFGAuthLibrary::GetGameCenterCredentialsSuccessDelegate.ExecuteIfBound(Credentials);
	  });
	} else {
	  FString ErrorMessage = FString(error.localizedDescription);
	  UE_LOG(LogFirebaseGoodies, Error, TEXT("Get game center credentials error: %s"), *ErrorMessage);
	  AsyncTask(ENamedThreads::GameThread, [=]() { UFGAuthLibrary::GetGameCenterCredentialsErrorDelegate.ExecuteIfBound(ErrorMessage); });
	}
  }];
}

TSharedPtr<IFirebaseAuthCredentials> AuthLibraryIOS::GetGenericOAuthCredentials(const FString& Provider, const FString& IdToken, const FString& AccessToken) {
  FIRAuthCredential* IOSCredential = [FIROAuthProvider credentialWithProviderID:Provider.GetNSString() IDToken:IdToken.GetNSString() accessToken:AccessToken.GetNSString()];
  return MakeShareable(new IOSFirebaseAuthCredentials(IOSCredential));
}

TSharedPtr<IFirebaseAuthCredentials> AuthLibraryIOS::GetPlayGamesCredentials(const FString& ServerAuthCode)
{
	// Not available on iOS
	return nullptr;
}

TSharedPtr<IFirebaseAuthCredentials> AuthLibraryIOS::GetTwitterCredentials(const FString& Token, const FString& Secret) {
  FIRAuthCredential* IOSCredential = [FIRTwitterAuthProvider credentialWithToken:Token.GetNSString() secret:Secret.GetNSString()];
  return MakeShareable(new IOSFirebaseAuthCredentials(IOSCredential));
}

TSharedPtr<IFirebaseAuthCredentials> AuthLibraryIOS::GetPhoneCredentials(const FString& VerificationId, const FString& VerificationCode) {
  FIRAuthCredential* IOSCredential = [[FIRPhoneAuthProvider provider] credentialWithVerificationID:VerificationId.GetNSString() verificationCode:VerificationCode.GetNSString()];
  return MakeShareable(new IOSFirebaseAuthCredentials(IOSCredential));
}

void AuthLibraryIOS::VerifyPhoneNumber(const FString& Number, int TimeoutMillis, const FAuthCredentialsDelegate& OnSuccess, const FAuthStringDelegate& OnError, const FAuthStringDelegate& OnTimeout,
									   const FAuthStringDelegate& OnSmsSent) {
  [[FIRPhoneAuthProvider provider] verifyPhoneNumber:Number.GetNSString()
										  UIDelegate:nil
										  completion:^(NSString* _Nullable verificationID, NSError* _Nullable error) {
											if (error != nil) {
											  FString ErrorMessage = FString(error.localizedDescription);
											  UE_LOG(LogFirebaseGoodies, Error, TEXT("Verify phone number error: %s"), *ErrorMessage);
											  AsyncTask(ENamedThreads::GameThread, [=]() { UFGAuthLibrary::AuthErrorCallback.ExecuteIfBound(ErrorMessage); });
											  return;
											}

											FString verificationIDString = FString(verificationID);
											AsyncTask(ENamedThreads::GameThread, [=]() { UFGAuthLibrary::PhoneVerificationSentCallback.ExecuteIfBound(verificationIDString); });
										  }];
}

void AuthLibraryIOS::PromptGoogleSignIn(const FAuthCredentialsDelegate& OnSuccess, const FAuthStringDelegate& OnError) {
  const UFirebaseGoodiesSettings* Settings = FFirebaseGoodiesModule::Get().GetSettings();
  GIDConfiguration* config = [[GIDConfiguration alloc] initWithClientID:Settings->ClientId.GetNSString()];
  [GIDSignIn.sharedInstance signInWithConfiguration:config
						   presentingViewController:[IOSAppDelegate GetDelegate].IOSController
										   callback:^(GIDGoogleUser* _Nullable user, NSError* _Nullable error) {
											 if (error == nil) {
											   GIDAuthentication* authentication = user.authentication;
											   FIRAuthCredential* credential = [FIRGoogleAuthProvider credentialWithIDToken:authentication.idToken accessToken:authentication.accessToken];
											 	auto IosCredentials = MakeShareable(new IOSFirebaseAuthCredentials(credential));
											   AsyncTask(ENamedThreads::GameThread, [=]()
											   {
													UFGAuthCredentials* credentials = NewObject<UFGAuthCredentials>();
													credentials->Init(IosCredentials);
													UFGAuthLibrary::SignInWithGoogleSuccessCallback.ExecuteIfBound(credentials);
											   });
											 } else {
											   FString ErrorMessage = FString(error.localizedDescription);
											   UE_LOG(LogFirebaseGoodies, Error, TEXT("Google sign in error: %s"), *ErrorMessage);
											   AsyncTask(ENamedThreads::GameThread, [=]() { UFGAuthLibrary::AuthErrorCallback.ExecuteIfBound(ErrorMessage); });
											 }
										   }];
}
