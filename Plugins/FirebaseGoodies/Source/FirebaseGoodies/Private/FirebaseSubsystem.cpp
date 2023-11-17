// Copyright (c) 2022 Nineva Studios

#include "FirebaseSubsystem.h"

#include "FirebaseGoodies.h"
#include "Utils/FirebaseGoodiesPluginUtils.h"
#include "FirebaseGoodiesSettings.h"
#include "Database/FGDatabaseLibrary.h"
#include "FirebaseGoodiesLog.h"
#include "FirebaseGoodiesSettings.h"
#include "CloudMessaging/FGCloudMessaging.h"
#include "CloudMessaging/FGRemoteMessage.h"
#include "Utils/FirebaseGoodiesPluginUtils.h"
#include "Async/Async.h"
#include "Misc/Paths.h"

#if (PLATFORM_WINDOWS || PLATFORM_MAC) && FG_ENABLE_EDITOR_SUPPORT
#include "firebase/app.h"
#include "firebase/analytics.h"
#include "firebase/messaging.h"

#include "FirebaseGoodiesLog.h"

#include "Desktop/Messaging/FGMessagingListener.h"
#endif

#if PLATFORM_IOS
#include "IOSAppDelegate.h"
#include "IOS/CloudMessaging/FGCloudMessagingDelegate.h"
#include "Misc/CoreDelegates.h"
#include "IOS/DynamicLinksHelper.h"

#import <FirebaseCore/FirebaseCore.h>
#import <FirebaseMessaging/FirebaseMessaging.h>
#import <GoogleSignIn/GoogleSignIn.h>

FGCloudMessagingDelegate* MessagingDelegate;
#endif

#if PLATFORM_IOS && DYNAMIC_LINKS_IOS
#import "FirebaseDynamicLinks/FirebaseDynamicLinks.h"
#endif

void UFirebaseSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	LogInit();

	const UFirebaseGoodiesSettings* Settings = FFirebaseGoodiesModule::Get().GetSettings();

#if WITH_EDITOR
	Settings->HandleAPNState();
#endif

	InitDesktop(Settings);
	InitIos(Settings);

#if PLATFORM_IOS && DYNAMIC_LINKS_IOS
	FIOSCoreDelegates::OnOpenURL.AddLambda([](UIApplication* application, NSURL* url, NSString* sourceApplication, id annotation)
	{
		NSLog(@"[FirebaseGoodies] OnOpenURL: %@", [url absoluteString]);
	
		DynamicLinksHelper::WasAppOpenViaUrl = true;
		DynamicLinksHelper::URL = url;
		
		FIRDynamicLink* link = [[FIRDynamicLinks dynamicLinks] dynamicLinkFromCustomSchemeURL:url];
		if (link != nil)
		{
			DynamicLinksHelper::DynamicLink = (FIRDynamicLink*)CFBridgingRetain(link);
		}
	});
#endif

#if !PLATFORM_IOS
	InitDbOptions(Settings);
#endif
}

void UFirebaseSubsystem::InitDesktop(const UFirebaseGoodiesSettings* Settings) const
{
#if (PLATFORM_WINDOWS || PLATFORM_MAC) && FG_ENABLE_EDITOR_SUPPORT
	firebase::AppOptions Options;
	Options.set_api_key(TCHAR_TO_ANSI(Settings->bUseGoogleServicesJsonForDesktop ? *Settings->AndroidApiKey : *Settings->ApiKey));
	Options.set_app_id(TCHAR_TO_ANSI(Settings->bUseGoogleServicesJsonForDesktop ? *Settings->AndroidGoogleAppId : *Settings->GoogleAppId));
	Options.set_client_id(TCHAR_TO_ANSI(Settings->bUseGoogleServicesJsonForDesktop ? *Settings->AndroidClientId : *Settings->ClientId));
	Options.set_database_url(TCHAR_TO_ANSI(Settings->bUseGoogleServicesJsonForDesktop ? *Settings->AndroidDatabaseUrl : *Settings->DatabaseUrl));
	Options.set_package_name(TCHAR_TO_ANSI(Settings->bUseGoogleServicesJsonForDesktop ? *Settings->AndroidBundleId : *Settings->BundleId));
	Options.set_project_id(TCHAR_TO_ANSI(Settings->bUseGoogleServicesJsonForDesktop ? *Settings->AndroidProjectId : *Settings->ProjectId));
	Options.set_storage_bucket(TCHAR_TO_ANSI(Settings->bUseGoogleServicesJsonForDesktop ? *Settings->AndroidStorageBucket : *Settings->StorageBucket));

	const firebase::App* App = firebase::App::Create(Options);

	if (App)
	{
		firebase::analytics::Initialize(*App);
		FGMessagingListener* Listener = new FGMessagingListener();
		firebase::messaging::Initialize(*App, Listener);
	}
	else
	{
		UE_LOG(LogFirebaseGoodies, Error, TEXT("Failed to initialize Firebase"));
	}
#endif
}

void UFirebaseSubsystem::InitIos(const UFirebaseGoodiesSettings* FirebaseGoodiesSettings)
{
#if PLATFORM_IOS
	__block FIROptions* options = [[FIROptions alloc] initWithGoogleAppID:FirebaseGoodiesSettings->GoogleAppId.GetNSString() GCMSenderID:FirebaseGoodiesSettings->GcmSenderId.GetNSString()];
	options.APIKey = FirebaseGoodiesSettings->ApiKey.GetNSString();
	options.bundleID = FirebaseGoodiesSettings->BundleId.GetNSString();
	options.clientID = FirebaseGoodiesSettings->ClientId.GetNSString();
	options.projectID = FirebaseGoodiesSettings->ProjectId.GetNSString();
	options.androidClientID = FirebaseGoodiesSettings->AndroidClientId.GetNSString();
	options.storageBucket = FirebaseGoodiesSettings->StorageBucket.GetNSString();
	
	NSString* DatabaseUrl = FirebaseGoodiesSettings->DatabaseUrl.GetNSString();
	if ([DatabaseUrl length] != 0)
	{
		options.databaseURL = DatabaseUrl;
	}

	dispatch_async(dispatch_get_main_queue(), ^{
		[FIRApp configureWithOptions:options];
		InitDbOptions(FirebaseGoodiesSettings); // otherwise causes crash because of race condition with init

		if (DynamicLinksHelper::WasAppOpenViaAppLink) {
			UE_LOG(LogFirebaseGoodies, Verbose, TEXT("Application was open from Universal link."));
			[[FIRDynamicLinks dynamicLinks] handleUniversalLink:DynamicLinksHelper::URL completion:^(FIRDynamicLink* _Nullable dynamicLink, NSError* _Nullable error) {
				NSLog(@"handleUniversalLink:completion:");
				if (dynamicLink != nil) {
					NSLog(@"Dynamic Link URL: %@", [dynamicLink.url absoluteString]);
					DynamicLinksHelper::DynamicLink = (FIRDynamicLink*)CFBridgingRetain(dynamicLink);
				}

				if (error != nil) {
					NSLog(@"Dynamic Link parse error: %@", error.localizedDescription);
				}
			}];
		}
		
		if (DynamicLinksHelper::WasAppOpenViaUrl) {
			UE_LOG(LogFirebaseGoodies, Verbose, TEXT("Application was open from URL."));
			FIRDynamicLink* link = [[FIRDynamicLinks dynamicLinks] dynamicLinkFromCustomSchemeURL:DynamicLinksHelper::URL];
			if (link != nil)
				DynamicLinksHelper::DynamicLink = (FIRDynamicLink*)CFBridgingRetain(link);
		}
	});

	FIOSCoreDelegates::OnOpenURL.AddStatic([](UIApplication* application, NSURL* url, NSString* sourceApplication, id annotation) {
		[[GIDSignIn sharedInstance] handleURL:url];
	});

	if (FirebaseGoodiesSettings->bEnableAPNForIOS)
	{
		dispatch_async(dispatch_get_main_queue(), ^{
			MessagingDelegate = [FGCloudMessagingDelegate new];

			MessagingDelegate.onTokenReceived = ^(NSString* token) {
				FString Token = FString(token);

				AsyncTask(ENamedThreads::GameThread, [=]() {
					UFGCloudMessaging::OnNewTokenCallback.ExecuteIfBound(Token);
				});
			};

			[FIRMessaging messaging].delegate = MessagingDelegate;

			//Register for notifications
			UNUserNotificationCenter* notificationCenter = [UNUserNotificationCenter currentNotificationCenter];

			[notificationCenter getNotificationSettingsWithCompletionHandler:^(UNNotificationSettings* settings) {
				if (settings.authorizationStatus == UNAuthorizationStatusDenied ||
					settings.authorizationStatus == UNAuthorizationStatusNotDetermined)
				{
					UNAuthorizationOptions authOptions = UNAuthorizationOptionAlert | UNAuthorizationOptionSound | UNAuthorizationOptionBadge;
					[notificationCenter requestAuthorizationWithOptions:authOptions
													  completionHandler:^(BOOL granted, NSError* _Nullable error) {
														  // ...
													  }];
				}
			}];

			[[UIApplication sharedApplication] registerForRemoteNotifications];
		});

		FCoreDelegates::ApplicationReceivedRemoteNotificationDelegate.AddStatic([](FString json, int state) {
			UFGCloudMessaging::OnRemoteNotificationReceivedCallback.ExecuteIfBound(json, state);
		});
	}
#endif
}

bool UFirebaseSubsystem::IsAppValid() const
{
#if (PLATFORM_WINDOWS || PLATFORM_MAC) && FG_ENABLE_EDITOR_SUPPORT
	return firebase::App::GetInstance() != nullptr;
#else
	return true;
#endif
}

void UFirebaseSubsystem::LogInit()
{
	const FString PluginName = "FirebaseGoodies";
	const FString PluginVersionName = UFirebaseGoodiesPluginUtils::GetPluginVersionName(PluginName);
	UE_LOG(LogFirebaseGoodies, Verbose, TEXT("[%s] - Version: %s"), *PluginName, *PluginVersionName);
}

void UFirebaseSubsystem::InitDbOptions(const UFirebaseGoodiesSettings* Settings)
{
	UFGDatabaseLibrary::SetPersistenceEnabled(Settings->bIsPersistenceEnabled);
	UFGDatabaseLibrary::SetPersistenceCacheSizeBytes(Settings->DatabaseCacheSize * 1024);
}

