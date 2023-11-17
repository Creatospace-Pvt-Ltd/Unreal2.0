// Copyright (c) 2023 Nineva Studios

#pragma once

#include "CloudStorage/FGCloudStorage.h"

#include "FGCloudStorageDeleteFileCallback.generated.h"

UCLASS()
class FIREBASEGOODIES_API UFGCloudStorageDeleteFileCallback : public UObject
{
	GENERATED_BODY()

public:
	UFGCloudStorageDeleteFileCallback();
	
	void BindSuccessDelegate(const FCloudStorageVoidDelegate& OnSuccess);
	void BindErrorDelegate(const FCloudStorageStringDelegate& OnError);
	
	void ExecuteSuccess();
	void ExecuteError(FString ErrorMessage);
	
private:
	UPROPERTY()
	FCloudStorageVoidDelegate OnDeleteSuccessDelegate;
	UPROPERTY()
	FCloudStorageStringDelegate OnDeleteErrorDelegate;
	
};
