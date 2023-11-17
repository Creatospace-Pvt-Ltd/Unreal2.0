// Copyright (c) 2022 Nineva Studios

#include "Auth/FGFirebaseUserInfo.h"
#include "FirebaseGoodiesLog.h"
#include "Interface/IFirebaseUserInfo.h"

UFGFirebaseUserInfo::~UFGFirebaseUserInfo()
{
}

FString UFGFirebaseUserInfo::GetUid() const
{
	if (!IsUserInfoValid())
	{
		FString Result;
		return Result;
	}

	return UserInfoImpl->GetUid();
}

FString UFGFirebaseUserInfo::GetProviderId() const
{
	if (!IsUserInfoValid())
	{
		FString Result;
		return Result;
	}

	return UserInfoImpl->GetProviderId();
}

FString UFGFirebaseUserInfo::GetDisplayName() const
{
	if (!IsUserInfoValid())
	{
		FString Result;
		return Result;
	}

	return UserInfoImpl->GetDisplayName();
}

FString UFGFirebaseUserInfo::GetPhotoUrl() const
{
	if (!IsUserInfoValid())
	{
		FString Result;
		return Result;
	}

	return UserInfoImpl->GetPhotoUrl();
}

FString UFGFirebaseUserInfo::GetEmail() const
{
	if (!IsUserInfoValid())
	{
		FString Result;
		return Result;
	}

	return UserInfoImpl->GetEmail();
}

FString UFGFirebaseUserInfo::GetPhoneNumber() const
{
	if (!IsUserInfoValid())
	{
		FString Result;
		return Result;
	}

	return UserInfoImpl->GetPhoneNumber();
}

bool UFGFirebaseUserInfo::IsUserInfoValid() const
{
	const bool bIsUserInfoValid = UserInfoImpl.IsValid();

	if (!bIsUserInfoValid)
	{
		UE_LOG(LogFirebaseGoodies, Error, TEXT("User info is not valid."));
	}
	return bIsUserInfoValid;
}


void UFGFirebaseUserInfo::Init(const TSharedPtr<IFirebaseUserInfo> UserInfo)
{
	UserInfoImpl = UserInfo;
}
