#include "IOSFirebaseUserInfo.h"
#import <FirebaseAuth/FirebaseAuth.h>

#include <objc/objc.h>

IOSFirebaseUserInfo::IOSFirebaseUserInfo(id<FIRUserInfo> UserInfo)
{
	NativeUserInfo = (id<FIRUserInfo>) CFBridgingRetain(UserInfo);
}

IOSFirebaseUserInfo::~IOSFirebaseUserInfo()
{
	(id<FIRUserInfo>) CFBridgingRelease(NativeUserInfo);
	NativeUserInfo = nil;
}

FString IOSFirebaseUserInfo::GetUid()
{
	return FString(NativeUserInfo.uid);
}

FString IOSFirebaseUserInfo::GetProviderId()
{
	return FString(NativeUserInfo.uid);
}

FString IOSFirebaseUserInfo::GetDisplayName()
{
	return FString(NativeUserInfo.displayName);
}

FString IOSFirebaseUserInfo::GetPhotoUrl()
{
	return FString(NativeUserInfo.photoURL.absoluteString);
}

FString IOSFirebaseUserInfo::GetEmail()
{
	return FString(NativeUserInfo.email);
}

FString IOSFirebaseUserInfo::GetPhoneNumber()
{
	return FString(NativeUserInfo.phoneNumber);
}
bool IOSFirebaseUserInfo::IsUserInfoValid()
{
	return NativeUserInfo != nil;
}
