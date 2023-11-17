#pragma once
#include "Auth/Interface/IFirebaseUserInfo.h"

#import <FirebaseAuth/FirebaseAuth.h>
#include <objc/objc.h>

@class FIRUserInfo;

class IOSFirebaseUserInfo : public IFirebaseUserInfo
{
public:
	IOSFirebaseUserInfo(id<FIRUserInfo> UserInfo);
	virtual ~IOSFirebaseUserInfo() override;
	virtual FString GetUid() override;
	virtual FString GetProviderId() override;
	virtual FString GetDisplayName() override;
	virtual FString GetPhotoUrl() override;
	virtual FString GetEmail() override;
	virtual FString GetPhoneNumber() override;
	virtual bool IsUserInfoValid() override;

private:
	id<FIRUserInfo> NativeUserInfo;
};
