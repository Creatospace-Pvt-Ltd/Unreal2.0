// Copyright (c) 2023 Nineva Studios

#pragma once

#include "CloudStorage/FGCloudStorage.h"

#include "FGCloudStorageDownloadUrlCallback.generated.h"

UCLASS()
class FIREBASEGOODIES_API UFGCloudStorageDownloadUrlCallback : public UObject
{
	GENERATED_BODY()

public:
	UFGCloudStorageDownloadUrlCallback();
	
	void BindSuccessDelegate(const FCloudStorageStringDelegate& OnSuccess);
	void BindErrorDelegate(const FCloudStorageStringDelegate& OnError);
	
	void ExecuteSuccess(FString Url);
	void ExecuteError(FString ErrorMessage);
	
private:
	UPROPERTY()
	FCloudStorageStringDelegate OnDownloadSuccessDelegate;
	UPROPERTY()
	FCloudStorageStringDelegate OnDownloadErrorDelegate;
	
};
