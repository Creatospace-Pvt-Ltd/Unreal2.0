#include "FGCloudFunctionsFloatCallback.h"
#include "Async/Async.h"

UFGCloudFunctionsFloatCallback::UFGCloudFunctionsFloatCallback()
{
	if (StaticClass()->GetDefaultObject() != this)
	{
		AddToRoot();
	}
}

void UFGCloudFunctionsFloatCallback::BindSuccessDelegate(const FCloudFunctionsFloatDelegate& OnSuccess)
{
	OnCallFloatFunctionSuccessDelegate = OnSuccess;
}

void UFGCloudFunctionsFloatCallback::ExecuteSuccess(float Num)
{
	AsyncTask(ENamedThreads::GameThread, [=]() {
		OnCallFloatFunctionSuccessDelegate.ExecuteIfBound(Num);
		this->RemoveFromRoot();
	});
}
