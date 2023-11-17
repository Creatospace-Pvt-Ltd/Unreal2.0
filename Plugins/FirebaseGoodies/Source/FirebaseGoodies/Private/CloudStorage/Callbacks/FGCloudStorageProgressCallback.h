// Copyright (c) 2023 Nineva Studios

#pragma once

#include "CloudStorage/FGCloudStorage.h"
#include "FGCloudStorageProgressCallback.generated.h"

UCLASS()
class FIREBASEGOODIES_API UFGCloudStorageProgressCallback : public UObject
{
	GENERATED_BODY()

public:
	UFGCloudStorageProgressCallback();
	
	void BindSuccessDelegate(const FCloudStorageStringDelegate& OnSuccess);
	void BindProgressDelegate(const FCloudStorageIntDelegate& OnProgress);
	void BindErrorDelegate(const FCloudStorageStringDelegate& OnError);
	
	void ExecuteSuccess(FString FullPath);
	void ExecuteProgress(int Progress);
	void ExecuteError(FString ErrorMessage);
	
private:
	UPROPERTY()
	FCloudStorageStringDelegate OnDownloadToLocalFileSuccessDelegate;
	UPROPERTY()
	FCloudStorageStringDelegate OnDownloadToLocalFileErrorDelegate;
	UPROPERTY()
	FCloudStorageIntDelegate OnDownloadToLocalFileProgress;
};
