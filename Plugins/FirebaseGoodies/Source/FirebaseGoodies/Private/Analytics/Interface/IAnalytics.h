// Copyright (c) 2023 Nineva Studios

#pragma once

#include "Analytics/FGAnalyticsParameter.h"
#include "Analytics/FGAnalytics.h"

class IAnalytics
{
public:
	virtual ~IAnalytics() = default;

	virtual void SetAnalyticsCollectionEnabled(bool bEnabled) = 0;
	virtual void ResetAnalyticsData() = 0;
	virtual void SetSessionTimeoutDuration(int64 Milliseconds) = 0;
	virtual void SetUserId(const FString& Id) = 0;
	virtual void SetUserProperty(const FString& Name, const FString& Value) = 0;
	virtual void SetCurrentScreen(const FString& ScreenName, const FString& ScreenClassName) = 0;
	virtual UFGAnalyticsParameter* CreateIntegerParameter(const FString& Name, int64 Value) = 0;
	virtual UFGAnalyticsParameter* CreateFloatParameter(const FString& Name, float Value) = 0;
	virtual UFGAnalyticsParameter* CreateStringParameter(const FString& Name, const FString& Value) = 0;
	virtual void LogEvent(const FString& EventName) = 0;
	virtual void LogEventWithParameter(const FString& EventName, UFGAnalyticsParameter* Parameter) = 0;
	virtual void LogEventWithParameters(const FString& EventName, const TArray<UFGAnalyticsParameter*>& Parameters) = 0;
};
