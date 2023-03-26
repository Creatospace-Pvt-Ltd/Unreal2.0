// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class CreatospaceServerTarget : TargetRules
{
	public CreatospaceServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
      	bUseLoggingInShipping = true; // If you want logging in shipping build
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "Creatospace" } );
	}
}
