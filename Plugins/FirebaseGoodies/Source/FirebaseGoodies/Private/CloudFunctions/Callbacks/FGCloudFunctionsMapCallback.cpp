#include "FGCloudFunctionsMapCallback.h"
#include "Async/Async.h"

UFGCloudFunctionsMapCallback::UFGCloudFunctionsMapCallback()
{
	if (StaticClass()->GetDefaultObject() != this)
	{
		AddToRoot();
	}
}

void UFGCloudFunctionsMapCallback::BindSuccessDelegate(const FCloudFunctionsMapDelegate& OnSuccess)
{
	OnCallMapFunctionSuccessDelegate = OnSuccess;
}

void UFGCloudFunctionsMapCallback::ExecuteSuccess(const FMapWrapper& Map)
{
	AsyncTask(ENamedThreads::GameThread, [=]() {
		OnCallMapFunctionSuccessDelegate.ExecuteIfBound(Map);
		this->RemoveFromRoot();
	});
}
