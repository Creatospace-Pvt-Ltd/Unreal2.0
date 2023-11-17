#include "CloudFunctionsIOS.h"
#include "CloudFunctions/FGCloudFunctions.h"

#include "IOS/Utils/FGCloudFunctionsErrorUtils.h"
#include "IOS/Utils/FGValueVariantUtils.h"

#include "FirebaseGoodiesLog.h"
#include "Async/Async.h"
#include "Common/FGValueVariantAccess.h"
#import <FirebaseFunctions/FirebaseFunctions.h>

#include "CloudFunctions/Callbacks/FGCloudFunctionsStringCallback.h"
#include "CloudFunctions/Callbacks/FGCloudFunctionsIntCallback.h"
#include "CloudFunctions/Callbacks/FGCloudFunctionsFloatCallback.h"
#include "CloudFunctions/Callbacks/FGCloudFunctionsBoolCallback.h"
#include "CloudFunctions/Callbacks/FGCloudFunctionsVoidCallback.h"
#include "CloudFunctions/Callbacks/FGCloudFunctionsMapCallback.h"
#include "CloudFunctions/Callbacks/FGCloudFunctionsArrayCallback.h"

const FString DefaultRegion = "us-central1";

CloudFunctionsIOS::~CloudFunctionsIOS()
{
}

void CloudFunctionsIOS::CallStringFunction(const FString& MethodName, FString Region,
	const TMap<FString, FFGValueVariant>& Parameters, const FCloudFunctionsStringDelegate& OnSuccess,
	const FCloudFunctionsStringDelegate& OnError)
{
	UFGCloudFunctionsStringCallback* NativeCallback = NewObject<UFGCloudFunctionsStringCallback>();
	NativeCallback->BindSuccessDelegate(OnSuccess);
	NativeCallback->BindErrorDelegate(OnError);

	if (Region.IsEmpty())
	{
		Region = DefaultRegion;
	}

	FIRFunctions* functions = [FIRFunctions functionsForRegion:Region.GetNSString()];

	id ParametersObject = ValueVariantUtils::VariantMapToNSDictionary(Parameters);

	[[functions HTTPSCallableWithName:MethodName.GetNSString()] callWithObject:ParametersObject
																	completion:^(FIRHTTPSCallableResult* result,
																		NSError* error) {
																		if (error)
																		{
																			FString ErrorMessage = CloudFunctionsErrorUtils::HandleError(error);
																			AsyncTask(ENamedThreads::GameThread, [=]()
																			{
																				NativeCallback->ExecuteError(ErrorMessage);
																			});
																		}
																		else
																		{
																			FFGValueVariant Data = ValueVariantUtils::IdToVariant(result.data);
																			AsyncTask(ENamedThreads::GameThread, [=]() {
																				NativeCallback->ExecuteSuccess(UFGValueVariantAccess::GetString(Data));
																			});
																		}
																	}];
}

void CloudFunctionsIOS::CallIntFunction(const FString& MethodName, FString Region,
	const TMap<FString, FFGValueVariant>& Parameters, const FCloudFunctionsIntDelegate& OnSuccess,
	const FCloudFunctionsStringDelegate& OnError)
{
	UFGCloudFunctionsIntCallback* NativeCallback = NewObject<UFGCloudFunctionsIntCallback>();
	NativeCallback->BindSuccessDelegate(OnSuccess);
	NativeCallback->BindErrorDelegate(OnError);

	if (Region.IsEmpty())
	{
		Region = DefaultRegion;
	}

	FIRFunctions* functions = [FIRFunctions functionsForRegion:Region.GetNSString()];

	id ParametersObject = ValueVariantUtils::VariantMapToNSDictionary(Parameters);

	[[functions HTTPSCallableWithName:MethodName.GetNSString()] callWithObject:ParametersObject
																	completion:^(FIRHTTPSCallableResult* result,
																		NSError* error) {
																		if (error)
																		{
																			FString ErrorMessage = CloudFunctionsErrorUtils::HandleError(error);
																			AsyncTask(ENamedThreads::GameThread, [=]()
																			{
																				NativeCallback->ExecuteError(ErrorMessage);
																			});
																		}
																		else
																		{
																			FFGValueVariant Data = ValueVariantUtils::IdToVariant(result.data);
																			AsyncTask(ENamedThreads::GameThread, [=]() {
																				NativeCallback->ExecuteSuccess(UFGValueVariantAccess::GetInteger(Data));
																			});
																		}
																	}];
}

void CloudFunctionsIOS::CallFloatFunction(const FString& MethodName, FString Region,
	const TMap<FString, FFGValueVariant>& Parameters, const FCloudFunctionsFloatDelegate& OnSuccess,
	const FCloudFunctionsStringDelegate& OnError)
{
	UFGCloudFunctionsFloatCallback* NativeCallback = NewObject<UFGCloudFunctionsFloatCallback>();
	NativeCallback->BindSuccessDelegate(OnSuccess);
	NativeCallback->BindErrorDelegate(OnError);
	
	if (Region.IsEmpty())
	{
		Region = DefaultRegion;
	}

	FIRFunctions* functions = [FIRFunctions functionsForRegion:Region.GetNSString()];

	id ParametersObject = ValueVariantUtils::VariantMapToNSDictionary(Parameters);

	[[functions HTTPSCallableWithName:MethodName.GetNSString()] callWithObject:ParametersObject
																	completion:^(FIRHTTPSCallableResult* result,
																		NSError* error) {
																		if (error)
																		{
																			FString ErrorMessage = CloudFunctionsErrorUtils::HandleError(error);
																			AsyncTask(ENamedThreads::GameThread, [=]()
																			{
																				NativeCallback->ExecuteError(ErrorMessage);
																			});
																		}
																		else
																		{
																			FFGValueVariant Data = ValueVariantUtils::IdToVariant(result.data);
																			AsyncTask(ENamedThreads::GameThread, [=]() {
																				NativeCallback->ExecuteSuccess(UFGValueVariantAccess::GetFloat(Data));
																			});
																		}
																	}];
}

void CloudFunctionsIOS::CallBoolFunction(const FString& MethodName, FString Region,
	const TMap<FString, FFGValueVariant>& Parameters, const FCloudFunctionsBoolDelegate& OnSuccess,
	const FCloudFunctionsStringDelegate& OnError)
{
	UFGCloudFunctionsBoolCallback* NativeCallback = NewObject<UFGCloudFunctionsBoolCallback>();
	NativeCallback->BindSuccessDelegate(OnSuccess);
	NativeCallback->BindErrorDelegate(OnError);
	
	if (Region.IsEmpty())
	{
		Region = DefaultRegion;
	}

	FIRFunctions* functions = [FIRFunctions functionsForRegion:Region.GetNSString()];

	id ParametersObject = ValueVariantUtils::VariantMapToNSDictionary(Parameters);

	[[functions HTTPSCallableWithName:MethodName.GetNSString()] callWithObject:ParametersObject
																	completion:^(FIRHTTPSCallableResult* result,
																		NSError* error) {
																		if (error)
																		{
																			FString ErrorMessage = CloudFunctionsErrorUtils::HandleError(error);
																			AsyncTask(ENamedThreads::GameThread, [=]()
																			{
																				NativeCallback->ExecuteError(ErrorMessage);
																			});
																		}
																		else
																		{
																			FFGValueVariant Data = ValueVariantUtils::IdToVariant(result.data);
																			AsyncTask(ENamedThreads::GameThread, [=]() {
																				NativeCallback->ExecuteSuccess(UFGValueVariantAccess::GetBool(Data));
																			});
																		}
																	}];
}

void CloudFunctionsIOS::CallVoidFunction(const FString& MethodName, FString Region,
	const TMap<FString, FFGValueVariant>& Parameters, const FCloudFunctionsVoidDelegate& OnSuccess,
	const FCloudFunctionsStringDelegate& OnError)
{
	UFGCloudFunctionsVoidCallback* NativeCallback = NewObject<UFGCloudFunctionsVoidCallback>();
	NativeCallback->BindSuccessDelegate(OnSuccess);
	NativeCallback->BindErrorDelegate(OnError);
	
	if (Region.IsEmpty())
	{
		Region = DefaultRegion;
	}

	FIRFunctions* functions = [FIRFunctions functionsForRegion:Region.GetNSString()];

	id ParametersObject = ValueVariantUtils::VariantMapToNSDictionary(Parameters);

	[[functions HTTPSCallableWithName:MethodName.GetNSString()] callWithObject:ParametersObject
																	completion:^(FIRHTTPSCallableResult* result,
																		NSError* error) {
																		if (error)
																		{
																			FString ErrorMessage = CloudFunctionsErrorUtils::HandleError(error);
																			AsyncTask(ENamedThreads::GameThread, [=]()
																			{
																				NativeCallback->ExecuteError(ErrorMessage);
																			});
																		}
																		else
																		{
																			AsyncTask(ENamedThreads::GameThread, [=]() {
																				NativeCallback->ExecuteSuccess();
																			});
																		}
																	}];
}

void CloudFunctionsIOS::CallMapFunction(const FString& MethodName, FString Region,
	const TMap<FString, FFGValueVariant>& Parameters, const FCloudFunctionsMapDelegate& OnSuccess,
	const FCloudFunctionsStringDelegate& OnError)
{
	UFGCloudFunctionsMapCallback* NativeCallback = NewObject<UFGCloudFunctionsMapCallback>();
	NativeCallback->BindSuccessDelegate(OnSuccess);
	NativeCallback->BindErrorDelegate(OnError);
	
	if (Region.IsEmpty())
	{
		Region = DefaultRegion;
	}

	FIRFunctions* functions = [FIRFunctions functionsForRegion:Region.GetNSString()];

	id ParametersObject = ValueVariantUtils::VariantMapToNSDictionary(Parameters);

	[[functions HTTPSCallableWithName:MethodName.GetNSString()] callWithObject:ParametersObject
																	completion:^(FIRHTTPSCallableResult* result,
																		NSError* error) {
																		if (error)
																		{
																			FString ErrorMessage = CloudFunctionsErrorUtils::HandleError(error);
																			AsyncTask(ENamedThreads::GameThread, [=]()
																			{
																				NativeCallback->ExecuteError(ErrorMessage);
																			});
																		}
																		else
																		{
																			FMapWrapper Result;
																			Result.Map = ValueVariantUtils::NSDictionaryToVariantMap(result.data);
																			AsyncTask(ENamedThreads::GameThread, [=]() {
																				NativeCallback->ExecuteSuccess(Result);
																			});
																		}
																	}];
}

void CloudFunctionsIOS::CallArrayFunction(const FString& MethodName, FString Region,
	const TMap<FString, FFGValueVariant>& Parameters, const FCloudFunctionsArrayDelegate& OnSuccess,
	const FCloudFunctionsStringDelegate& OnError)
{
	UFGCloudFunctionsArrayCallback* NativeCallback = NewObject<UFGCloudFunctionsArrayCallback>();
	NativeCallback->BindSuccessDelegate(OnSuccess);
	NativeCallback->BindErrorDelegate(OnError);
	
	if (Region.IsEmpty())
	{
		Region = DefaultRegion;
	}
	
	FIRFunctions* functions = [FIRFunctions functionsForRegion:Region.GetNSString()];

	id ParametersObject = ValueVariantUtils::VariantMapToNSDictionary(Parameters);

	[[functions HTTPSCallableWithName:MethodName.GetNSString()] callWithObject:ParametersObject
																	completion:^(FIRHTTPSCallableResult* result,
																		NSError* error) {
																		if (error)
																		{
																			FString ErrorMessage = CloudFunctionsErrorUtils::HandleError(error);
																			AsyncTask(ENamedThreads::GameThread, [=]()
																			{
																				NativeCallback->ExecuteError(ErrorMessage);
																			});
																		}
																		else
																		{
																			TArray<FFGValueVariant> Result = ValueVariantUtils::NSArrayToVariantArray(result.data);
																			AsyncTask(ENamedThreads::GameThread, [=]() {
																				NativeCallback->ExecuteSuccess(Result);
																			});
																		}
																	}];
}
