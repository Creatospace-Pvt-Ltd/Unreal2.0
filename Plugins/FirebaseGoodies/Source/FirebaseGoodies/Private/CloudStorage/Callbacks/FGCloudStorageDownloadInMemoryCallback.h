// Copyright (c) 2023 Nineva Studios

#pragma once

#include "CloudStorage/FGCloudStorage.h"

#include "FGCloudStorageDownloadInMemoryCallback.generated.h"

UCLASS()
class FIREBASEGOODIES_API UFGCloudStorageDownloadInMemoryCallback : public UObject
{
	GENERATED_BODY()

public:
	UFGCloudStorageDownloadInMemoryCallback();
	
	void BindSuccessDelegate(const FCloudStorageByteArrayDelegate& OnSuccess);
	void BindErrorDelegate(const FCloudStorageStringDelegate& OnError);
	
	void ExecuteSuccess(TArray<uint8> ByteArray);
	void ExecuteError(FString ErrorMessage);
	
private:
	UPROPERTY()
	FCloudStorageByteArrayDelegate OnDownloadInMemorySuccessDelegate;
	UPROPERTY()
	FCloudStorageStringDelegate OnDownloadInMemoryErrorDelegate;
	
};
