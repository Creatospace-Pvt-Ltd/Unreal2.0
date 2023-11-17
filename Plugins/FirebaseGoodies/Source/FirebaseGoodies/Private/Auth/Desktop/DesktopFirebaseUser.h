#pragma once


#if (PLATFORM_WINDOWS || PLATFORM_MAC) && FG_ENABLE_EDITOR_SUPPORT

#include <firebase/auth.h>
#include "Auth/Interface/IFirebaseUser.h"

class DesktopFirebaseUser : public IFirebaseUser
{
public:
	DesktopFirebaseUser(firebase::auth::User* User);

	virtual bool IsAnonymous() override;
	virtual bool IsUserValid() override;
	virtual void Delete(const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError) override;
	virtual void LinkWithCredentials(UFGAuthCredentials* Credentials, const FOnUserUpdated& OnSuccess, const FOnUserOperationError& OnError) override;
	virtual void Reauthenticate(UFGAuthCredentials* Credentials, const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError) override;
	virtual void Reload(const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError) override;
	virtual void SendEmailVerification(const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError) override;
	virtual void UnlinkProvider(const FString& Provider, const FOnUserUpdated& OnSuccess, const FOnUserOperationError& OnError) override;
	virtual void UpdateEmail(const FString& Email, const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError) override;
	virtual void UpdatePassword(const FString& Password, const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError) override;
	virtual void UpdatePhoneNumber(UFGAuthCredentials* Credentials, const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError) override;
	virtual void UpdateProfile(const FString& DisplayName, const FString& AvatarUrl, const FOnUserVoidTaskCompleted& OnSuccess, const FOnUserOperationError& OnError) override;
	virtual void GetToken(bool ForceRefresh, const FOnUserStringTaskCompleted& OnSuccess, const FOnUserOperationError& OnError) override;
	virtual TArray<TSharedPtr<IFirebaseUserInfo>> GetProviderData() override;
	virtual FString GetUid() override;
	virtual FString GetProviderId() override;
	virtual FString GetDisplayName() override;
	virtual FString GetPhotoUrl() override;
	virtual FString GetEmail() override;
	virtual FString GetPhoneNumber() override;
	virtual bool IsEmailVerified() override;
	virtual bool IsUserInfoValid() override;

private:
	firebase::auth::User* DesktopUser;
};

#endif
