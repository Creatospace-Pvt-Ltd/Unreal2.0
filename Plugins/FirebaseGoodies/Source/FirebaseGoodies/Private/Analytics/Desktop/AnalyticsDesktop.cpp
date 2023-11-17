#include "AnalyticsDesktop.h"

#if (PLATFORM_WINDOWS || PLATFORM_MAC) && FG_ENABLE_EDITOR_SUPPORT


#include "Analytics/FGAnalyticsParameter.h"
#include "FirebaseGoodiesLog.h"

#include "firebase/analytics.h"

#include "Desktop/FGUtils.h"


AnalyticsDesktop::~AnalyticsDesktop()
{
}

void AnalyticsDesktop::SetAnalyticsCollectionEnabled(bool bEnabled)
{
	firebase::analytics::SetAnalyticsCollectionEnabled(bEnabled);
}

void AnalyticsDesktop::ResetAnalyticsData()
{
	firebase::analytics::ResetAnalyticsData();
}

void AnalyticsDesktop::SetSessionTimeoutDuration(int64 Milliseconds)
{
	firebase::analytics::SetSessionTimeoutDuration(Milliseconds);
}

void AnalyticsDesktop::SetUserId(const FString& Id)
{
	firebase::analytics::SetUserId(TCHAR_TO_ANSI(*Id));
}

void AnalyticsDesktop::SetUserProperty(const FString& Name, const FString& Value)
{
	firebase::analytics::SetUserProperty(TCHAR_TO_ANSI(*Name), TCHAR_TO_ANSI(*Value));
}

void AnalyticsDesktop::SetCurrentScreen(const FString& ScreenName, const FString& ScreenClassName)
{
}

UFGAnalyticsParameter* AnalyticsDesktop::CreateIntegerParameter(const FString& Name, int64 Value)
{
	UFGAnalyticsParameter* parameter = NewObject<UFGAnalyticsParameter>();
	parameter->SetName(Name);
	parameter->SetInt(Value);

	return parameter;
}

UFGAnalyticsParameter* AnalyticsDesktop::CreateFloatParameter(const FString& Name, float Value)
{
	UFGAnalyticsParameter* parameter = NewObject<UFGAnalyticsParameter>();
	parameter->SetName(Name);
	parameter->SetFloat(Value);

	return parameter;
}

UFGAnalyticsParameter* AnalyticsDesktop::CreateStringParameter(const FString& Name, const FString& Value)
{
	UFGAnalyticsParameter* parameter = NewObject<UFGAnalyticsParameter>();
	parameter->SetName(Name);
	parameter->SetString(Value);

	return parameter;
}

void AnalyticsDesktop::LogEvent(const FString& EventName)
{
	firebase::analytics::LogEvent(TCHAR_TO_ANSI(*EventName));
}

void AnalyticsDesktop::LogEventWithParameter(const FString& EventName, UFGAnalyticsParameter* Parameter)
{
	TArray<UFGAnalyticsParameter*> parameters;
	parameters.Add(Parameter);
	LogEventWithParameters(EventName, parameters);
}

void AnalyticsDesktop::LogEventWithParameters(const FString& EventName,
                                              const TArray<UFGAnalyticsParameter*>& Parameters)
{
	const int size = Parameters.Num();

	TArray<firebase::analytics::Parameter> parameters;

	for (int i = 0; i < Parameters.Num(); i++)
	{
		firebase::analytics::Parameter parameter;

		char* parameterName = FGUtils::GetStringCopy(Parameters[i]->GetName());
		switch (Parameters[i]->GetType())
		{
		case EParameterType::Integer:
			{
				parameters.Add({parameterName, Parameters[i]->GetInt()});
				break;
			}
		case EParameterType::Float:
			{
				parameters.Add({parameterName, Parameters[i]->GetFloat()});
				break;
			}
		case EParameterType::String:
			{
				parameters.Add({parameterName, FGUtils::GetStringCopy(Parameters[i]->GetString())});
				break;
			}
		default:
			{
				UE_LOG(LogFirebaseGoodies, Error, TEXT("Trying to log an event with an undefined parameter"));
				break;
			}
		}
	}

	firebase::analytics::LogEvent(TCHAR_TO_ANSI(*EventName), parameters.GetData(), parameters.Num());

	FGUtils::ClearStringCopies();
}

#endif
