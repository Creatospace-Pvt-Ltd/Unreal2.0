#pragma once
#include "Auth/Interface/IFirebaseAuthCredentials.h"

#import <FirebaseAuth/FirebaseAuth.h>

class IOSFirebaseAuthCredentials : public IFirebaseAuthCredentials
{
public:
	IOSFirebaseAuthCredentials(FIRAuthCredential* Credential);
	virtual ~IOSFirebaseAuthCredentials() override;
	virtual FString GetProvider() override;
	virtual bool AreValid() override;
	
	FIRAuthCredential* GetNativeCredentials() const;

private:
	FIRAuthCredential* NativeCredential;
};
