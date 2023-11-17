#pragma once
#include "Containers/UnrealString.h"

#include "Containers/UnrealString.h"

class IFirebaseUserInfo
{
public:
	virtual ~IFirebaseUserInfo() = default;
	virtual FString GetUid() =0;
	virtual FString GetProviderId() =0;
	virtual FString GetDisplayName() =0;
	virtual FString GetPhotoUrl() =0;
	virtual FString GetEmail() =0;
	virtual FString GetPhoneNumber() =0;
	virtual bool IsUserInfoValid() =0;
};
