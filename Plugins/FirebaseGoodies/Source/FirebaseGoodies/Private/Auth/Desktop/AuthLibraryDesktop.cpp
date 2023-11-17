#if (PLATFORM_WINDOWS || PLATFORM_MAC) && FG_ENABLE_EDITOR_SUPPORT

#include "AuthLibraryDesktop.h"

#include "DesktopFirebaseAuthCredentials.h"
#include "FirebaseGoodiesLog.h"
#include "DesktopFirebaseUser.h"
#include "Listener/FGAuthStateListener.h"
#include "Listener/FGIdTokenListener.h"
#include "Listener/FGPhoneListener.h"

static void HandleSignInCallback(const firebase::Future<firebase::auth::User*>& Callback)
{
	if (Callback.status() == firebase::FutureStatus::kFutureStatusComplete && Callback.error() == 0)
	{
		const auto IosUser = MakeShareable(new DesktopFirebaseUser(*Callback.result()));

		AsyncTask(ENamedThreads::GameThread, [=]()
		{
			UFGFirebaseUser* User = NewObject<UFGFirebaseUser>();
			User->Init(IosUser);
			UFGAuthLibrary::SignInSuccessCallback.ExecuteIfBound(User);
		});
	}
	else
	{
		const FString ErrorMessage = FString(Callback.error_message());
		UE_LOG(LogFirebaseGoodies, Error, TEXT("Auth error: %s"), *ErrorMessage);

		AsyncTask(ENamedThreads::GameThread, [=]()
		{
			UFGAuthLibrary::AuthErrorCallback.ExecuteIfBound(ErrorMessage);
		});
	}
}

void AuthLibraryDesktop::InitListeners(const FAuthVoidDelegate& IdTokenChangedDelegate, const FAuthVoidDelegate& AuthStateChangedDelegate)
{
	firebase::auth::Auth* Auth = firebase::auth::Auth::GetAuth(firebase::App::GetInstance());

	Auth->AddAuthStateListener(new FGAuthStateListener());
	Auth->AddIdTokenListener(new FGIdTokenListener());
}

TSharedPtr<IFirebaseUser> AuthLibraryDesktop::CurrentUser()
{
	firebase::auth::Auth* Auth = firebase::auth::Auth::GetAuth(firebase::App::GetInstance());
	return MakeShareable(new DesktopFirebaseUser(Auth->current_user()));
}

void AuthLibraryDesktop::CreateUser(const FString& Email, const FString& Password, const FAuthUserDelegate& OnSuccess, const FAuthStringDelegate& OnError)
{
	firebase::auth::Auth* Auth = firebase::auth::Auth::GetAuth(firebase::App::GetInstance());

	const auto Task = Auth->CreateUserWithEmailAndPassword(TCHAR_TO_ANSI(*Email), TCHAR_TO_ANSI(*Password));
	Task.AddOnCompletion([=](const ::firebase::Future<firebase::auth::User*>& Callback)
	{
		if (Callback.status() == firebase::FutureStatus::kFutureStatusComplete && Callback.error() == 0)
		{
			const auto IosUser = MakeShareable(new DesktopFirebaseUser(*Callback.result()));

			AsyncTask(ENamedThreads::GameThread, [=]()
			{
				UFGFirebaseUser* User = NewObject<UFGFirebaseUser>();
				User->Init(IosUser);
				UFGAuthLibrary::UserCreateSuccessCallback.ExecuteIfBound(User);
			});
		}
		else
		{
			const FString ErrorMessage = FString(Callback.error_message());
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Create user error: %s"), *ErrorMessage);

			AsyncTask(ENamedThreads::GameThread, [=]()
			{
				UFGAuthLibrary::AuthErrorCallback.ExecuteIfBound(ErrorMessage);
			});
		}
	});
}

void AuthLibraryDesktop::FetchProvidersForEmail(const FString& Email, const FSignInMethodsFetchedDelegate& OnSuccess, const FAuthStringDelegate& OnError)
{
	firebase::auth::Auth* Auth = firebase::auth::Auth::GetAuth(firebase::App::GetInstance());

	const auto Task = Auth->FetchProvidersForEmail(TCHAR_TO_ANSI(*Email));
	Task.AddOnCompletion([=](const ::firebase::Future<firebase::auth::Auth::FetchProvidersResult>& Callback)
	{
		if (Callback.status() == firebase::FutureStatus::kFutureStatusComplete && Callback.error() == 0)
		{
			TArray<FString> FetchedMethods;

			for (auto Provider : (*Callback.result()).providers)
			{
				FetchedMethods.Add(Provider.c_str());
			}

			AsyncTask(ENamedThreads::GameThread, [=]()
			{
				UFGAuthLibrary::SignInMethodsFetchedCallback.ExecuteIfBound(FetchedMethods);
			});
		}
		else
		{
			const FString ErrorMessage = FString(Callback.error_message());
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Failed to fetch providers: %s"), *ErrorMessage);

			AsyncTask(ENamedThreads::GameThread, [=]()
			{
				UFGAuthLibrary::AuthErrorCallback.ExecuteIfBound(ErrorMessage);
			});
		}
	});
}

void AuthLibraryDesktop::SendPasswordReset(const FString& Email, const FAuthVoidDelegate& OnSuccess, const FAuthStringDelegate& OnError)
{
	firebase::auth::Auth* Auth = firebase::auth::Auth::GetAuth(firebase::App::GetInstance());

	const auto Task = Auth->SendPasswordResetEmail(TCHAR_TO_ANSI(*Email));
	Task.AddOnCompletion([=](const ::firebase::Future<void>& Callback)
	{
		if (Callback.status() == firebase::FutureStatus::kFutureStatusComplete && Callback.error() == 0)
		{
			AsyncTask(ENamedThreads::GameThread, [=]()
			{
				UFGAuthLibrary::PasswordResetSuccessCallback.ExecuteIfBound();
			});
		}
		else
		{
			const FString ErrorMessage = FString(Callback.error_message());
			UE_LOG(LogFirebaseGoodies, Error, TEXT("Password reset error: %s"), *ErrorMessage);

			AsyncTask(ENamedThreads::GameThread, [=]()
			{
				UFGAuthLibrary::AuthErrorCallback.ExecuteIfBound(ErrorMessage);
			});
		}
	});
}

void AuthLibraryDesktop::SignInWithEmailAndPassword(const FString& Email, const FString& Password, const FAuthUserDelegate& OnSuccess, const FAuthStringDelegate& OnError)
{
	firebase::auth::Auth* Auth = firebase::auth::Auth::GetAuth(firebase::App::GetInstance());
	Auth->SignInWithEmailAndPassword(TCHAR_TO_ANSI(*Email), TCHAR_TO_ANSI(*Password)).AddOnCompletion(HandleSignInCallback);
}

void AuthLibraryDesktop::SignInWithToken(const FString& Token, const FAuthUserDelegate& OnSuccess, const FAuthStringDelegate& OnError)
{
	firebase::auth::Auth* Auth = firebase::auth::Auth::GetAuth(firebase::App::GetInstance());
	Auth->SignInWithCustomToken(TCHAR_TO_ANSI(*Token)).AddOnCompletion(HandleSignInCallback);
}

void AuthLibraryDesktop::SignInAnonymously(const FAuthUserDelegate& OnSuccess, const FAuthStringDelegate& OnError)
{
	firebase::auth::Auth* Auth = firebase::auth::Auth::GetAuth(firebase::App::GetInstance());
	Auth->SignInAnonymously().AddOnCompletion(HandleSignInCallback);
}

void AuthLibraryDesktop::SignInWithCredentials(TSharedPtr<IFirebaseAuthCredentials> Credentials, const FAuthUserDelegate& OnSuccess, const FAuthStringDelegate& OnError)
{
	firebase::auth::Auth* Auth = firebase::auth::Auth::GetAuth(firebase::App::GetInstance());
	const auto DesktopCredentials = StaticCastSharedPtr<DesktopFirebaseAuthCredentials>(Credentials);
	Auth->SignInWithCredential(DesktopCredentials->GetDesktopCredential()).AddOnCompletion(HandleSignInCallback);
}

void AuthLibraryDesktop::SignOut()
{
	firebase::auth::Auth::GetAuth(firebase::App::GetInstance())->SignOut();
}

TSharedPtr<IFirebaseAuthCredentials> AuthLibraryDesktop::GetEmailCredentials(const FString& Email, const FString& Password)
{
	const firebase::auth::Credential Credential = firebase::auth::EmailAuthProvider::GetCredential(TCHAR_TO_ANSI(*Email), TCHAR_TO_ANSI(*Password));
	return MakeShareable(new DesktopFirebaseAuthCredentials(Credential));
}

TSharedPtr<IFirebaseAuthCredentials> AuthLibraryDesktop::GetFacebookCredentials(const FString& Token)
{
	const firebase::auth::Credential Credential = firebase::auth::FacebookAuthProvider::GetCredential(TCHAR_TO_ANSI(*Token));
	return MakeShareable(new DesktopFirebaseAuthCredentials(Credential));
}

TSharedPtr<IFirebaseAuthCredentials> AuthLibraryDesktop::GetGithubCredentials(const FString& Token)
{
	const firebase::auth::Credential Credential = firebase::auth::GitHubAuthProvider::GetCredential(TCHAR_TO_ANSI(*Token));
	return MakeShareable(new DesktopFirebaseAuthCredentials(Credential));
}

TSharedPtr<IFirebaseAuthCredentials> AuthLibraryDesktop::GetGoogleCredentials(const FString& IdToken, const FString& AccessToken)
{
	const firebase::auth::Credential Credential = firebase::auth::GoogleAuthProvider::GetCredential(TCHAR_TO_ANSI(*IdToken), TCHAR_TO_ANSI(*AccessToken));
	return MakeShareable(new DesktopFirebaseAuthCredentials(Credential));
}

TSharedPtr<IFirebaseAuthCredentials> AuthLibraryDesktop::GetAppleCredentials(const FString& IdToken, const FString& RawNonce)
{
	// TODO check if not available on desktop
	return nullptr;
}

void AuthLibraryDesktop::GetGameCenterCredentials(const FAuthCredentialsDelegate& OnSuccess, const FAuthStringDelegate& OnError)
{
	// TODO check if not available on desktop
}

TSharedPtr<IFirebaseAuthCredentials> AuthLibraryDesktop::GetGenericOAuthCredentials(const FString& Provider, const FString& IdToken, const FString& AccessToken)
{
	const firebase::auth::Credential Credential = firebase::auth::OAuthProvider::GetCredential(TCHAR_TO_ANSI(*Provider), TCHAR_TO_ANSI(*IdToken), TCHAR_TO_ANSI(*AccessToken));
	return MakeShareable(new DesktopFirebaseAuthCredentials(Credential));
}

TSharedPtr<IFirebaseAuthCredentials> AuthLibraryDesktop::GetPlayGamesCredentials(const FString& ServerAuthCode)
{
	const firebase::auth::Credential Credential = firebase::auth::PlayGamesAuthProvider::GetCredential(TCHAR_TO_ANSI(*ServerAuthCode));
	return MakeShareable(new DesktopFirebaseAuthCredentials(Credential));
}

TSharedPtr<IFirebaseAuthCredentials> AuthLibraryDesktop::GetTwitterCredentials(const FString& Token, const FString& Secret)
{
	const firebase::auth::Credential Credential = firebase::auth::TwitterAuthProvider::GetCredential(TCHAR_TO_ANSI(*Token), TCHAR_TO_ANSI(*Secret));
	return MakeShareable(new DesktopFirebaseAuthCredentials(Credential));
}

TSharedPtr<IFirebaseAuthCredentials> AuthLibraryDesktop::GetPhoneCredentials(const FString& VerificationId, const FString& VerificationCode)
{
	firebase::auth::Auth* Auth = firebase::auth::Auth::GetAuth(firebase::App::GetInstance());
	const firebase::auth::Credential Credential = firebase::auth::PhoneAuthProvider::GetInstance(Auth).GetCredential(TCHAR_TO_ANSI(*VerificationId), TCHAR_TO_ANSI(*VerificationCode));
	return MakeShareable(new DesktopFirebaseAuthCredentials(Credential));
}

void AuthLibraryDesktop::VerifyPhoneNumber(const FString& Number, int TimeoutMillis, const FAuthCredentialsDelegate& OnSuccess, const FAuthStringDelegate& OnError,
                                           const FAuthStringDelegate& OnTimeout, const FAuthStringDelegate& OnSmsSent)
{
	firebase::auth::Auth* Auth = firebase::auth::Auth::GetAuth(firebase::App::GetInstance());
	firebase::auth::PhoneAuthProvider::GetInstance(Auth).VerifyPhoneNumber(TCHAR_TO_ANSI(*Number), TimeoutMillis, new firebase::auth::PhoneAuthProvider::ForceResendingToken(), new FGPhoneListener());
}

void AuthLibraryDesktop::PromptGoogleSignIn(const FAuthCredentialsDelegate& OnSuccess, const FAuthStringDelegate& OnError)
{
	// TODO not available on desktop
}


#endif
