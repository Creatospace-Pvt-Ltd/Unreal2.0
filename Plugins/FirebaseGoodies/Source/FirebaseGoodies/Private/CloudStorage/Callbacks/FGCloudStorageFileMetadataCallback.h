// Copyright (c) 2023 Nineva Studios

#pragma once

#include "CloudStorage/FGCloudStorage.h"
#include "FGCloudStorageFileMetadataCallback.generated.h"

UCLASS()
class FIREBASEGOODIES_API UFGCloudStorageFileMetadataCallback : public UObject
{
	GENERATED_BODY()

public:
	UFGCloudStorageFileMetadataCallback();
	
	void BindSuccessDelegate(const FCloudStorageMetadataDelegate& OnSuccess);
	void BindErrorDelegate(const FCloudStorageStringDelegate& OnError);
	
	void ExecuteSuccess(UFGStorageMetadata* Metadata);
	void ExecuteError(FString ErrorMessage);
	
private:
	UPROPERTY()
	FCloudStorageMetadataDelegate OnGetFileMetadataSuccessDelegate;
	UPROPERTY()
	FCloudStorageStringDelegate OnGetFileMetadataErrorDelegate;
	
};
