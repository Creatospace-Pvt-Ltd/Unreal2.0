// Copyright (c) 2022 Nineva Studios

#include "Auth/FGAuthLibrary.h"

#include "Async/Async.h"
#include "FirebaseGoodiesLog.h"
#include "Interface/IFirebaseAuthLibrary.h"

#if PLATFORM_IOS
#include "IOS/AuthLibraryIOS.h"
#endif

#if PLATFORM_ANDROID
#include "Android/AuthLibraryAndroid.h"
#endif

#if (PLATFORM_WINDOWS || PLATFORM_MAC) && FG_ENABLE_EDITOR_SUPPORT
#include "Desktop/AuthLibraryDesktop.h"
#endif

#if PLATFORM_ANDROID
TSharedPtr<IFirebaseAuthLibrary> UFGAuthLibrary::AuthLibraryImpl = MakeShareable(new AuthLibraryAndroid());
#endif

#if PLATFORM_IOS
TSharedPtr<IFirebaseAuthLibrary> UFGAuthLibrary::AuthLibraryImpl = MakeShareable(new AuthLibraryIOS());
#endif

#if (PLATFORM_WINDOWS || PLATFORM_MAC) && !FG_ENABLE_EDITOR_SUPPORT
TSharedPtr<IFirebaseAuthLibrary> UFGAuthLibrary::AuthLibraryImpl = nullptr;
#endif

#if (PLATFORM_WINDOWS || PLATFORM_MAC) && FG_ENABLE_EDITOR_SUPPORT
TSharedPtr<IFirebaseAuthLibrary> UFGAuthLibrary::AuthLibraryImpl = MakeShareable(new AuthLibraryDesktop());
#endif


FAuthVoidDelegate UFGAuthLibrary::IdTokenChangedCallback;
FAuthVoidDelegate UFGAuthLibrary::AuthStateChangedCallback;
FAuthVoidDelegate UFGAuthLibrary::PasswordResetSuccessCallback;
FAuthUserDelegate UFGAuthLibrary::UserCreateSuccessCallback;
FAuthUserDelegate UFGAuthLibrary::SignInSuccessCallback;
FAuthStringDelegate UFGAuthLibrary::AuthErrorCallback;
FSignInMethodsFetchedDelegate UFGAuthLibrary::SignInMethodsFetchedCallback;
FAuthCredentialsDelegate UFGAuthLibrary::PhoneVerificationSuccessCallback;
FAuthStringDelegate UFGAuthLibrary::PhoneVerificationTimeoutCallback;
FAuthStringDelegate UFGAuthLibrary::PhoneVerificationSentCallback;
FAuthCredentialsDelegate UFGAuthLibrary::SignInWithGoogleSuccessCallback;

// Game Center
FAuthCredentialsDelegate UFGAuthLibrary::GetGameCenterCredentialsSuccessDelegate;
FAuthStringDelegate UFGAuthLibrary::GetGameCenterCredentialsErrorDelegate;

void UFGAuthLibrary::InitListeners(const FAuthVoidDelegate& IdTokenChangedDelegate, const FAuthVoidDelegate& AuthStateChangedDelegate)
{
	IdTokenChangedCallback = IdTokenChangedDelegate;
	AuthStateChangedCallback = AuthStateChangedDelegate;

	if (!AuthLibraryImpl)
	{
		return;
	}
	AuthLibraryImpl->InitListeners(IdTokenChangedDelegate, AuthStateChangedDelegate);
}

UFGFirebaseUser* UFGAuthLibrary::CurrentUser()
{
	const TSharedPtr<IFirebaseUser> FirebaseUser = AuthLibraryImpl->CurrentUser();

	UFGFirebaseUser* User = NewObject<UFGFirebaseUser>();
	if (!AuthLibraryImpl)
	{
		return User;
	}
	User->Init(FirebaseUser);
	return User;
}

void UFGAuthLibrary::CreateUser(const FString& Email, const FString& Password, const FAuthUserDelegate& OnSuccess,
                                const FAuthStringDelegate& OnError)
{
	UserCreateSuccessCallback = OnSuccess;
	AuthErrorCallback = OnError;

	if (!AuthLibraryImpl)
	{
		return;
	}

	AuthLibraryImpl->CreateUser(Email, Password, OnSuccess, OnError);
}

void UFGAuthLibrary::FetchProvidersForEmail(const FString& Email, const FSignInMethodsFetchedDelegate& OnSuccess,
                                            const FAuthStringDelegate& OnError)
{
	SignInMethodsFetchedCallback = OnSuccess;
	AuthErrorCallback = OnError;

	if (!AuthLibraryImpl)
	{
		return;
	}

	AuthLibraryImpl->FetchProvidersForEmail(Email, OnSuccess, OnError);
}

void UFGAuthLibrary::SendPasswordReset(const FString& Email, const FAuthVoidDelegate& OnSuccess, const FAuthStringDelegate& OnError)
{
	PasswordResetSuccessCallback = OnSuccess;
	AuthErrorCallback = OnError;

	if (!AuthLibraryImpl)
	{
		return;
	}

	AuthLibraryImpl->SendPasswordReset(Email, OnSuccess, OnError);
}

void UFGAuthLibrary::SignInWithEmailAndPassword(const FString& Email, const FString& Password, const FAuthUserDelegate& OnSuccess,
                                                const FAuthStringDelegate& OnError)
{
	SignInSuccessCallback = OnSuccess;
	AuthErrorCallback = OnError;

	if (!AuthLibraryImpl)
	{
		return;
	}

	AuthLibraryImpl->SignInWithEmailAndPassword(Email, Password, OnSuccess, OnError);
}

void UFGAuthLibrary::SignInWithToken(const FString& Token, const FAuthUserDelegate& OnSuccess, const FAuthStringDelegate& OnError)
{
	SignInSuccessCallback = OnSuccess;
	AuthErrorCallback = OnError;

	if (!AuthLibraryImpl)
	{
		return;
	}

	AuthLibraryImpl->SignInWithToken(Token, OnSuccess, OnError);
}

void UFGAuthLibrary::SignInAnonymously(const FAuthUserDelegate& OnSuccess, const FAuthStringDelegate& OnError)
{
	SignInSuccessCallback = OnSuccess;
	AuthErrorCallback = OnError;
	if (!AuthLibraryImpl)
	{
		UE_LOG(LogTemp, Warning, TEXT("Anon login done!!"));
		return;
	}

	AuthLibraryImpl->SignInAnonymously(OnSuccess, OnError);
}

void UFGAuthLibrary::SignInWithCredentials(UFGAuthCredentials* Credentials, const FAuthUserDelegate& OnSuccess,
                                           const FAuthStringDelegate& OnError)
{
	SignInSuccessCallback = OnSuccess;
	AuthErrorCallback = OnError;

	if (!Credentials->AreValid())
	{
		return;
	}

	if (!AuthLibraryImpl)
	{
		return;
	}

	AuthLibraryImpl->SignInWithCredentials(Credentials->GetCredentials(), OnSuccess, OnError);
}

void UFGAuthLibrary::SignOut()
{
	if (!AuthLibraryImpl)
	{
		return;
	}

	AuthLibraryImpl->SignOut();
}

UFGAuthCredentials* UFGAuthLibrary::GetEmailCredentials(const FString& Email, const FString& Password)
{
	if (Email.IsEmpty() || Password.IsEmpty())
	{
		UE_LOG(LogFirebaseGoodies, Error, TEXT("Empty string! \n Check email & password!"));
		return nullptr;
	}

	UFGAuthCredentials* Credentials = NewObject<UFGAuthCredentials>();
	if (!AuthLibraryImpl)
		return Credentials;

	Credentials->Init(AuthLibraryImpl->GetEmailCredentials(Email, Password));
	return Credentials;
}

UFGAuthCredentials* UFGAuthLibrary::GetFacebookCredentials(const FString& Token)
{
	if (Token.IsEmpty())
	{
		UE_LOG(LogFirebaseGoodies, Error, TEXT("Empty string! \n Check token!"));
		return nullptr;
	}

	UFGAuthCredentials* Credentials = NewObject<UFGAuthCredentials>();
	if (!AuthLibraryImpl)
		return Credentials;

	Credentials->Init(AuthLibraryImpl->GetFacebookCredentials(Token));
	return Credentials;
}

UFGAuthCredentials* UFGAuthLibrary::GetGithubCredentials(const FString& Token)
{
	if (Token.IsEmpty())
	{
		UE_LOG(LogFirebaseGoodies, Error, TEXT("Empty string! \n Check token!"));
		return nullptr;
	}

	UFGAuthCredentials* Credentials = NewObject<UFGAuthCredentials>();
	if (!AuthLibraryImpl)
		return Credentials;

	Credentials->Init(AuthLibraryImpl->GetGithubCredentials(Token));
	return Credentials;
}

UFGAuthCredentials* UFGAuthLibrary::GetGoogleCredentials(const FString& IdToken, const FString& AccessToken)
{
	if (IdToken.IsEmpty() || AccessToken.IsEmpty())
	{
		UE_LOG(LogFirebaseGoodies, Error, TEXT("Empty string! \n Check tokens!"));
		return nullptr;
	}

	UFGAuthCredentials* Credentials = NewObject<UFGAuthCredentials>();
	if (!AuthLibraryImpl)
		return Credentials;

	Credentials->Init(AuthLibraryImpl->GetGoogleCredentials(IdToken, AccessToken));
	return Credentials;
}

UFGAuthCredentials* UFGAuthLibrary::GetAppleCredentials(const FString& IdToken, const FString& RawNonce)
{
	if (IdToken.IsEmpty() || RawNonce.IsEmpty())
	{
		UE_LOG(LogFirebaseGoodies, Error, TEXT("Empty string! \n Check token or nonce!"));
		return nullptr;
	}

	UFGAuthCredentials* Credentials = NewObject<UFGAuthCredentials>();
	if (!AuthLibraryImpl)
		return Credentials;

	Credentials->Init(AuthLibraryImpl->GetAppleCredentials(IdToken, RawNonce));
	return Credentials;
}

void UFGAuthLibrary::GetGameCenterCredentials(const FAuthCredentialsDelegate& OnSuccess, const FAuthStringDelegate& OnError)
{
	GetGameCenterCredentialsSuccessDelegate = OnSuccess;
	GetGameCenterCredentialsErrorDelegate = OnError;

	if (!AuthLibraryImpl)
	{
		return;
	}

	AuthLibraryImpl->GetGameCenterCredentials(OnSuccess, OnError);
}

UFGAuthCredentials* UFGAuthLibrary::GetGenericOAuthCredentials(const FString& Provider, const FString& IdToken, const FString& AccessToken)
{
	if (Provider.IsEmpty() || IdToken.IsEmpty() || AccessToken.IsEmpty())
	{
		UE_LOG(LogFirebaseGoodies, Error, TEXT("Empty string! \n Check provider and tokens!"));
		return nullptr;
	}

	UFGAuthCredentials* Credentials = NewObject<UFGAuthCredentials>();
	if (!AuthLibraryImpl)
		return Credentials;

	Credentials->Init(AuthLibraryImpl->GetGenericOAuthCredentials(Provider, IdToken, AccessToken));
	return Credentials;
}

UFGAuthCredentials* UFGAuthLibrary::GetPlayGamesCredentials(const FString& ServerAuthCode)
{
	if (ServerAuthCode.IsEmpty())
	{
		UE_LOG(LogFirebaseGoodies, Error, TEXT("Empty string! \n Check server authorization code!"));
		return nullptr;
	}

	UFGAuthCredentials* Credentials = NewObject<UFGAuthCredentials>();
	if (!AuthLibraryImpl)
		return Credentials;

	Credentials->Init(AuthLibraryImpl->GetPlayGamesCredentials(ServerAuthCode));
	return Credentials;
}

UFGAuthCredentials* UFGAuthLibrary::GetTwitterCredentials(const FString& Token, const FString& Secret)
{
	if (Token.IsEmpty() || Secret.IsEmpty())
	{
		UE_LOG(LogFirebaseGoodies, Error, TEXT("Empty string! \n Check token and secret!"));
		return nullptr;
	}

	UFGAuthCredentials* Credentials = NewObject<UFGAuthCredentials>();
	if (!AuthLibraryImpl)
		return Credentials;

	Credentials->Init(AuthLibraryImpl->GetTwitterCredentials(Token, Secret));
	return Credentials;
}

UFGAuthCredentials* UFGAuthLibrary::GetPhoneCredentials(const FString& VerificationId, const FString& VerificationCode)
{
	UFGAuthCredentials* Credentials = NewObject<UFGAuthCredentials>();

	if (VerificationId.IsEmpty() || VerificationCode.IsEmpty())
	{
		UE_LOG(LogFirebaseGoodies, Error, TEXT("Empty string! \n Check verification id and code!"));
		return Credentials;
	}
	if (!AuthLibraryImpl)
		return Credentials;

	Credentials->Init(AuthLibraryImpl->GetTwitterCredentials(VerificationId, VerificationCode));
	return Credentials;
}

void UFGAuthLibrary::VerifyPhoneNumber(const FString& Number, int TimeoutMillis, const FAuthCredentialsDelegate& OnSuccess,
                                       const FAuthStringDelegate& OnError, const FAuthStringDelegate& OnTimeout,
                                       const FAuthStringDelegate& OnSmsSent)
{
	if (Number.IsEmpty())
	{
		UE_LOG(LogFirebaseGoodies, Error, TEXT("Empty string! \n Check number!"));
		return;
	}

	PhoneVerificationSuccessCallback = OnSuccess;
	AuthErrorCallback = OnError;
	PhoneVerificationTimeoutCallback = OnTimeout;
	PhoneVerificationSentCallback = OnSmsSent;

	if (!AuthLibraryImpl)
	{
		return;
	}

	AuthLibraryImpl->VerifyPhoneNumber(Number, TimeoutMillis, OnSuccess, OnError, OnTimeout, OnSmsSent);
}

void UFGAuthLibrary::PromptGoogleSignIn(const FAuthCredentialsDelegate& OnSuccess,
                                        const FAuthStringDelegate& OnError)
{
	SignInWithGoogleSuccessCallback = OnSuccess;
	AuthErrorCallback = OnError;

	if (!AuthLibraryImpl)
	{
		return;
	}

	AuthLibraryImpl->PromptGoogleSignIn(OnSuccess, OnError);
}
