#if (PLATFORM_WINDOWS || PLATFORM_MAC) && FG_ENABLE_EDITOR_SUPPORT
#include "DesktopFirebaseAuthCredentials.h"

DesktopFirebaseAuthCredentials::~DesktopFirebaseAuthCredentials()
{
}

FString DesktopFirebaseAuthCredentials::GetProvider()
{
	return DesktopCredential.provider().c_str();
}

bool DesktopFirebaseAuthCredentials::AreValid()
{
	return DesktopCredential.is_valid();
}

DesktopFirebaseAuthCredentials::DesktopFirebaseAuthCredentials(firebase::auth::Credential Credential)
{
	DesktopCredential = Credential;
}

firebase::auth::Credential DesktopFirebaseAuthCredentials::GetDesktopCredential() const
{
	return DesktopCredential;
}
#endif
