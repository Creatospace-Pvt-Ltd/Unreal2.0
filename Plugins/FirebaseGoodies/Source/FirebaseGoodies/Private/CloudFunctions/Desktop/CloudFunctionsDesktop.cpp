// Copyright (c) 2023 Nineva Studios

#if (PLATFORM_WINDOWS || PLATFORM_MAC) && FG_ENABLE_EDITOR_SUPPORT

#include "CloudFunctionsDesktop.h"
#include "CloudFunctions/FGCloudFunctions.h"

#include "Async/Async.h"
#include "FirebaseGoodiesLog.h"
#include "Common/FGValueVariantAccess.h"

#include "firebase/functions.h"

#include "Desktop/FGUtils.h"
#include "Desktop/FGVariantUtils.h"

#include "CloudFunctions/Callbacks/FGCloudFunctionsStringCallback.h"
#include "CloudFunctions/Callbacks/FGCloudFunctionsIntCallback.h"
#include "CloudFunctions/Callbacks/FGCloudFunctionsFloatCallback.h"
#include "CloudFunctions/Callbacks/FGCloudFunctionsBoolCallback.h"
#include "CloudFunctions/Callbacks/FGCloudFunctionsVoidCallback.h"
#include "CloudFunctions/Callbacks/FGCloudFunctionsMapCallback.h"
#include "CloudFunctions/Callbacks/FGCloudFunctionsArrayCallback.h"

const FString DefaultRegion = "us-central1";

CloudFunctionsDesktop::~CloudFunctionsDesktop()
{
}

void CloudFunctionsDesktop::CallStringFunction(const FString& MethodName, FString Region,
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
	firebase::functions::Functions* functions = firebase::functions::Functions::GetInstance(firebase::App::GetInstance(), TCHAR_TO_ANSI(*Region));

	auto task = functions->GetHttpsCallable(TCHAR_TO_ANSI(*MethodName)).Call(FGVariantUtils::ValueVariantMapToFirebaseVariant(Parameters));
	task.AddOnCompletion([=](const ::firebase::Future<firebase::functions::HttpsCallableResult>& callback) {
		if (callback.status() == firebase::FutureStatus::kFutureStatusComplete && callback.error() == 0)
		{
			FString Result = FString(callback.result()->data().string_value());

			AsyncTask(ENamedThreads::GameThread, [=]() {
				NativeCallback->ExecuteSuccess(Result);
			});
		}
		else
		{
			FString ErrorMessage = FString(callback.error_message());

			UE_LOG(LogFirebaseGoodies, Error, TEXT("Call String Function error: %s"), *ErrorMessage);

			AsyncTask(ENamedThreads::GameThread, [=]() {
				NativeCallback->ExecuteError(ErrorMessage);
			});
		}

		FGUtils::ClearStringCopies();
	});
}

void CloudFunctionsDesktop::CallIntFunction(const FString& MethodName, FString Region,
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
	
	firebase::functions::Functions* functions = firebase::functions::Functions::GetInstance(firebase::App::GetInstance(), TCHAR_TO_ANSI(*Region));

	auto task = functions->GetHttpsCallable(TCHAR_TO_ANSI(*MethodName)).Call(FGVariantUtils::ValueVariantMapToFirebaseVariant(Parameters));
	task.AddOnCompletion([=](const ::firebase::Future<firebase::functions::HttpsCallableResult>& callback) {
		if (callback.status() == firebase::FutureStatus::kFutureStatusComplete && callback.error() == 0)
		{
			int Result = callback.result()->data().int64_value();

			AsyncTask(ENamedThreads::GameThread, [=]() {
				NativeCallback->ExecuteSuccess(Result);
			});
		}
		else
		{
			FString ErrorMessage = FString(callback.error_message());

			UE_LOG(LogFirebaseGoodies, Error, TEXT("Call Int Function error: %s"), *ErrorMessage);

			AsyncTask(ENamedThreads::GameThread, [=]() {
				NativeCallback->ExecuteError(ErrorMessage);
			});
		}

		FGUtils::ClearStringCopies();
	});
}

void CloudFunctionsDesktop::CallFloatFunction(const FString& MethodName, FString Region,
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
	
	firebase::functions::Functions* functions = firebase::functions::Functions::GetInstance(firebase::App::GetInstance(), TCHAR_TO_ANSI(*Region));

	auto task = functions->GetHttpsCallable(TCHAR_TO_ANSI(*MethodName)).Call(FGVariantUtils::ValueVariantMapToFirebaseVariant(Parameters));
	task.AddOnCompletion([=](const ::firebase::Future<firebase::functions::HttpsCallableResult>& callback) {
		if (callback.status() == firebase::FutureStatus::kFutureStatusComplete && callback.error() == 0)
		{
			float Result = callback.result()->data().double_value();

			AsyncTask(ENamedThreads::GameThread, [=]() {
				NativeCallback->ExecuteSuccess(Result);
			});
		}
		else
		{
			FString ErrorMessage = FString(callback.error_message());

			UE_LOG(LogFirebaseGoodies, Error, TEXT("Call Float Function error: %s"), *ErrorMessage);

			AsyncTask(ENamedThreads::GameThread, [=]() {
				NativeCallback->ExecuteError(ErrorMessage);
			});
		}

		FGUtils::ClearStringCopies();
	});
}

void CloudFunctionsDesktop::CallBoolFunction(const FString& MethodName, FString Region,
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

	firebase::functions::Functions* functions = firebase::functions::Functions::GetInstance(firebase::App::GetInstance(), TCHAR_TO_ANSI(*Region));

	auto task = functions->GetHttpsCallable(TCHAR_TO_ANSI(*MethodName)).Call(FGVariantUtils::ValueVariantMapToFirebaseVariant(Parameters));
	task.AddOnCompletion([=](const ::firebase::Future<firebase::functions::HttpsCallableResult>& callback) {
		if (callback.status() == firebase::FutureStatus::kFutureStatusComplete && callback.error() == 0)
		{
			bool Result = callback.result()->data().bool_value();

			AsyncTask(ENamedThreads::GameThread, [=]() {
				NativeCallback->ExecuteSuccess(Result);
			});
		}
		else
		{
			FString ErrorMessage = FString(callback.error_message());

			UE_LOG(LogFirebaseGoodies, Error, TEXT("Call Bool Function error: %s"), *ErrorMessage);

			AsyncTask(ENamedThreads::GameThread, [=]() {
				NativeCallback->ExecuteError(ErrorMessage);
			});
		}

		FGUtils::ClearStringCopies();
	});
}

void CloudFunctionsDesktop::CallVoidFunction(const FString& MethodName, FString Region,
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
	
	firebase::functions::Functions* functions = firebase::functions::Functions::GetInstance(firebase::App::GetInstance(), TCHAR_TO_ANSI(*Region));

	auto task = functions->GetHttpsCallable(TCHAR_TO_ANSI(*MethodName)).Call(FGVariantUtils::ValueVariantMapToFirebaseVariant(Parameters));
	task.AddOnCompletion([=](const ::firebase::Future<firebase::functions::HttpsCallableResult>& callback) {
		if (callback.status() == firebase::FutureStatus::kFutureStatusComplete && callback.error() == 0)
		{
			AsyncTask(ENamedThreads::GameThread, [=]() {
				NativeCallback->ExecuteSuccess();
			});
		}
		else
		{
			FString ErrorMessage = FString(callback.error_message());

			UE_LOG(LogFirebaseGoodies, Error, TEXT("Call Bool Function error: %s"), *ErrorMessage);

			AsyncTask(ENamedThreads::GameThread, [=]() {
				NativeCallback->ExecuteError(ErrorMessage);
			});
		}

		FGUtils::ClearStringCopies();
	});
}

void CloudFunctionsDesktop::CallMapFunction(const FString& MethodName, FString Region,
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

	firebase::functions::Functions* functions = firebase::functions::Functions::GetInstance(firebase::App::GetInstance(), TCHAR_TO_ANSI(*Region));

	auto task = functions->GetHttpsCallable(TCHAR_TO_ANSI(*MethodName)).Call(FGVariantUtils::ValueVariantMapToFirebaseVariant(Parameters));
	task.AddOnCompletion([=](const ::firebase::Future<firebase::functions::HttpsCallableResult>& callback) {
		if (callback.status() == firebase::FutureStatus::kFutureStatusComplete && callback.error() == 0)
		{
			FMapWrapper Wrapper;
			Wrapper.Map = FGVariantUtils::FirebaseVariantMapToValueVariantMap(callback.result()->data().map());

			AsyncTask(ENamedThreads::GameThread, [=]() {
				NativeCallback->ExecuteSuccess(Wrapper);
			});
		}
		else
		{
			FString ErrorMessage = FString(callback.error_message());

			UE_LOG(LogFirebaseGoodies, Error, TEXT("Call Array Function error: %s"), *ErrorMessage);

			AsyncTask(ENamedThreads::GameThread, [=]() {
				NativeCallback->ExecuteError(ErrorMessage);
			});
		}

		FGUtils::ClearStringCopies();
	});
}

void CloudFunctionsDesktop::CallArrayFunction(const FString& MethodName, FString Region,
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

	firebase::functions::Functions* functions = firebase::functions::Functions::GetInstance(firebase::App::GetInstance(), TCHAR_TO_ANSI(*Region));

	auto task = functions->GetHttpsCallable(TCHAR_TO_ANSI(*MethodName)).Call(FGVariantUtils::ValueVariantMapToFirebaseVariant(Parameters));
	task.AddOnCompletion([=](const ::firebase::Future<firebase::functions::HttpsCallableResult>& callback) {
		if (callback.status() == firebase::FutureStatus::kFutureStatusComplete && callback.error() == 0)
		{
			TArray<FFGValueVariant> Result = FGVariantUtils::FirebaseVariantVectorToValueVariantArray(callback.result()->data().vector());

			AsyncTask(ENamedThreads::GameThread, [=]() {
				NativeCallback->ExecuteSuccess(Result);
			});
		}
		else
		{
			FString ErrorMessage = FString(callback.error_message());

			UE_LOG(LogFirebaseGoodies, Error, TEXT("Call Array Function error: %s"), *ErrorMessage);

			AsyncTask(ENamedThreads::GameThread, [=]() {
				NativeCallback->ExecuteError(ErrorMessage);
			});
		}

		FGUtils::ClearStringCopies();
	});
}

#endif