// Copyright (c) 2023 Nineva Studios

#pragma once
#include "Analytics/Interface/IAnalytics.h"

class AnalyticsIOS : public IAnalytics
{
public:
	virtual ~AnalyticsIOS() override;

	virtual void SetAnalyticsCollectionEnabled(bool bEnabled) override;
	virtual void ResetAnalyticsData() override;
	virtual void SetSessionTimeoutDuration(int64 Milliseconds) override;
	virtual void SetUserId(const FString& Id) override;
	virtual void SetUserProperty(const FString& Name, const FString& Value) override;
	virtual void SetCurrentScreen(const FString& ScreenName, const FString& ScreenClassName) override;
	virtual UFGAnalyticsParameter* CreateIntegerParameter(const FString& Name, int64 Value) override;
	virtual UFGAnalyticsParameter* CreateFloatParameter(const FString& Name, float Value) override;
	virtual UFGAnalyticsParameter* CreateStringParameter(const FString& Name, const FString& Value) override;
	virtual void LogEvent(const FString& EventName) override;
	virtual void LogEventWithParameter(const FString& EventName, UFGAnalyticsParameter* Parameter) override;
	virtual void LogEventWithParameters(const FString& EventName, const TArray<UFGAnalyticsParameter*>& Parameters) override;
};
