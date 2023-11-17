#include "IOSFirebaseAuthCredentials.h"
#import <FirebaseAuth/FirebaseAuth.h>
#include "objc/objc.h"

IOSFirebaseAuthCredentials::IOSFirebaseAuthCredentials(FIRAuthCredential* Credential)
{
	NativeCredential = (FIRAuthCredential*)CFBridgingRetain(Credential);
}

IOSFirebaseAuthCredentials::~IOSFirebaseAuthCredentials()
{
	(FIRAuthCredential*)CFBridgingRelease(NativeCredential);
	NativeCredential = nil;
}

FString IOSFirebaseAuthCredentials::GetProvider()
{
	return NativeCredential.provider;
}

bool IOSFirebaseAuthCredentials::AreValid()
{
	return NativeCredential != nil;
}

FIRAuthCredential* IOSFirebaseAuthCredentials::GetNativeCredentials() const
{
	return NativeCredential;
}
