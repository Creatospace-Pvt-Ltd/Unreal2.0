#include "FGCloudFunctionsArrayCallback.h"
#include "Async/Async.h"

UFGCloudFunctionsArrayCallback::UFGCloudFunctionsArrayCallback()
{
	if (StaticClass()->GetDefaultObject() != this)
	{
		AddToRoot();
	}
}

void UFGCloudFunctionsArrayCallback::BindSuccessDelegate(const FCloudFunctionsArrayDelegate& OnSuccess)
{
	OnCallArrayFunctionSuccessDelegate = OnSuccess;
}


void UFGCloudFunctionsArrayCallback::ExecuteSuccess(const TArray<FFGValueVariant>& Array)
{
	AsyncTask(ENamedThreads::GameThread, [=]() {
		OnCallArrayFunctionSuccessDelegate.ExecuteIfBound(Array);
		this->RemoveFromRoot();
	});
}
