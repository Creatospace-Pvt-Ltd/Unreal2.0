// Copyright (c) 2022 Nineva Studios

#pragma once

#include "Auth/FGAuthLibrary.h"

class IFirebaseUser;
class IFirebaseAuthCredentials;

class IFirebaseAuthLibrary
{
public:
	virtual ~IFirebaseAuthLibrary() = default;

	virtual void InitListeners(const FAuthVoidDelegate& IdTokenChangedDelegate, const FAuthVoidDelegate& AuthStateChangedDelegate) = 0;
	virtual TSharedPtr<IFirebaseUser> CurrentUser() = 0;
	virtual void CreateUser(const FString& Email, const FString& Password, const FAuthUserDelegate& OnSuccess, const FAuthStringDelegate& OnError) = 0;
	virtual void FetchProvidersForEmail(const FString& Email, const FSignInMethodsFetchedDelegate& OnSuccess, const FAuthStringDelegate& OnError) = 0;
	virtual void SendPasswordReset(const FString& Email, const FAuthVoidDelegate& OnSuccess, const FAuthStringDelegate& OnError) = 0;
	virtual void SignInWithEmailAndPassword(const FString& Email, const FString& Password, const FAuthUserDelegate& OnSuccess,
	                                        const FAuthStringDelegate& OnError) = 0;
	virtual void SignInWithToken(const FString& Token, const FAuthUserDelegate& OnSuccess, const FAuthStringDelegate& OnError) = 0;
	virtual void SignInAnonymously(const FAuthUserDelegate& OnSuccess, const FAuthStringDelegate& OnError) = 0;
	virtual void SignInWithCredentials(TSharedPtr<IFirebaseAuthCredentials> Credentials, const FAuthUserDelegate& OnSuccess, const FAuthStringDelegate& OnError) = 0;
	virtual void SignOut() = 0;
	virtual TSharedPtr<IFirebaseAuthCredentials> GetEmailCredentials(const FString& Email, const FString& Password) = 0;
	virtual TSharedPtr<IFirebaseAuthCredentials> GetFacebookCredentials(const FString& Token) = 0;
	virtual TSharedPtr<IFirebaseAuthCredentials> GetGithubCredentials(const FString& Token) = 0;
	virtual TSharedPtr<IFirebaseAuthCredentials> GetGoogleCredentials(const FString& IdToken, const FString& AccessToken) = 0;
	virtual TSharedPtr<IFirebaseAuthCredentials> GetAppleCredentials(const FString& IdToken, const FString& RawNonce) = 0;
	virtual void GetGameCenterCredentials(const FAuthCredentialsDelegate& OnSuccess, const FAuthStringDelegate& OnError) = 0;
	virtual TSharedPtr<IFirebaseAuthCredentials> GetGenericOAuthCredentials(const FString& Provider, const FString& IdToken, const FString& AccessToken) = 0;
	virtual TSharedPtr<IFirebaseAuthCredentials> GetPlayGamesCredentials(const FString& ServerAuthCode) = 0;
	virtual TSharedPtr<IFirebaseAuthCredentials> GetTwitterCredentials(const FString& Token, const FString& Secret) = 0;
	virtual TSharedPtr<IFirebaseAuthCredentials> GetPhoneCredentials(const FString& VerificationId, const FString& VerificationCode) = 0;
	virtual void VerifyPhoneNumber(const FString& Number, int TimeoutMillis, const FAuthCredentialsDelegate& OnSuccess,
	                               const FAuthStringDelegate& OnError, const FAuthStringDelegate& OnTimeout,
	                               const FAuthStringDelegate& OnSmsSent) = 0;
	virtual void PromptGoogleSignIn(const FAuthCredentialsDelegate& OnSuccess, const FAuthStringDelegate& OnError) = 0;
};
