#include "FGCloudFunctionsVoidCallback.h"
#include "Async/Async.h"

UFGCloudFunctionsVoidCallback::UFGCloudFunctionsVoidCallback()
{
	if (StaticClass()->GetDefaultObject() != this)
	{
		AddToRoot();
	}
}

void UFGCloudFunctionsVoidCallback::BindSuccessDelegate(const FCloudFunctionsVoidDelegate& OnSuccess)
{
	OnCallVoidFunctionSuccessDelegate = OnSuccess;
}

void UFGCloudFunctionsVoidCallback::ExecuteSuccess()
{
	AsyncTask(ENamedThreads::GameThread, [=]()
	{
		OnCallVoidFunctionSuccessDelegate.ExecuteIfBound();
		this->RemoveFromRoot();
	});
}
