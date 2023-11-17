#if (PLATFORM_WINDOWS || PLATFORM_MAC) && FG_ENABLE_EDITOR_SUPPORT

#include "DesktopFirebaseUserInfo.h"

DesktopFirebaseUserInfo::DesktopFirebaseUserInfo(firebase::auth::User::UserInfoInterface* Info)
{
	UserInfo = Info;
}

DesktopFirebaseUserInfo::~DesktopFirebaseUserInfo()
{
}

FString DesktopFirebaseUserInfo::GetUid()
{
	return UserInfo->uid().c_str();
}

FString DesktopFirebaseUserInfo::GetProviderId()
{
	return UserInfo->provider_id().c_str();
}

FString DesktopFirebaseUserInfo::GetDisplayName()
{
	return UserInfo->display_name().c_str();
}

FString DesktopFirebaseUserInfo::GetPhotoUrl()
{
	return UserInfo->photo_url().c_str();
}

FString DesktopFirebaseUserInfo::GetEmail()
{
	return UserInfo->email().c_str();
}

FString DesktopFirebaseUserInfo::GetPhoneNumber()
{
	return UserInfo->phone_number().c_str();
}

bool DesktopFirebaseUserInfo::IsUserInfoValid()
{
	return UserInfo != nullptr;
}
#endif
