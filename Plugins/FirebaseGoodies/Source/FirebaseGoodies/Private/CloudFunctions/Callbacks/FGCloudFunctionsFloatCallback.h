// Copyright (c) 2023 Nineva Studios

#pragma once

#include "FGCloudFunctionsCallback.h"
#include "CloudFunctions/FGCloudFunctions.h"

#include "FGCloudFunctionsFloatCallback.generated.h"

UCLASS()
class FIREBASEGOODIES_API UFGCloudFunctionsFloatCallback : public UFGCloudFunctionsCallback
{
	GENERATED_BODY()

public:
	UFGCloudFunctionsFloatCallback();

	void BindSuccessDelegate(const FCloudFunctionsFloatDelegate& OnSuccess);
	// void BindErrorDelegate(const FCloudFunctionsStringDelegate& OnError);

	void ExecuteSuccess(float Num);
	// void ExecuteError(FString ErrorMessage);

private:
	UPROPERTY()
	FCloudFunctionsFloatDelegate OnCallFloatFunctionSuccessDelegate;
	// UPROPERTY()
	// FCloudFunctionsStringDelegate OnCallFloatFunctionErrorDelegate;
};
