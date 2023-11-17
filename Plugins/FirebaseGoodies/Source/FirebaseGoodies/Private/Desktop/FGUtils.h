#pragma once

#include "Containers/UnrealString.h"

class FGUtils final
{
public:
	static char* GetStringCopy(const FString& Value);
	static void ClearStringCopies();
};
