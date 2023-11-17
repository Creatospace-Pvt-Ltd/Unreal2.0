#pragma once
#include "Auth/Interface/IFirebaseAuthLibrary.h"

class AuthLibraryIOS : public IFirebaseAuthLibrary
{
public:
	virtual ~AuthLibraryIOS() override;
	virtual void InitListeners(const FAuthVoidDelegate& IdTokenChangedDelegate, const FAuthVoidDelegate& AuthStateChangedDelegate) override;
	virtual TSharedPtr<IFirebaseUser> CurrentUser() override;
	virtual void CreateUser(const FString& Email, const FString& Password, const FAuthUserDelegate& OnSuccess, const FAuthStringDelegate& OnError) override;
	virtual void FetchProvidersForEmail(const FString& Email, const FSignInMethodsFetchedDelegate& OnSuccess, const FAuthStringDelegate& OnError) override;
	virtual void SendPasswordReset(const FString& Email, const FAuthVoidDelegate& OnSuccess, const FAuthStringDelegate& OnError) override;
	virtual void SignInWithEmailAndPassword(const FString& Email, const FString& Password, const FAuthUserDelegate& OnSuccess, const FAuthStringDelegate& OnError) override;
	virtual void SignInWithToken(const FString& Token, const FAuthUserDelegate& OnSuccess, const FAuthStringDelegate& OnError) override;
	virtual void SignInAnonymously(const FAuthUserDelegate& OnSuccess, const FAuthStringDelegate& OnError) override;
	virtual void SignInWithCredentials(TSharedPtr<IFirebaseAuthCredentials> Credentials, const FAuthUserDelegate& OnSuccess, const FAuthStringDelegate& OnError) override;
	virtual void SignOut() override;
	virtual TSharedPtr<IFirebaseAuthCredentials> GetEmailCredentials(const FString& Email, const FString& Password) override;
	virtual TSharedPtr<IFirebaseAuthCredentials> GetFacebookCredentials(const FString& Token) override;
	virtual TSharedPtr<IFirebaseAuthCredentials> GetGithubCredentials(const FString& Token) override;
	virtual TSharedPtr<IFirebaseAuthCredentials> GetGoogleCredentials(const FString& IdToken, const FString& AccessToken) override;
	virtual TSharedPtr<IFirebaseAuthCredentials> GetAppleCredentials(const FString& IdToken, const FString& RawNonce) override;
	virtual void GetGameCenterCredentials(const FAuthCredentialsDelegate& OnSuccess, const FAuthStringDelegate& OnError) override;
	virtual TSharedPtr<IFirebaseAuthCredentials> GetGenericOAuthCredentials(const FString& Provider, const FString& IdToken, const FString& AccessToken) override;
	virtual TSharedPtr<IFirebaseAuthCredentials> GetPlayGamesCredentials(const FString& ServerAuthCode) override;
	virtual TSharedPtr<IFirebaseAuthCredentials> GetTwitterCredentials(const FString& Token, const FString& Secret) override;
	virtual TSharedPtr<IFirebaseAuthCredentials> GetPhoneCredentials(const FString& VerificationId, const FString& VerificationCode) override;
	virtual void VerifyPhoneNumber(const FString& Number, int TimeoutMillis, const FAuthCredentialsDelegate& OnSuccess, const FAuthStringDelegate& OnError, const FAuthStringDelegate& OnTimeout,
		const FAuthStringDelegate& OnSmsSent) override;
	virtual void PromptGoogleSignIn(const FAuthCredentialsDelegate& OnSuccess, const FAuthStringDelegate& OnError) override;
};
