// Copyright (c) 2022 Nineva Studios

#include "Analytics/FGAnalytics.h"

#include "Interface/IAnalytics.h"

#if PLATFORM_ANDROID
#include "Android/AnalyticsAndroid.h"
#endif

#if PLATFORM_IOS
#import "IOS/AnalyticsIOS.h"
#endif

#if (PLATFORM_WINDOWS || PLATFORM_MAC) && FG_ENABLE_EDITOR_SUPPORT
#include "Desktop/AnalyticsDesktop.h"
#endif


#if (PLATFORM_WINDOWS || PLATFORM_MAC) && !FG_ENABLE_EDITOR_SUPPORT
TSharedPtr<IAnalytics> UFGAnalytics::AnalyticsImpl = nullptr;
#endif

#if (PLATFORM_WINDOWS || PLATFORM_MAC) && FG_ENABLE_EDITOR_SUPPORT
TSharedPtr<IAnalytics> UFGAnalytics::AnalyticsImpl = MakeShareable(new AnalyticsDesktop());;
#endif

#if PLATFORM_ANDROID
TSharedPtr<IAnalytics> UFGAnalytics::AnalyticsImpl = MakeShareable(new AnalyticsAndroid());;
#endif

#if PLATFORM_IOS
TSharedPtr<IAnalytics> UFGAnalytics::AnalyticsImpl = MakeShareable(new AnalyticsIOS());;
#endif

// static const ANSICHAR* FGAnalyticsClassName = "com/ninevastudios/unrealfirebase/FGAnalytics";

void UFGAnalytics::SetAnalyticsCollectionEnabled(bool bEnabled)
{
	if (!AnalyticsImpl)
		return;

	AnalyticsImpl->SetAnalyticsCollectionEnabled(bEnabled);
}

void UFGAnalytics::ResetAnalyticsData()
{
	if (!AnalyticsImpl)
		return;

	AnalyticsImpl->ResetAnalyticsData();
}

void UFGAnalytics::SetSessionTimeoutDuration(int64 Milliseconds)
{
	if (!AnalyticsImpl)
		return;

	AnalyticsImpl->SetSessionTimeoutDuration(Milliseconds);
}

void UFGAnalytics::SetUserId(const FString& Id)
{
	if (!AnalyticsImpl)
		return;

	AnalyticsImpl->SetUserId(Id);
}

void UFGAnalytics::SetUserProperty(const FString& Name, const FString& Value)
{
	if (!AnalyticsImpl)
		return;

	AnalyticsImpl->SetUserProperty(Name, Value);
}

void UFGAnalytics::SetCurrentScreen(const FString& ScreenName, const FString& ScreenClassName)
{
	if (!AnalyticsImpl)
		return;

	AnalyticsImpl->SetCurrentScreen(ScreenName, ScreenClassName);
}

UFGAnalyticsParameter* UFGAnalytics::CreateIntegerParameter(const FString& Name, int64 Value)
{
	if (!AnalyticsImpl)
		return nullptr;


	return 	AnalyticsImpl->CreateIntegerParameter(Name, Value);
}

UFGAnalyticsParameter* UFGAnalytics::CreateFloatParameter(const FString& Name, float Value)
{
	if (!AnalyticsImpl)
		return nullptr;


	return 	AnalyticsImpl->CreateFloatParameter(Name, Value);
}

UFGAnalyticsParameter* UFGAnalytics::CreateStringParameter(const FString& Name, const FString& Value)
{
	if (!AnalyticsImpl)
		return nullptr;


	return 	AnalyticsImpl->CreateStringParameter(Name, Value);
}

void UFGAnalytics::LogEvent(const FString& EventName)
{
	if (!AnalyticsImpl)
		return;

	AnalyticsImpl->LogEvent(EventName);
}

void UFGAnalytics::LogEventWithParameter(const FString& EventName, UFGAnalyticsParameter* Parameter)
{
	if (!AnalyticsImpl)
		return;

	AnalyticsImpl->LogEventWithParameter(EventName, Parameter);
}

void UFGAnalytics::LogEventWithParameters(const FString& EventName, const TArray<UFGAnalyticsParameter*>& Parameters)
{
	if (!AnalyticsImpl)
		return;

	AnalyticsImpl->LogEventWithParameters(EventName, Parameters);
}