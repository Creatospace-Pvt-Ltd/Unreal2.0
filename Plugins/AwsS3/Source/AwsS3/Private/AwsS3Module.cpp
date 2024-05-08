/**
* Copyright (C) 2017-2023 eelDev AB
*/

#include "AwsS3Module.h"
#include "AwsS3Logging.h"
#include "AwsS3PrivatePCH.h"

IMPLEMENT_MODULE(FAwsS3Module, AwsS3);
#define LOCTEXT_NAMESPACE "FAwsS3Module"
DEFINE_LOG_CATEGORY(LogAwsS3);

#if !DISABLE_S3
void* S3MemoryManagerWrapper::AllocateMemory(std::size_t blockSize, std::size_t alignment, const char* allocationTag)
{
	return ::operator new(blockSize);
}

void S3MemoryManagerWrapper::FreeMemory(void* memoryPtr)
{
	return ::operator delete(memoryPtr);
}

class FS3ConsoleLogger : public Aws::Utils::Logging::ConsoleLogSystem
{
public:
	FS3ConsoleLogger(Aws::Utils::Logging::LogLevel logLevel)
		: ConsoleLogSystem(logLevel)
	{
	}
protected:
	virtual void ProcessFormattedStatement(Aws::String&& statement) override
	{
		const FString Statement = UTF8_TO_TCHAR(statement.c_str());

		if (Statement.Contains("[FATAL]"))
		{
			LogError("%s", *Statement);
		}
		else if (Statement.Contains("[ERROR]"))
		{
			LogError("%s", *Statement);
		}
		else if (Statement.Contains("[WARN]"))
		{
			LogWarning("%s", *Statement);
		}
		else if (Statement.Contains("[INFO]"))
		{
			LogVerbose("%s", *Statement);
		}
		else if (Statement.Contains("[DEBUG]"))
		{
			LogVerbose("%s", *Statement);
		}
		else if (Statement.Contains("[TRACE]"))
		{
			LogVeryVerbose("%s", *Statement);
		}
		else
		{
			LogVerbose("%s", *Statement);
		}

		std::cout << statement;
	}
};
#endif

FAwsS3Module::FAwsS3Module()
#if !DISABLE_S3
	: m_sdkOptions(new Aws::SDKOptions)
#else
	: m_sdkOptions(nullptr)
#endif
{
#if PLATFORM_ANDROID && !DISABLE_S3
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	jobject Obj = FAndroidApplication::GetGameActivityThis();
	Aws::Platform::InitAndroid(Env, Obj);
#endif
}

FAwsS3Module::~FAwsS3Module()
{
#if !DISABLE_S3
	if (m_sdkOptions)
	{
		delete static_cast<Aws::SDKOptions *>(m_sdkOptions);
		m_sdkOptions = nullptr;
	}
#endif
}

void FAwsS3Module::StartupModule()
{
#if !DISABLE_S3
	LogVerbose("Loading AwsS3 Module");

	const TSharedPtr<IPlugin> PluginPtr = IPluginManager::Get().FindPlugin("AwsS3");
	
	FString PluginVersion;

	if (PluginPtr)
	{
		PluginVersion = PluginPtr->GetDescriptor().VersionName;
	}

	UE_LOG(LogTemp, Log, TEXT("--------------------------------------------------------------------------------"));
	UE_LOG(LogTemp, Log, TEXT("Using AwsS3 Version: %s"), *PluginVersion);
	UE_LOG(LogTemp, Log, TEXT("--------------------------------------------------------------------------------"));

	Aws::Utils::Logging::LogLevel LoggingLevel = Aws::Utils::Logging::LogLevel::Off;
	
	FString DebuggingLevel;
	
	if (GConfig->GetString(TEXT("Core.Log"), TEXT("LogAws"), DebuggingLevel, GEngineIni))
	{
		LoggingLevel = Aws::Utils::Logging::LogLevel::Debug;
	}

	Aws::SDKOptions* SdkOptions = static_cast<Aws::SDKOptions*>(m_sdkOptions);
	SdkOptions->loggingOptions.logLevel = LoggingLevel;
#if !WITH_EDITOR && !PLATFORM_ANDROID && !PLATFORM_IOS
	SdkOptions->loggingOptions.logger_create_fn = [LoggingLevel] { return std::make_shared<FS3ConsoleLogger>(LoggingLevel); };
#endif
	SdkOptions->memoryManagementOptions.memoryManager = &m_memoryManager;
	Aws::InitAPI(*SdkOptions);
#endif
}

void FAwsS3Module::ShutdownModule()
{
#if !DISABLE_S3
	LogVerbose("");
#endif
}

#undef LOCTEXT_NAMESPACE
