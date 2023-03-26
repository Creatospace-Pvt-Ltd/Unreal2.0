// 2022 Copyright Pandores Marketplace

using UnrealBuildTool;
using System.IO;
using System;

public class MongoDBDriver : ModuleRules
{
	/**
	 * Set it to true if you want to link against arm64 libraries
	 * for MacOS.
	 **/
	static bool bLinkArm64ForDarwin = false;

	/**
	 * Set it to true if you want to link against arm64 libraries
	 * for Android.
	 **/
	static bool bLinkArm64ForAndroid = true;
	/**
	 * Module rules target constructor.
	 **/
	public MongoDBDriver(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		// Enable exceptions as MongoCXX relies on it.
		bEnableExceptions = true;
		// bUseRTTI = true;
		// Engine dependencies
		PublicDependencyModuleNames.AddRange(new string[] { "Core" });
		PrivateDependencyModuleNames.AddRange(new string[] { "CoreUObject", "Engine", "zlib", "OpenSSL" });

		// For Project config panel.
		if (Target.Type == TargetType.Editor)
		{
			PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "UnrealEd", "PropertyEditor", "Settings" });
		}

		// Darwin and iOS system dependencies.
		if (Target.Platform == UnrealTargetPlatform.Mac || Target.Platform == UnrealTargetPlatform.IOS)
        {
			PublicFrameworks.AddRange(new string[]
			{
				"CoreFoundation",
				"Foundation",
				"Security",
				"GSS",
				"SystemConfiguration",
				"Kerberos"
			});

			PublicSystemLibraries.Add("resolv");
		}

		// Path to Source/ThirdParty.
		string ThirdPartyPath = Path.Combine(PluginDirectory, "Source/ThirdParty/");
		string MongocxxPath   = Path.Combine(ThirdPartyPath,  "mongocxx/");
		string BoostPath      = Path.Combine(ThirdPartyPath,  "boost/");

		//////////////////////////////////////////////////////////////////////////////
		//																			//
		//								WINDOWS										//
		//																			//
		//////////////////////////////////////////////////////////////////////////////
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// Libraries
			AddLibrary(MongocxxPath + "windows-x64/libs/libssl.lib");
			AddLibrary(MongocxxPath + "windows-x64/libs/bson-1.0.lib");
			AddLibrary(MongocxxPath + "windows-x64/libs/mongoc-1.0.lib");
			AddLibrary(MongocxxPath + "windows-x64/libs/bsoncxx.lib");
			AddLibrary(MongocxxPath + "windows-x64/libs/mongocxx.lib");

			// Dlls - Not needed anymore as mongocxx is statically linked.
			//CopyDll(MongocxxPath + "windows-x64/bin/bson-1.0.dll");
			//CopyDll(MongocxxPath + "windows-x64/bin/mongoc-1.0.dll");
			//CopyDll(MongocxxPath + "windows-x64/bin/bsoncxx.dll");
			//CopyDll(MongocxxPath + "windows-x64/bin/mongocxx.dll");
			//CopyDll(MongocxxPath + "windows-x64/bin/zlib1.dll");

			// Includes
			AddPrivateInclude(MongocxxPath + "windows-x64/include/");
		}

		//////////////////////////////////////////////////////////////////////////////
		//																			//
		//								ANDROID										//
		//																			//
		//////////////////////////////////////////////////////////////////////////////
		else if (Target.Platform == UnrealTargetPlatform.Android)
		{
            AddPrivateInclude(Path.Combine(ModuleDirectory, "Private/Android/"));
            AddPrivateInclude(Path.Combine(ModuleDirectory, "Private/Android/mongocxx"));
            AddPrivateInclude(Path.Combine(ModuleDirectory, "Private/Android/bsoncxx"));
            AddPrivateInclude(Path.Combine(ModuleDirectory, "Private/Android/common"));

			PublicDefinitions.Add("BSON_COMPILATION=1");
			PublicDefinitions.Add("MONGOC_COMPILATION=1");
			PublicDefinitions.Add("MONGOC_COMPILATION=1");
            PublicSystemLibraries.Add("c");

            if (false)
			{
				bool bLinkStaticLibraries = false;

				if (bLinkStaticLibraries)
				{
					AddLibrary(MongocxxPath + "android-arm64-static/lib/libbsoncxx-static.a");
					AddLibrary(MongocxxPath + "android-arm64-static/lib/libbson-static-1.0.a");
					AddLibrary(MongocxxPath + "android-arm64-static/lib/libcrypto.3.so");
					AddLibrary(MongocxxPath + "android-arm64-static/lib/libmongoc-static-1.0.a");
					AddLibrary(MongocxxPath + "android-arm64-static/lib/libmongocxx-static.a");
					AddLibrary(MongocxxPath + "android-arm64-static/lib/libssl.3.so");
					AddLibrary(MongocxxPath + "android-arm64-static/lib/libz.a");

					// Includes
					AddPrivateInclude(MongocxxPath + "android-arm64-static/include/");
				}

				else if (bLinkArm64ForAndroid)
				{
					// Libraries for arm64
					AddLibrary(MongocxxPath + "android-arm64/libbson/lib/libbson-1.0.so");
					AddLibrary(MongocxxPath + "android-arm64/zlib/lib/libz.so");
					AddLibrary(MongocxxPath + "android-arm64/mongo-c-driver/lib/libmongoc-1.0.so");
					AddLibrary(MongocxxPath + "android-arm64/mongo-cxx-driver/lib/libbsoncxx.so");
					AddLibrary(MongocxxPath + "android-arm64/mongo-cxx-driver/lib/libmongocxx.so");

					// Includes
					AddPrivateInclude(MongocxxPath + "android-arm64/mongo-cxx-driver/include/");
				}

				else
				{
					// Libraries for armv7
					AddLibrary(MongocxxPath + "android-armv7/libbson/lib/libbson-1.0.so");
					AddLibrary(MongocxxPath + "android-armv7/mongo-c-driver/lib/libmongoc-1.0.so");
					AddLibrary(MongocxxPath + "android-armv7/mongo-cxx-driver/lib/libbsoncxx.so");
					AddLibrary(MongocxxPath + "android-armv7/mongo-cxx-driver/lib/libmongocxx.so");
				}
			}


            // UPL File for copying the libraries inside the APK.
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(ModuleDirectory, "UPL/MongoDB.Android.UPL.xml"));
        }

		//////////////////////////////////////////////////////////////////////////////
		//																			//
		//									IOS										//
		//																			//
		//////////////////////////////////////////////////////////////////////////////
		else if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			// Libraries
			AddLibrary(MongocxxPath + "ios-arm64/libs/libbson-1.0.a");
			AddLibrary(MongocxxPath + "ios-arm64/libs/libmongoc-1.0.a");
			AddLibrary(MongocxxPath + "ios-arm64/libs/libbsoncxx.a");
			AddLibrary(MongocxxPath + "ios-arm64/libs/libmongocxx.a");

			// Includes
			AddPrivateInclude(MongocxxPath + "ios-arm64/include/");
		}

		//////////////////////////////////////////////////////////////////////////////
		//																			//
		//									MACOS									//
		//																			//
		//////////////////////////////////////////////////////////////////////////////
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			if (bLinkArm64ForDarwin)
			{
				// Libraries 
				AddLibrary(MongocxxPath + "darwin-arm64/libs/libbson-1.0.a");
				AddLibrary(MongocxxPath + "darwin-arm64/libs/libmongoc-1.0.a");
				AddLibrary(MongocxxPath + "darwin-arm64/libs/libbsoncxx.a");
				AddLibrary(MongocxxPath + "darwin-arm64/libs/libmongocxx.a");

				// Includes
				AddPrivateInclude(MongocxxPath + "darwin-arm64/include/");
			}

            else
			{
				// Libraries 
				AddLibrary(MongocxxPath + "darwin-x64/libs/libsasl2.a");
				AddLibrary(MongocxxPath + "darwin-x64/libs/libbson-1.0.a");
				AddLibrary(MongocxxPath + "darwin-x64/libs/libmongoc-1.0.a");
				AddLibrary(MongocxxPath + "darwin-x64/libs/libbsoncxx.a");
				AddLibrary(MongocxxPath + "darwin-x64/libs/libmongocxx.a");

				// Includes
				AddPrivateInclude(MongocxxPath + "darwin-x64/include/");
			}
		}

		//////////////////////////////////////////////////////////////////////////////
		//																			//
		//								LINUX										//
		//																			//
		//////////////////////////////////////////////////////////////////////////////
		else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			// Libraries 
			AddLibrary(MongocxxPath + "linux-x64/libs/libmongoc-1.0.so"); 
			AddLibrary(MongocxxPath + "linux-x64/libs/libbson-1.0.a");
			AddLibrary(MongocxxPath + "linux-x64/libs/libbsoncxx.a");
			AddLibrary(MongocxxPath + "linux-x64/libs/libmongocxx.a");

			// Includes
			AddPrivateInclude(MongocxxPath + "linux-x64/include/");
		}

		//////////////////////////////////////////////////////////////////////////////
		//																			//
		//						UNSUPPORTED PLATFORM								//
		//																			//
		//////////////////////////////////////////////////////////////////////////////
		else
		{
			// This platform is not supported.
			// Note that you can add platform by compiling mongocxx for this platform.
			System.Console.Error.WriteLine("Unsupported platform `" + Target.Platform + "`.");
	    }

		// Boost libraries
		AddPrivateInclude(BoostPath + "common/boost-assert/include");
		AddPrivateInclude(BoostPath + "common/boost-config/include");
		AddPrivateInclude(BoostPath + "common/boost-container-hash/include");
		AddPrivateInclude(BoostPath + "common/boost-core/include");
		AddPrivateInclude(BoostPath + "common/boost-io/include");
		AddPrivateInclude(BoostPath + "common/boost-move/include");
		AddPrivateInclude(BoostPath + "common/boost-optional/include");
		AddPrivateInclude(BoostPath + "common/boost-preprocessor/include");
		AddPrivateInclude(BoostPath + "common/boost-static-assert/include");
		AddPrivateInclude(BoostPath + "common/boost-throw-exception/include");
		AddPrivateInclude(BoostPath + "common/boost-type-traits/include");
		AddPrivateInclude(BoostPath + "common/boost-utility/include");
		AddPrivateInclude(BoostPath + "common/boost-variant/include");
		AddPrivateInclude(BoostPath + "common/boost-type-index/include");
		AddPrivateInclude(BoostPath + "common/boost-mpl/include");
		AddPrivateInclude(BoostPath + "common/boost-detail/include");
		AddPrivateInclude(BoostPath + "common/boost-integer/include");

		// Plugin files
		PublicIncludePaths .Add(Path.Combine(ModuleDirectory, "Public"));
		PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));
	}

	/**
	 * Adds a library.
	 **/
	void AddLibrary(string LibraryPath)
	{
		PublicAdditionalLibraries.Add(LibraryPath);
		//if (LibraryPath.EndsWith(".so"))
        //{
		//	CopyDll(LibraryPath);
        //}
	}

	/**
	 * Copy a DLL file to the binaries folder.
	 **/
	void CopyDll(string DllPath)
    {
		RuntimeDependencies.Add("$(BinaryOutputDir)/" + Path.GetFileName(DllPath), DllPath);
	}

	/**
	 * Adds a folder to the include list.
	 **/
	void AddPrivateInclude(string IncludePath)
    {
		PrivateIncludePaths.Add(IncludePath);
    }

}
