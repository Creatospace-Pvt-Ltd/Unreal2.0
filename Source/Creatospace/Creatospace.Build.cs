// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Creatospace : ModuleRules
{
	public Creatospace(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "HTTP", "Json", "OnlineSubsystemEOS", "OnlineSubsystem", "OnlineSubsystemUtils", "GameLiftServerSDK", "VoiceChat", "EOSVoiceChat", "EOSSDK", "EOSShared", "SocketSubsystemEOS" });

		PrivateDependencyModuleNames.AddRange(new string[] { "ReadyPlayerMe", "SocketSubsystemEOS", "OnlineSubsystemEOS", "EOSSDK", "OnlineSubsystem", "OnlineSubsystemUtils", "EOSVoiceChat", "VoiceChat", "EOSShared" });

		// Dependencies Web Browser
		PrivateDependencyModuleNames.AddRange(new string[] { "WebBrowserWidget", "WebBrowser", "UMG" });

		// DynamicallyLoadedModuleNames.Add("OnlineSubsystemEOS");

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
