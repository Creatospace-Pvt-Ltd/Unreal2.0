#include "FGCloudStorageProgressCallback.h"
#include "Async/Async.h"

UFGCloudStorageProgressCallback::UFGCloudStorageProgressCallback()
{
	if (StaticClass()->GetDefaultObject() != this)
	{
		AddToRoot();
	}
}

void UFGCloudStorageProgressCallback::BindSuccessDelegate(const FCloudStorageStringDelegate& OnSuccess)
{
	OnDownloadToLocalFileSuccessDelegate = OnSuccess;
}

void UFGCloudStorageProgressCallback::BindErrorDelegate(const FCloudStorageStringDelegate& OnError)
{
	OnDownloadToLocalFileErrorDelegate = OnError;
}

void UFGCloudStorageProgressCallback::BindProgressDelegate(const FCloudStorageIntDelegate& OnProgress)
{
	OnDownloadToLocalFileProgress = OnProgress;
}

void UFGCloudStorageProgressCallback::ExecuteSuccess(FString FullPath)
{
	AsyncTask(ENamedThreads::GameThread, [=]() {
		OnDownloadToLocalFileSuccessDelegate.ExecuteIfBound(FullPath);
		this->RemoveFromRoot();
	});
}

void UFGCloudStorageProgressCallback::ExecuteError(FString ErrorMessage)
{
	AsyncTask(ENamedThreads::GameThread, [=]() {
		OnDownloadToLocalFileErrorDelegate.ExecuteIfBound(ErrorMessage);
		this->RemoveFromRoot();
	});
}

void UFGCloudStorageProgressCallback::ExecuteProgress(int Progress)
{
	AsyncTask(ENamedThreads::GameThread, [=]() {
		OnDownloadToLocalFileProgress.ExecuteIfBound(Progress);
		this->RemoveFromRoot();
	});
}
