// Copyright (c) 2023 Nineva Studios

#include "FGCloudStorageDownloadUrlCallback.h"

#include "Async/Async.h"

UFGCloudStorageDownloadUrlCallback::UFGCloudStorageDownloadUrlCallback()
{
	if (StaticClass()->GetDefaultObject() != this)
	{
		AddToRoot();
	}
}

void UFGCloudStorageDownloadUrlCallback::BindSuccessDelegate(const FCloudStorageStringDelegate& OnSuccess)
{
	OnDownloadSuccessDelegate = OnSuccess;
}

void UFGCloudStorageDownloadUrlCallback::BindErrorDelegate(const FCloudStorageStringDelegate& OnError)
{
	OnDownloadErrorDelegate = OnError;
}

void UFGCloudStorageDownloadUrlCallback::ExecuteSuccess(FString Url)
{
	AsyncTask(ENamedThreads::GameThread, [=]() {
		OnDownloadSuccessDelegate.ExecuteIfBound(Url);
		this->RemoveFromRoot();
	});
	
}

void UFGCloudStorageDownloadUrlCallback::ExecuteError(FString ErrorMessage)
{
	AsyncTask(ENamedThreads::GameThread, [=]() {
		OnDownloadErrorDelegate.ExecuteIfBound(ErrorMessage);
		this->RemoveFromRoot();
	});
}
