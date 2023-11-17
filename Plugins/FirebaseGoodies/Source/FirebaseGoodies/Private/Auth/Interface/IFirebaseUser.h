#pragma once

#include "Auth/FGFirebaseUser.h"
#include "IFirebaseUserInfo.h"

class IFirebaseUser : public IFirebaseUserInfo
{
public:
	virtual ~IFirebaseUser() override = default;

	virtual bool IsAnonymous() = 0;
	virtual bool IsEmailVerified() = 0;
	virtual bool IsUserValid() = 0;
	virtual void Delete(const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError) = 0;
	virtual void LinkWithCredentials(UFGAuthCredentials* Credentials, const FOnUserUpdated& OnSuccess, const FOnUserOperationError& OnError) = 0;
	virtual void Reauthenticate(UFGAuthCredentials* Credentials, const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError) = 0;
	virtual void Reload(const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError) = 0;
	virtual void SendEmailVerification(const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError) = 0;
	virtual void UnlinkProvider(const FString& Provider, const FOnUserUpdated& OnSuccess, const FOnUserOperationError& OnError) = 0;
	virtual void UpdateEmail(const FString& Email, const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError) = 0;
	virtual void UpdatePassword(const FString& Password, const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError) = 0;
	virtual void UpdatePhoneNumber(UFGAuthCredentials* Credentials, const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError) = 0;
	virtual void UpdateProfile(const FString& DisplayName, const FString& AvatarUrl, const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError) = 0;
	virtual void GetToken(bool ForceRefresh, const FOnUserStringTaskCompleted& OnSuccess, const FOnUserOperationError& OnError) = 0;
	virtual TArray<TSharedPtr<IFirebaseUserInfo>> GetProviderData() = 0;
};
