// Copyright (c) 2023 Nineva Studios

#pragma once

#include "FGCloudFunctionsCallback.h"
#include "CloudFunctions/FGCloudFunctions.h"

#include "FGCloudFunctionsVoidCallback.generated.h"

UCLASS()
class FIREBASEGOODIES_API UFGCloudFunctionsVoidCallback : public UFGCloudFunctionsCallback
{
	GENERATED_BODY()

public:
	UFGCloudFunctionsVoidCallback();

	void BindSuccessDelegate(const FCloudFunctionsVoidDelegate& OnSuccess);
	// void BindErrorDelegate(const FCloudFunctionsStringDelegate& OnError);

	void ExecuteSuccess();
	// void ExecuteError(FString ErrorMessage);

private:
	UPROPERTY()
	FCloudFunctionsVoidDelegate OnCallVoidFunctionSuccessDelegate;
	// UPROPERTY()
	// FCloudFunctionsStringDelegate OnCallVoidFunctionErrorDelegate;
};
