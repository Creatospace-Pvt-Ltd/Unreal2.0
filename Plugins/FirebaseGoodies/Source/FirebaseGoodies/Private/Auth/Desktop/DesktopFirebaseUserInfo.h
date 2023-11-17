#pragma once

#if (PLATFORM_WINDOWS || PLATFORM_MAC) && FG_ENABLE_EDITOR_SUPPORT

#include <firebase/auth/user.h>

#include "Auth/Interface/IFirebaseUserInfo.h"

class DesktopFirebaseUserInfo : public IFirebaseUserInfo
{
public:
	DesktopFirebaseUserInfo(firebase::auth::User::UserInfoInterface* Info);
	virtual ~DesktopFirebaseUserInfo() override;
	virtual FString GetUid() override;
	virtual FString GetProviderId() override;
	virtual FString GetDisplayName() override;
	virtual FString GetPhotoUrl() override;
	virtual FString GetEmail() override;
	virtual FString GetPhoneNumber() override;
	virtual bool IsUserInfoValid() override;

private:
	firebase::auth::User::UserInfoInterface* UserInfo;
};

#endif
