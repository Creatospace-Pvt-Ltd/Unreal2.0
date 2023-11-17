#pragma once

#include "Containers/UnrealString.h"

class IFirebaseAuthCredentials
{
public:
	virtual ~IFirebaseAuthCredentials() = default;
	virtual FString GetProvider() = 0;
	virtual bool AreValid() = 0;
};
