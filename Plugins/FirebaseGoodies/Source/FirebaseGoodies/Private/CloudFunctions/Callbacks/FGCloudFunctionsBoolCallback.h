// Copyright (c) 2023 Nineva Studios

#pragma once

#include "FGCloudFunctionsCallback.h"
#include "CloudFunctions/FGCloudFunctions.h"

#include "FGCloudFunctionsBoolCallback.generated.h"

UCLASS()
class FIREBASEGOODIES_API UFGCloudFunctionsBoolCallback : public UFGCloudFunctionsCallback
{
	GENERATED_BODY()

public:
	UFGCloudFunctionsBoolCallback();

	void BindSuccessDelegate(const FCloudFunctionsBoolDelegate& OnSuccess);
	// void BindErrorDelegate(const FCloudFunctionsStringDelegate& OnError);

	void ExecuteSuccess(bool Bool);
	// void ExecuteError(FString ErrorMessage);

private:
	UPROPERTY()
	FCloudFunctionsBoolDelegate OnCallBoolFunctionSuccessDelegate;
	// UPROPERTY()
	// FCloudFunctionsStringDelegate OnCallBoolFunctionErrorDelegate;
};
