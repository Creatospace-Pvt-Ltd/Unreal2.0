// Copyright (c) 2023 Nineva Studios

#include "FGCloudStorageFileMetadataCallback.h"

#include "Async/Async.h"

UFGCloudStorageFileMetadataCallback::UFGCloudStorageFileMetadataCallback()
{
	if (StaticClass()->GetDefaultObject() != this)
	{
		AddToRoot();
	}
}

void UFGCloudStorageFileMetadataCallback::BindSuccessDelegate(const FCloudStorageMetadataDelegate& OnSuccess)
{
	OnGetFileMetadataSuccessDelegate = OnSuccess;
}

void UFGCloudStorageFileMetadataCallback::BindErrorDelegate(const FCloudStorageStringDelegate& OnError)
{
	OnGetFileMetadataErrorDelegate = OnError;
}

void UFGCloudStorageFileMetadataCallback::ExecuteSuccess(UFGStorageMetadata* Metadata)
{
	AsyncTask(ENamedThreads::GameThread, [=]() {
		OnGetFileMetadataSuccessDelegate.ExecuteIfBound(Metadata);
		this->RemoveFromRoot();
	});
}

void UFGCloudStorageFileMetadataCallback::ExecuteError(FString ErrorMessage)
{
	AsyncTask(ENamedThreads::GameThread, [=]() {
		OnGetFileMetadataErrorDelegate.ExecuteIfBound(ErrorMessage);
		this->RemoveFromRoot();
	});
}
