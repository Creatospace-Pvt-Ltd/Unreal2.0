/**
* Copyright (C) 2017-2023 eelDev AB
*/

#include "AwsS3LoaderModule.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "HAL/PlatformProcess.h"
#include "HAL/FileManager.h"
#include <Modules/ModuleInterface.h>
#include <Modules/ModuleManager.h>
#include <CoreGlobals.h>

IMPLEMENT_MODULE(FAwsS3LoaderModule, AwsS3Loader);
#define LOCTEXT_NAMESPACE "FAwsS3LoaderModule"

void FAwsS3LoaderModule::StartupModule()
{
#if !DISABLE_S3
	UE_LOG(LogTemp, Log, TEXT("Loading AwsS3LoaderModule"));

	const TSharedPtr<IPlugin> PluginPtr = IPluginManager::Get().FindPlugin("AwsS3");

#if PLATFORM_WINDOWS
	if (PluginPtr)
	{
		const FString PluginDir = PluginPtr->GetBaseDir();
		const FString BinDir =  FPaths::ConvertRelativePathToFull(FPaths::Combine(PluginDir, TEXT("Source"), TEXT("ThirdParty"), TEXT("AwsS3Library"), TEXT("Lib"), TEXT("Win64")));
		FPlatformProcess::PushDllDirectory(*BinDir);
		TArray<FString> LibFiles;
		IFileManager::Get().FindFiles(LibFiles, *(BinDir / TEXT("*.dll")), true, false);

		UE_LOG(LogTemp, Log, TEXT("S3LoaderModule: Found (%i) DLLs in %s"), LibFiles.Num(), *BinDir);

		for (auto Element : LibFiles)
		{
			const FString FinalPath = FPaths::Combine(BinDir, Element);
			UE_LOG(LogTemp, Log, TEXT("S3LoaderModule Loading DLL: %s"), *FinalPath);
			m_LoadedLibs.Add(FPlatformProcess::GetDllHandle(*FinalPath));
		}
		FPlatformProcess::PopDllDirectory(*BinDir);
	}
#endif

#if PLATFORM_MAC
	if (PluginPtr)
	{
		const FString PluginDir = PluginPtr->GetBaseDir();
		const FString BinDir =  FPaths::ConvertRelativePathToFull(FPaths::Combine(PluginDir, TEXT("Source"), TEXT("ThirdParty"), TEXT("AwsS3Library"), TEXT("Lib"), TEXT("Mac")));
		FPlatformProcess::PushDllDirectory(*BinDir);
		TArray<FString> LibFiles;
		IFileManager::Get().FindFiles(LibFiles, *(BinDir / TEXT("*.dylib")), true, false);

		UE_LOG(LogTemp, Log, TEXT("S3LoaderModule: Found (%i) Dylibs in %s"), LibFiles.Num(), *BinDir);

		for (auto Element : LibFiles)
		{
			const FString FinalPath = FPaths::Combine(BinDir, Element);
			UE_LOG(LogTemp, Log, TEXT("S3LoaderModule Loading DLL: %s"), *FinalPath);
			m_LoadedLibs.Add(FPlatformProcess::GetDllHandle(*FinalPath));
		}
		FPlatformProcess::PopDllDirectory(*BinDir);
	}
#endif
#endif
}

void FAwsS3LoaderModule::ShutdownModule()
{
#if !DISABLE_S3
#if PLATFORM_WINDOWS || PLATFORM_MAC
	for (const auto Element : m_LoadedLibs)
	{
		FPlatformProcess::FreeDllHandle(Element);
	}
	m_LoadedLibs.Empty();
#endif
#endif
}

#undef LOCTEXT_NAMESPACE
