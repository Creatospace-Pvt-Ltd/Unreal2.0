// Copyright (c) 2023 Nineva Studios

#include "FGCloudStorageDeleteFileCallback.h"

#include "Async/Async.h"

UFGCloudStorageDeleteFileCallback::UFGCloudStorageDeleteFileCallback()
{
	if (StaticClass()->GetDefaultObject() != this)
	{
		AddToRoot();
	}
}

void UFGCloudStorageDeleteFileCallback::BindSuccessDelegate(const FCloudStorageVoidDelegate& OnSuccess)
{
	OnDeleteSuccessDelegate = OnSuccess;
}

void UFGCloudStorageDeleteFileCallback::BindErrorDelegate(const FCloudStorageStringDelegate& OnError)
{
	OnDeleteErrorDelegate = OnError;
}

void UFGCloudStorageDeleteFileCallback::ExecuteSuccess()
{
	AsyncTask(ENamedThreads::GameThread, [=]() {
		OnDeleteSuccessDelegate.ExecuteIfBound();
		this->RemoveFromRoot();
	});
}

void UFGCloudStorageDeleteFileCallback::ExecuteError(FString ErrorMessage)
{
	AsyncTask(ENamedThreads::GameThread, [=]() {
		OnDeleteErrorDelegate.ExecuteIfBound(ErrorMessage);
		this->RemoveFromRoot();
	});
}
