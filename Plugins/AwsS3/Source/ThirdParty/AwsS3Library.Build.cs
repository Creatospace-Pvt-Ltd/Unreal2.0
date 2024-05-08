/**
* Copyright (C) 2017-2023 eelDev AB
*/

using System;
using System.Collections.Generic;
using System.IO;
using UnrealBuildTool;

public sealed class AwsS3Library : ModuleRules
{
	private readonly List<string> LibraryNames = new List<string>()
	{
		"aws-c-auth",
		"aws-c-cal",
		"aws-c-common",
		"aws-c-compression",
		"aws-c-event-stream",
		"aws-checksums",
		"aws-c-http",
		"aws-c-io",
		"aws-c-mqtt",
		"aws-cpp-sdk-kms",
		"aws-cpp-sdk-s3",
		"aws-cpp-sdk-s3control",
		"aws-cpp-sdk-s3-encryption",
		"aws-cpp-sdk-core",
		"aws-crt-cpp",
		"aws-c-s3",
		"aws-c-sdkutils",
	};

	public string SdkBaseDir
	{
		get { return ModuleDirectory + "/" + Name; }
	}

	public string SdkIncludesDir
	{
		get { return Path.Combine(SdkBaseDir, "src"); }
	}

	public string SdkLibsDir
	{
		get
		{
			if (Target.Platform == UnrealTargetPlatform.IOS)
			{
				return Path.Combine(SdkBaseDir, "Lib", "IOS");
			}
			else if (Target.Platform == UnrealTargetPlatform.Android)
			{
				return Path.Combine(SdkBaseDir, "Lib", "Android");
			}
			else if (Target.Platform == UnrealTargetPlatform.Linux)
			{
				return Path.Combine(SdkBaseDir, "Lib", "Linux");
			}
			else if (Target.Platform == UnrealTargetPlatform.Mac)
			{
				return Path.Combine(SdkBaseDir, "Lib", "Mac");
			}
			else
			{
				return Path.Combine(SdkBaseDir, "Lib", "Win64");
			}
		}
	}

	public AwsS3Library(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		bool SupportedPlatform = Target.Platform == UnrealTargetPlatform.Android || Target.Platform == UnrealTargetPlatform.Linux || Target.Platform == UnrealTargetPlatform.Win64 || Target.Platform == UnrealTargetPlatform.Mac || Target.Platform == UnrealTargetPlatform.IOS;
		
		if (SupportedPlatform)
		{
			PublicDefinitions.Add("DISABLE_S3=0");
			
			string EngineVersion = (Target.Version.MajorVersion == 5 ? "5" : "4") + Target.Version.MinorVersion.ToString();

			PublicDefinitions.Add("USE_IMPORT_EXPORT");
			PublicDefinitions.Add("AWS_CRT_CPP_USE_IMPORT_EXPORT");
			PublicDefinitions.Add("AWS_DEEP_CHECKS=0");

			bEnableUndefinedIdentifierWarnings = false;
			
			PublicIncludePaths.Add(SdkIncludesDir);

			LibraryNames.ForEach(AddLibrary);

			if (Target.Platform == UnrealTargetPlatform.Android)
			{
				PublicAdditionalLibraries.Add(Path.Combine(SdkLibsDir, EngineVersion, "libs2n.a"));
				PublicIncludePathModuleNames.AddRange(new string[] { "Launch" });
				AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(ModuleDirectory, "LibAwsS3_APL.xml"));
			}

			if (Target.Platform == UnrealTargetPlatform.Linux)
			{
				RuntimeDependencies.Add(Path.Combine(SdkLibsDir, "libcrypto.so.1.1"));
				PublicAdditionalLibraries.Add(Path.Combine(SdkLibsDir, "libs2n.a"));
			}

			if (Target.Platform == UnrealTargetPlatform.Win64)
			{
				PublicDefinitions.Add("USE_WINDOWS_DLL_SEMANTICS");
			}

			if (Target.Platform == UnrealTargetPlatform.IOS)
			{
				PublicDependencyModuleNames.AddRange(new string[]
				{
					"ApplicationCore",
				});

				PublicFrameworks.Add("Foundation");
				PublicFrameworks.Add("Security");

				PrivateDefinitions.Add("FORCE_ANSI_ALLOCATOR=1");

				PublicAdditionalLibraries.Add(Path.Combine(SdkLibsDir, "libcurl.a"));

				PublicIncludePaths.AddRange(new string[] { "Runtime/ApplicationCore/Public/IOS" });
				PublicIncludePaths.AddRange(new string[] { "Runtime/ApplicationCore/Private/Apple" });
			}
		}
		else
		{
			PublicDefinitions.Add("DISABLE_S3=1");
		}
	}

	private void AddLibrary(string LibraryName) 
	{
		string EngineVersion = (Target.Version.MajorVersion == 5 ? "5" : "4") + Target.Version.MinorVersion.ToString();
		string Platform = Target.Platform.ToString();

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicAdditionalLibraries.Add(Path.Combine(SdkLibsDir, LibraryName + ".lib"));
			RuntimeDependencies.Add(Path.Combine(SdkLibsDir, LibraryName + ".dll"));
			PublicDelayLoadDLLs.Add(LibraryName + ".dll");
		}
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			string BaseFile = Path.Combine(SdkLibsDir, "lib" + LibraryName);
			
			string[] DependencyLibs =
			{
				".1.dylib",
				".1.0.0.dylib",
				".0unstable.dylib"
			};
			
			foreach (string Dependency in DependencyLibs)
			{
				if (System.IO.File.Exists(BaseFile + Dependency))
				{
					Console.WriteLine("Adding additional Mac Dylib: lib" + LibraryName + Dependency);
					PublicDelayLoadDLLs.Add(Path.Combine(SdkLibsDir, "lib" + LibraryName + Dependency));
					RuntimeDependencies.Add(Path.Combine(SdkLibsDir, "lib" + LibraryName + Dependency));
				}
			}

			Console.WriteLine("Adding Mac Dylib: lib" + LibraryName + ".dylib");
			PublicAdditionalLibraries.Add(Path.Combine(SdkLibsDir, "lib" + LibraryName + ".dylib"));
			PublicDelayLoadDLLs.Add(Path.Combine(SdkLibsDir, "lib" + LibraryName + ".dylib"));
			RuntimeDependencies.Add(Path.Combine(SdkLibsDir, "lib" + LibraryName + ".dylib"));
		}
		else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			string BaseFile = Path.Combine(SdkLibsDir, "lib" + LibraryName + ".so");
			string StaticLib = Path.Combine(SdkLibsDir, "lib" + LibraryName + ".a");

			if (System.IO.File.Exists(StaticLib))
			{
				Console.WriteLine("AwsS3Library: Adding linux static lib: " + StaticLib);
				PublicAdditionalLibraries.Add(StaticLib);
			}
			else
			{
				Console.WriteLine("AwsS3Library: Adding linux dynamic lib: " + BaseFile);
				PublicAdditionalLibraries.Add(Path.Combine(SdkLibsDir, "lib" + LibraryName + ".so"));
				RuntimeDependencies.Add(Path.Combine(SdkLibsDir, "lib" + LibraryName + ".so"));
			}
		}
		else if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			PublicAdditionalLibraries.Add(Path.Combine(SdkLibsDir, EngineVersion, "lib" + LibraryName + ".a"));
		}
		else if (Target.Platform == UnrealTargetPlatform.Android)
		{
			PublicAdditionalLibraries.Add(Path.Combine(SdkLibsDir, EngineVersion, "lib" + LibraryName + ".a"));
		}
		else
		{
			throw new PlatformNotSupportedException("Platform " + Platform + " is not supported by the AwsS3 Module.");
		}
	}
}
