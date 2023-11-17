// Copyright (c) 2022 Nineva Studios

#pragma once

class IFirebaseAuthCredentials;

#include "FGAuthCredentials.generated.h"

UCLASS(BlueprintType)
class FIREBASEGOODIES_API UFGAuthCredentials : public UObject
{
	GENERATED_BODY()

	virtual ~UFGAuthCredentials();

public:
	/**
	* Get name of the provider, associated with these credentials.
	*/
	UFUNCTION(BlueprintCallable, Category = "Firebase Goodies|Auth|Credentials")
	FString GetProvider() const;

	/**
	* Convenience method to check if the native iOS/Android credentials object is valid.
	*/
	UFUNCTION(BlueprintCallable, Category = "Firebase Goodies|Auth|Credentials")
	bool AreValid() const;

	void Init(TSharedPtr<IFirebaseAuthCredentials> Credentials);
	TSharedPtr<IFirebaseAuthCredentials> GetCredentials();

private:
	TSharedPtr<IFirebaseAuthCredentials> CredentialsImpl;
};
