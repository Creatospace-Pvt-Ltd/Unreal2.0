// Copyright (c) 2023 Nineva Studios

#pragma once

#include "CloudFunctions/FGCloudFunctions.h"

#include "FGCloudFunctionsCallback.generated.h"

UCLASS()
class FIREBASEGOODIES_API UFGCloudFunctionsCallback : public UObject
{
	GENERATED_BODY()

public:
	UFGCloudFunctionsCallback();

	void BindErrorDelegate(const FCloudFunctionsStringDelegate& OnError);

	void ExecuteError(FString ErrorMessage);

	// FORCEINLINE FCloudFunctionsStringDelegate GetErrorDelegate() { return OnCallErrorDelegate; }
	// FORCEINLINE void SetErrorDelegate(FCloudFunctionsStringDelegate Delegate) { OnCallErrorDelegate = Delegate; }
private:

	UPROPERTY()
	FCloudFunctionsStringDelegate OnCallErrorDelegate;
};
