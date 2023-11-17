#include "FGCloudStorageUploadCallback.h"
#include "Async/Async.h"

UFGCloudStorageUploadCallback::UFGCloudStorageUploadCallback()
{
	if (StaticClass()->GetDefaultObject() != this)
	{
		AddToRoot();
	}
}

void UFGCloudStorageUploadCallback::BindSuccessDelegate(const FCloudStorageVoidDelegate& OnSuccess)
{
	OnUploadFromDataSuccessDelegate = OnSuccess;
}

void UFGCloudStorageUploadCallback::BindProgressDelegate(const FCloudStorageIntDelegate& OnProgress)
{
	OnUploadFromDataProgressDelegate = OnProgress;
}

void UFGCloudStorageUploadCallback::BindErrorDelegate(const FCloudStorageStringDelegate& OnError)
{
	OnUploadFromDataErrorDelegate = OnError;
}

void UFGCloudStorageUploadCallback::ExecuteSuccess()
{
	AsyncTask(ENamedThreads::GameThread, [=]() {
		OnUploadFromDataSuccessDelegate.ExecuteIfBound();
		this->RemoveFromRoot();
	});
}

void UFGCloudStorageUploadCallback::ExecuteProgress(int Progress)
{
	AsyncTask(ENamedThreads::GameThread, [=]() {
		OnUploadFromDataProgressDelegate.ExecuteIfBound(Progress);
		this->RemoveFromRoot();
	});
}

void UFGCloudStorageUploadCallback::ExecuteError(FString ErrorMessage)
{
	AsyncTask(ENamedThreads::GameThread, [=]() {
		OnUploadFromDataErrorDelegate.ExecuteIfBound(ErrorMessage);
		this->RemoveFromRoot();
	});
	
}
