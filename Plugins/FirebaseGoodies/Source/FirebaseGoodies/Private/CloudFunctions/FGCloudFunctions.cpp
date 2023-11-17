// Copyright (c) 2022 Nineva Studios

#include "CloudFunctions/FGCloudFunctions.h"

#include "Common/FGValueVariantAccess.h"
#include "Interface/ICloudFunctions.h"

#if (PLATFORM_WINDOWS || PLATFORM_MAC) && FG_ENABLE_EDITOR_SUPPORT
#include "Desktop/CloudFunctionsDesktop.h"
#endif

#if PLATFORM_ANDROID
#include "Android/CloudFunctionsAndroid.h"
#endif

#if PLATFORM_IOS
#include "IOS/CloudFunctionsIOS.h"
#endif

#if (PLATFORM_WINDOWS || PLATFORM_MAC) && !FG_ENABLE_EDITOR_SUPPORT
TSharedPtr<ICloudFunctions> UFGCloudFunctions::CloudFunctionsImpl = nullptr;
#endif

#if (PLATFORM_WINDOWS || PLATFORM_MAC) && FG_ENABLE_EDITOR_SUPPORT
TSharedPtr<ICloudFunctions> UFGCloudFunctions::CloudFunctionsImpl = MakeShareable(new CloudFunctionsDesktop());;
#endif

#if PLATFORM_ANDROID
TSharedPtr<ICloudFunctions> UFGCloudFunctions::CloudFunctionsImpl = MakeShareable(new CloudFunctionsAndroid());;
#endif

#if PLATFORM_IOS
TSharedPtr<ICloudFunctions> UFGCloudFunctions::CloudFunctionsImpl = MakeShareable(new CloudFunctionsIOS());;
#endif

void UFGCloudFunctions::CallStringFunction(const FString& MethodName, FString Region,
	const TMap<FString, FFGValueVariant>& Parameters,
	const FCloudFunctionsStringDelegate& OnSuccess,
	const FCloudFunctionsStringDelegate& OnError)
{
	if (!CloudFunctionsImpl)
		return;
	
	CloudFunctionsImpl->CallStringFunction(MethodName, Region, Parameters, OnSuccess, OnError);
}

void UFGCloudFunctions::CallIntFunction(const FString& MethodName, FString Region,
	const TMap<FString, FFGValueVariant>& Parameters,
	const FCloudFunctionsIntDelegate& OnSuccess,
	const FCloudFunctionsStringDelegate& OnError)
{
	if (!CloudFunctionsImpl)
		return;
	
	CloudFunctionsImpl->CallIntFunction(MethodName, Region, Parameters, OnSuccess, OnError);
}

void UFGCloudFunctions::CallFloatFunction(const FString& MethodName, FString Region,
	const TMap<FString, FFGValueVariant>& Parameters,
	const FCloudFunctionsFloatDelegate& OnSuccess,
	const FCloudFunctionsStringDelegate& OnError)
{
	if (!CloudFunctionsImpl)
		return;
	
	CloudFunctionsImpl->CallFloatFunction(MethodName, Region, Parameters, OnSuccess, OnError);
}

void UFGCloudFunctions::CallBoolFunction(const FString& MethodName, FString Region,
	const TMap<FString, FFGValueVariant>& Parameters,
	const FCloudFunctionsBoolDelegate& OnSuccess,
	const FCloudFunctionsStringDelegate& OnError)
{
	if (!CloudFunctionsImpl)
		return;
	
	CloudFunctionsImpl->CallBoolFunction(MethodName, Region, Parameters, OnSuccess, OnError);
}

void UFGCloudFunctions::CallVoidFunction(const FString& MethodName, FString Region,
	const TMap<FString, FFGValueVariant>& Parameters,
	const FCloudFunctionsVoidDelegate& OnSuccess,
	const FCloudFunctionsStringDelegate& OnError)
{
	if (!CloudFunctionsImpl)
		return;
	
	CloudFunctionsImpl->CallVoidFunction(MethodName, Region, Parameters, OnSuccess, OnError);
}

void UFGCloudFunctions::CallMapFunction(const FString& MethodName, FString Region,
	const TMap<FString, FFGValueVariant>& Parameters,
	const FCloudFunctionsMapDelegate& OnSuccess,
	const FCloudFunctionsStringDelegate& OnError)
{
	if (!CloudFunctionsImpl)
		return;
	
	CloudFunctionsImpl->CallMapFunction(MethodName, Region, Parameters, OnSuccess, OnError);
}

void UFGCloudFunctions::CallArrayFunction(const FString& MethodName, FString Region,
	const TMap<FString, FFGValueVariant>& Parameters,
	const FCloudFunctionsArrayDelegate& OnSuccess,
	const FCloudFunctionsStringDelegate& OnError)
{
	if (!CloudFunctionsImpl)
		return;
	
	CloudFunctionsImpl->CallArrayFunction(MethodName, Region, Parameters, OnSuccess, OnError);
}
