// Copyright (c) 2023 Nineva Studios

#include "FGCloudStorageDownloadInMemoryCallback.h"

#include "Async/Async.h"

UFGCloudStorageDownloadInMemoryCallback::UFGCloudStorageDownloadInMemoryCallback()
{
	if (StaticClass()->GetDefaultObject() != this)
	{
		AddToRoot();
	}
}

void UFGCloudStorageDownloadInMemoryCallback::BindSuccessDelegate(const FCloudStorageByteArrayDelegate& OnSuccess)
{
	OnDownloadInMemorySuccessDelegate = OnSuccess;
}

void UFGCloudStorageDownloadInMemoryCallback::BindErrorDelegate(const FCloudStorageStringDelegate& OnError)
{
	OnDownloadInMemoryErrorDelegate = OnError;
}

void UFGCloudStorageDownloadInMemoryCallback::ExecuteSuccess(TArray<uint8> ByteArray)
{
	AsyncTask(ENamedThreads::GameThread, [=]() {
		OnDownloadInMemorySuccessDelegate.ExecuteIfBound(ByteArray);
		this->RemoveFromRoot();
	});
}

void UFGCloudStorageDownloadInMemoryCallback::ExecuteError(FString ErrorMessage)
{
	AsyncTask(ENamedThreads::GameThread, [=]() {
		OnDownloadInMemoryErrorDelegate.ExecuteIfBound(ErrorMessage);
		this->RemoveFromRoot();
	});
}
