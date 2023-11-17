// Copyright (c) 2023 Nineva Studios

#include "AnalyticsIOS.h"

#include "Analytics/FGAnalyticsParameter.h"
#include "FirebaseGoodiesLog.h"

#import <FirebaseAnalytics/FirebaseAnalytics.h>

AnalyticsIOS::~AnalyticsIOS()
{
}

void AnalyticsIOS::SetAnalyticsCollectionEnabled(bool bEnabled)
{
	[FIRAnalytics setAnalyticsCollectionEnabled:bEnabled];
}

void AnalyticsIOS::ResetAnalyticsData()
{
	[FIRAnalytics resetAnalyticsData];
}

void AnalyticsIOS::SetSessionTimeoutDuration(int64 Milliseconds)
{
	[FIRAnalytics setSessionTimeoutInterval:Milliseconds];
}

void AnalyticsIOS::SetUserId(const FString& Id)
{
	NSString* userId = !Id.IsEmpty() ? Id.GetNSString() : nil;
	[FIRAnalytics setUserID:userId];
}

void AnalyticsIOS::SetUserProperty(const FString& Name, const FString& Value)
{
	NSString* value = !Value.IsEmpty() ? Value.GetNSString() : nil;
	[FIRAnalytics setUserPropertyString:value forName:Name.GetNSString()];
}

void AnalyticsIOS::SetCurrentScreen(const FString& ScreenName, const FString& ScreenClassName)
{
	NSString* screenName = !ScreenName.IsEmpty() ? ScreenName.GetNSString() : nil;
	NSString* screenClassName = !ScreenClassName.IsEmpty() ? ScreenClassName.GetNSString() : nil;
	dispatch_async(dispatch_get_main_queue(), ^{
		[FIRAnalytics logEventWithName:kFIREventScreenView parameters: @{kFIRParameterScreenName: screenName, kFIRParameterScreenClass: screenClassName}];
	});
}

UFGAnalyticsParameter* AnalyticsIOS::CreateIntegerParameter(const FString& Name, int64 Value)
{
	UFGAnalyticsParameter* parameter = NewObject<UFGAnalyticsParameter>();
	parameter->SetName(Name);
	parameter->SetInt(Value);

	return parameter;
}

UFGAnalyticsParameter* AnalyticsIOS::CreateFloatParameter(const FString& Name, float Value)
{
	UFGAnalyticsParameter* parameter = NewObject<UFGAnalyticsParameter>();
	parameter->SetName(Name);
	parameter->SetFloat(Value);

	return parameter;
}

UFGAnalyticsParameter* AnalyticsIOS::CreateStringParameter(const FString& Name, const FString& Value)
{
	UFGAnalyticsParameter* parameter = NewObject<UFGAnalyticsParameter>();
	parameter->SetName(Name);
	parameter->SetString(Value);

	return parameter;
}

void AnalyticsIOS::LogEvent(const FString& EventName)
{
	[FIRAnalytics logEventWithName:EventName.GetNSString()
					parameters:nil];
}

void AnalyticsIOS::LogEventWithParameter(const FString& EventName, UFGAnalyticsParameter* Parameter)
{
	TArray<UFGAnalyticsParameter*> parameters;
	parameters.Add(Parameter);
	LogEventWithParameters(EventName, parameters);
}

void AnalyticsIOS::LogEventWithParameters(const FString& EventName, const TArray<UFGAnalyticsParameter*>& Parameters)
{
	NSMutableDictionary* parameters = [NSMutableDictionary dictionary];

	for (const auto parameter : Parameters)
	{
		switch (parameter->GetType())
		{
		case EParameterType::Integer:
			{
				NSNumber* paramValue = [NSNumber numberWithInt:parameter->GetInt()];
				parameters[parameter->GetName().GetNSString()] = paramValue;
				break;
			}
		case EParameterType::Float:
			{
				NSNumber* paramValue = [NSNumber numberWithFloat:parameter->GetFloat()];
				parameters[parameter->GetName().GetNSString()] = paramValue;
				break;
			}
		case EParameterType::String:
			{
				parameters[parameter->GetName().GetNSString()] = parameter->GetString().GetNSString();
				break;
			}
		default:
			{
				UE_LOG(LogFirebaseGoodies, Error, TEXT("Trying to log an event with an undefined parameter"));
				break;
			}
		}
	}

	[FIRAnalytics logEventWithName:EventName.GetNSString() parameters:parameters];
}
