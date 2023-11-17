// Copyright (c) 2023 Nineva Studios

#pragma once

#include "FGCloudFunctionsCallback.h"
#include "CloudFunctions/FGCloudFunctions.h"

#include "FGCloudFunctionsStringCallback.generated.h"

UCLASS()
class FIREBASEGOODIES_API UFGCloudFunctionsStringCallback : public UFGCloudFunctionsCallback
{
	GENERATED_BODY()

public:
	UFGCloudFunctionsStringCallback();

	void BindSuccessDelegate(const FCloudFunctionsStringDelegate& OnSuccess);
	// void BindErrorDelegate(const FCloudFunctionsStringDelegate& OnError);

	void ExecuteSuccess(FString String);
	// void ExecuteError(FString ErrorMessage);

private:
	UPROPERTY()
	FCloudFunctionsStringDelegate OnCallStringFunctionSuccessDelegate;
	// UPROPERTY()
	// FCloudFunctionsStringDelegate OnCallStringFunctionErrorDelegate;
};
