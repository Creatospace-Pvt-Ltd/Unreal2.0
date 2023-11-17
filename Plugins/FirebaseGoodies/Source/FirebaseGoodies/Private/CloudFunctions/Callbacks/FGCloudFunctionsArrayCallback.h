// Copyright (c) 2023 Nineva Studios

#pragma once

#include "FGCloudFunctionsCallback.h"
#include "CloudFunctions/FGCloudFunctions.h"

#include "FGCloudFunctionsArrayCallback.generated.h"

UCLASS()
class FIREBASEGOODIES_API UFGCloudFunctionsArrayCallback : public UFGCloudFunctionsCallback
{
	GENERATED_BODY()

public:
	UFGCloudFunctionsArrayCallback();

	void BindSuccessDelegate(const FCloudFunctionsArrayDelegate& OnSuccess);
	// void BindErrorDelegate(const FCloudFunctionsStringDelegate& OnError);

	void ExecuteSuccess(const TArray<FFGValueVariant>& Array);
	// void ExecuteError(FString ErrorMessage);

private:
	UPROPERTY()
	FCloudFunctionsArrayDelegate OnCallArrayFunctionSuccessDelegate;
	// UPROPERTY()
	// FCloudFunctionsStringDelegate OnCallArrayFunctionErrorDelegate;
};
