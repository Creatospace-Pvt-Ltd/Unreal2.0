// Copyright (c) 2022 Nineva Studios

#include "Auth/FGAuthCredentials.h"

#include "FirebaseGoodiesLog.h"
#include "Interface/IFirebaseAuthCredentials.h"

UFGAuthCredentials::~UFGAuthCredentials()
{
}

FString UFGAuthCredentials::GetProvider() const
{
	if (!AreValid())
	{
		FString Result;
		return Result;
	}

	return CredentialsImpl->GetProvider();
}

bool UFGAuthCredentials::AreValid() const
{
	const bool bAreCredentialsValid = CredentialsImpl->AreValid();

	if (!bAreCredentialsValid)
	{
		UE_LOG(LogFirebaseGoodies, Error, TEXT("Auth Credentials are not valid."));
	}
	return bAreCredentialsValid;
}

void UFGAuthCredentials::Init(TSharedPtr<IFirebaseAuthCredentials> Credentials)
{
	CredentialsImpl = Credentials;
}

TSharedPtr<IFirebaseAuthCredentials> UFGAuthCredentials::GetCredentials()
{
	return CredentialsImpl;
}
