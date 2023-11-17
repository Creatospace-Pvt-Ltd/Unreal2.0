// Copyright (c) 2022 Nineva Studios

#include "FirebaseGoodies.h"

#include "Developer/Settings/Public/ISettingsModule.h"
#include "FirebaseGoodiesLog.h"
#include "FirebaseGoodiesSettings.h"
#include "Utils/FirebaseGoodiesPluginUtils.h"
#include "Misc/Paths.h"
#include "UObject/Package.h"

#define LOCTEXT_NAMESPACE "FFirebaseGoodiesModule"

void FFirebaseGoodiesModule::StartupModule()
{
	FirebaseGoodiesSettings = NewObject<UFirebaseGoodiesSettings>(GetTransientPackage(), "FirebaseGoodiesSettings", RF_Standalone);
	FirebaseGoodiesSettings->AddToRoot();

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "FirebaseGoodies",
			LOCTEXT("RuntimeSettingsName", "Firebase Goodies"),
			LOCTEXT("RuntimeSettingsDescription", "Configure Firebase Goodies"),
			FirebaseGoodiesSettings);
	}
}

void FFirebaseGoodiesModule::ShutdownModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "FirebaseGoodies");
	}

	if (!GExitPurge)
	{
		// If we're in exit purge, this object has already been destroyed
		FirebaseGoodiesSettings->RemoveFromRoot();
	}
	else
	{
		FirebaseGoodiesSettings = nullptr;
	}
}

UFirebaseGoodiesSettings* FFirebaseGoodiesModule::GetSettings() const
{
	return FirebaseGoodiesSettings;
}

FString FFirebaseGoodiesModule::GetPluginDirAbsPath() const
{
	const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("FirebaseGoodies"));

	if(!Plugin)
	{
		return FString();
	}

	return FPaths::ConvertRelativePathToFull(Plugin->GetBaseDir());
}

#undef LOCTEXT_NAMESPACE

DEFINE_LOG_CATEGORY(LogFirebaseGoodies);

IMPLEMENT_MODULE(FFirebaseGoodiesModule, FirebaseGoodies)
