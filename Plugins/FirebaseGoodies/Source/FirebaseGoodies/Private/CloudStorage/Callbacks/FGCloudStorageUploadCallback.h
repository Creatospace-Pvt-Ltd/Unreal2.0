// Copyright (c) 2023 Nineva Studios

#pragma once

#include "CloudStorage/FGCloudStorage.h"

#include "FGCloudStorageUploadCallback.generated.h"

UCLASS()
class FIREBASEGOODIES_API UFGCloudStorageUploadCallback : public UObject
{
	GENERATED_BODY()

public:
	UFGCloudStorageUploadCallback();
	
	void BindSuccessDelegate(const FCloudStorageVoidDelegate& OnSuccess);
	void BindProgressDelegate(const FCloudStorageIntDelegate& OnProgress);
	void BindErrorDelegate(const FCloudStorageStringDelegate& OnError);
	
	void ExecuteSuccess();
	void ExecuteProgress(int Progress);
	void ExecuteError(FString ErrorMessage);
	
private:
	UPROPERTY()
	FCloudStorageVoidDelegate OnUploadFromDataSuccessDelegate;
	UPROPERTY()
	FCloudStorageStringDelegate OnUploadFromDataErrorDelegate;
	UPROPERTY()
	FCloudStorageIntDelegate OnUploadFromDataProgressDelegate;
};
