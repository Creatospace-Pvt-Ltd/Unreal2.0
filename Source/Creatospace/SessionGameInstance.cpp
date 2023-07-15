// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionGameInstance.h"
#include <Kismet/GameplayStatics.h>
#include <OnlineSessionClient.h>
//#include <EOSSDK/SDK/Include/eos_sessions_types.h>
#include <GameFramework/GameModeBase.h>
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSubsystem.h"
//#include <OnlineSubsystemEOS.h>
//#include <EOSSDK/SDK/Include/eos_integratedplatform.h>
//#include <EOSSDK/SDK/Include/eos_integratedplatform_types.h>
//#include <EOSSDK/SDK/Include/eos_common.h>
//#include <IEOSSDKManager.h>
//#include <UserManagerEOS.h>
//#include <OnlineSubsystemEOS.h>
//#include <eos_rtc.h>
//#include <Windows/eos_Windows.h>
//#include <eos_rtc_admin.h>


// In our UNWGameIntance.cpp:

USessionGameInstance::USessionGameInstance(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
	

	/** Bind function for CREATING a Session */
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &USessionGameInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &USessionGameInstance::OnStartOnlineGameComplete);

/** Bind function for FINDING a Session */
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &USessionGameInstance::OnFindSessionsComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &USessionGameInstance::OnJoinSessionComplete);

	/** Bind function for DESTROYING a Session */
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &USessionGameInstance::OnDestroySessionComplete);
	
		
	//OnLoginCallback = EOS_Connect_OnLoginCallback::CreateUObject(this, &USessionGameInstance::OnLoginCallback);
	

}

/*
typedef TEOSCallback<EOS_Connect_OnLoginCallback, EOS_Connect_LoginCallbackInfo> FConnectLoginCallback;
typedef TEOSCallback<EOS_RTC_OnJoinRoomCallback, EOS_RTC_JoinRoomCallbackInfo> FJoinRoomCallback;
typedef TEOSCallback<EOS_RTCAdmin_OnQueryJoinRoomTokenCompleteCallback, EOS_RTCAdmin_QueryJoinRoomTokenCompleteCallbackInfo> FCreateRoomTokenCallback;
typedef TEOSCallback<EOS_Connect_OnCreateUserCallback, EOS_Connect_CreateUserCallbackInfo> FCreateUserCallback;
typedef TEOSCallback<EOS_Connect_OnCreateDeviceIdCallback, EOS_Connect_CreateDeviceIdCallbackInfo> FCreateDeviceIDCallback;
*/

bool USessionGameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers, FString Mapname)
{
	// Get the Online Subsystem to work with
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// Get the Session Interface, so we can call the "CreateSession" function on it
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			/*
				Fill in all the Session Settings that we want to use.

				There are more with SessionSettings.Set(...);
				For example the Map or the GameMode/Type.
			*/
			SessionSettings = MakeShareable(new FOnlineSessionSettings());

			SessionSettings->bIsLANMatch = false;
			SessionSettings->bUsesPresence = bIsPresence;
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->NumPrivateConnections = 3;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bIsDedicated = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;
			SessionSettings->bUseLobbiesVoiceChatIfAvailable = true;
			SessionSettings->bUseLobbiesIfAvailable = true;
			MapName = Mapname;
			SessionSettings->Set(SETTING_MAPNAME, MapName, EOnlineDataAdvertisementType::ViaOnlineService);
			// Set the delegate to the Handle of the SessionInterface
			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnHostSession")));

			const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
			// Our delegate should get called when this is complete (doesn't need to be successful!)
			return Sessions->CreateSession(*localPlayer->GetPreferredUniqueNetId(), SessionName, *SessionSettings);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No OnlineSubsytem found!"));
	}

	return false;
}


void USessionGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnCreateSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	// Get the OnlineSubsystem so we can get the Session Interface
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the Session Interface to call the StartSession function
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Clear the SessionComplete delegate handle, since we finished this call
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				// Set the StartSession delegate handle
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

				// Our StartSessionComplete delegate should get called after this
				Sessions->StartSession(SessionName);
			}
			else {
				FindOnlineSessions(false, true);
			}
		}

	}
}


void USessionGameInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnStartSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Map: %s"), *MapName));
	// Get the Online Subsystem so we can get the Session Interface
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the Session Interface to clear the Delegate
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// Clear the delegate, since we are done with this call
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}

	// If the start was successful, we can open a NewMap if we want. Make sure to use "listen" as a parameter!
	if (bWasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Map: %s"), *MapName));
		UWorld* world = GetWorld();
		world->ServerTravel(MapName, true);
		//UGameplayStatics::OpenLevel(GetWorld(), FName(MapName), true, "listen");
	}
}


void USessionGameInstance::FindOnlineSessions(bool bIsLAN, bool bIsPresence)
{
	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Finding started")));

	if (OnlineSub)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Finding started 2")));
		// Get the SessionInterface from our OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			/*
				Fill in all the SearchSettings, like if we are searching for a LAN game and how many results we want to have!
			*/
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Finding started 3")));
			SessionSearch = MakeShareable(new FOnlineSessionSearch());

			SessionSearch->bIsLanQuery = bIsLAN;
			SessionSearch->MaxSearchResults = 20;
			SessionSearch->PingBucketSize = 500;

			// We only want to set this Query Setting if "bIsPresence" is true
			if (bIsPresence)
			{
				SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
			}

			TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();

			// Set the Delegate to the Delegate Handle of the FindSession function
			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

			const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
			// Finally call the SessionInterface function. The Delegate gets called once this is finished
			Sessions->FindSessions(*localPlayer->GetPreferredUniqueNetId(), SearchSettingsRef);
			
		}
	}
	else
	{
		// If something goes wrong, just call the Delegate Function directly with "false".
		OnFindSessionsComplete(false);
	}
}


/** Bind function for FINDING a Session */


void USessionGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnFindSessionsComplete bSuccess: %d"), bWasSuccessful));

	// Get OnlineSubsystem we want to work with
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get SessionInterface of the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// Clear the Delegate handle, since we finished this call
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

			// Just debugging the Number of Search results. Can be displayed in UMG or something later on
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Num Search Results: %d"), SessionSearch->SearchResults.Num()));
			FOnlineSessionSearchResult SearchResult;

			if (SessionSearch->SearchResults.Num() == 0)
			{
				//First get a reference to whichever game player you need, in this case the local one
				//const TSharedPtr<const FUniqueNetId> netID = UGameplayStatics::GetGameInstance(GetWorld())->GetFirstGamePlayer()->GetPreferredUniqueNetId().GetUniqueNetId();


				// Call our custom HostSession function. GameSessionName is a GameInstance variable
				//HostSession(netID, FName(RoomId), true, true, 4, MapName);

				RoomNotFound.Broadcast();
			}
			// If we have found at least 1 session, we just going to debug them. You could add them to a list of UMG Widgets, like it is done in the BP version!
			if (SessionSearch->SearchResults.Num() > 0)
			{
				
				// "SessionSearch->SearchResults" is an Array that contains all the information. You can access the Session in this and get a lot of information.
				// This can be customized later on with your own classes to add more information that can be set and displayed
				for (int32 SearchIdx = 0; SearchIdx < SessionSearch->SearchResults.Num(); SearchIdx++)
				{
					if (SearchIdx == 0)
					{
						GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("SessionFound")));
						SearchResult = SessionSearch->SearchResults[SearchIdx];
						//Output = true;

						//First get a reference to whichever game player you need, in this case the local one
						const TSharedPtr<const FUniqueNetId> netID = UGameplayStatics::GetGameInstance(GetWorld())->GetFirstGamePlayer()->GetPreferredUniqueNetId().GetUniqueNetId();

						JoinOnlineSession(netID, FName(RoomId), SearchResult);
						break;
					}
					// OwningUserName is just the SessionName for now. I guess you can create your own Host Settings class and GameSession Class and add a proper GameServer Name here.
					// This is something you can't do in Blueprint for example!
					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Session Number: %d | Sessionname: %s "), SearchIdx + 1, *(SessionSearch->SearchResults[SearchIdx].Session.OwningUserName)));
				}
			}
		}
	}
}

// In our UNWGameInstance.cpp:

bool USessionGameInstance::JoinOnlineSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult)
{
	// Return bool
	bool bSuccessful = false;
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Join Session Fired!")));

	// Get OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// Get SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			// Set the Handle again
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
			

			const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
			// Call the "JoinSession" Function with the passed "SearchResult". The "SessionSearch->SearchResults" can be used to get such a
			// "FOnlineSessionSearchResult" and pass it. Pretty straight forward!
			bSuccessful = Sessions->JoinSession(*localPlayer->GetPreferredUniqueNetId(), SessionName, SearchResult);

			if (!bSuccessful) {
				//First get a reference to whichever game player you need, in this case the local one
				//const TSharedPtr<const FUniqueNetId> netID = UGameplayStatics::GetGameInstance(GetWorld())->GetFirstGamePlayer()->GetPreferredUniqueNetId().GetUniqueNetId();


				// Call our custom HostSession function. GameSessionName is a GameInstance variable
				//HostSession(netID, FName(RoomId), true, true, 4, MapName);
				RoomNotFound.Broadcast();

			}
		}
	}

	return bSuccessful;
}

/** Bind function for JOINING a Session */


// In our UNWGameInstance.cpp:

void USessionGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnJoinSessionComplete %s, %d"), *SessionName.ToString(), static_cast<int32>(Result)));

	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Clear the Delegate again
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("SessionValid")));
			// Get the first local PlayerController, so we can call "ClientTravel" to get to the Server Map
			// This is something the Blueprint Node "Join Session" does automatically!
			// 
			// Start navigating player to the selected online session level
			APlayerController* pLocalPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

			// A container for the engine to know where we are going to
			FString strTravelUrl;

			// Checking the First Local PlayerController is not null and the session name is match with the selected online session
			if (pLocalPlayerController && Sessions->GetResolvedConnectString(SessionName, strTravelUrl))
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Travel To Level"));
				pLocalPlayerController->ClientTravel(strTravelUrl, ETravelType::TRAVEL_Absolute);

				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, strTravelUrl);
			}
		}
	}
}







void USessionGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnDestroySessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Clear the Delegate
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);

			// If it was successful, we just load another level (could be a MainMenu!)
			if (bWasSuccessful)
			{
				UGameplayStatics::OpenLevel(GetWorld(), "Lobby", true);
			}
		}
	}
}








void USessionGameInstance::StartOnlineGame(const FString Mapname)
{
	// Creating a local player where we can get the UserID from
	ULocalPlayer* const Player = GetFirstGamePlayer();

	//First get a reference to whichever game player you need, in this case the local one
	const TSharedPtr<const FUniqueNetId> netID = UGameplayStatics::GetGameInstance(GetWorld())->GetFirstGamePlayer()->GetPreferredUniqueNetId().GetUniqueNetId();

	// Call our custom HostSession function. GameSessionName is a GameInstance variable
	HostSession(netID, FName(RoomId), false, true, 50, Mapname);
}

void USessionGameInstance::FindOnlineGames(const FString RoomID)
{
	ULocalPlayer* const Player = GetWorld()->GetFirstLocalPlayerFromController();
	RoomId = RoomID;
	//First get a reference to whichever game player you need, in this case the local one
	// const TSharedPtr<const FUniqueNetId> netID = UGameplayStatics::GetGameInstance(GetWorld())->GetFirstGamePlayer()->GetPreferredUniqueNetId().GetUniqueNetId();

	FindOnlineSessions(false, true);
}

// In our UNWGameInstance.cpp:

void USessionGameInstance::JoinOnlineGame(const FString Mapname, const FString RoomID)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Joining started")));
	//ULocalPlayer* const Player = GetFirstGamePlayer();
	// Output = false;
	
	// Just a SearchResult where we can save the one we want to use, for the case we find more than one!
	FOnlineSessionSearchResult SearchResult;
	MapName = Mapname;
	RoomId = RoomID;
	// If the Array is not empty, we can go through it
	if (SessionSearch->SearchResults.Num() > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Search result found")));
		for (int32 i = 0; i < SessionSearch->SearchResults.Num(); i++)
		{
			// To avoid something crazy, we filter sessions from ourself
			
				//First get a reference to whichever game player you need, in this case the local one
				const TSharedPtr<const FUniqueNetId> netID = UGameplayStatics::GetGameInstance(GetWorld())->GetFirstGamePlayer()->GetPreferredUniqueNetId().GetUniqueNetId();

				JoinOnlineSession(netID, FName(RoomId), SearchResult);
				break;
			
		}
	}
}


void USessionGameInstance::DestroySessionAndLeaveGame()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();


		if (Sessions.IsValid())
		{
			Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

			Sessions->DestroySession(FName(RoomId));
		}
	}
}

//void USessionGameInstance::CreateDeviceID(FString UserID) {
//	UE_LOG(LogTemp, Warning, TEXT("Create Device ID Called!"));
//
//	EOS_HPlatform platform = PlatformHandle;
//	EOS_HConnect connect = EOS_Platform_GetConnectInterface(platform);
//	const TCHAR* id = *UserID;
//	void* clientData = TCHAR_TO_UTF8(id);
//	std::string NameString(TCHAR_TO_UTF8(*UserID));
//	const char* name = NameString.c_str();
//	const char* devModel = name;
//	UE_LOG(LogTemp, Warning, TEXT("Create Device ID Called 2!"));
//	EOS_Connect_CreateDeviceIdOptions deviceOptions = {};
//	deviceOptions.ApiVersion = 1;
//	deviceOptions.DeviceModel = "PC Windows";
//
//	/*EOS_Connect_OnCreateDeviceIdCallback createDeviceIDcallback = [](const EOS_Connect_CreateDeviceIdCallbackInfo* CreatedeviceCallbackData)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Device ID Created!"));
//		EOSLogin(UserID, UserID);
//	};*/
//	UE_LOG(LogTemp, Warning, TEXT("Create Device ID Called! 3"));
//	FCreateDeviceIDCallback *CallbackObj = new FCreateDeviceIDCallback();
//	UE_LOG(LogTemp, Warning, TEXT("Create Device ID Called! 4"));
//
//	CallbackObj->CallbackLambda = [UserID, this](const EOS_Connect_CreateDeviceIdCallbackInfo* Data) {
//		UE_LOG(LogTemp, Warning, TEXT("Device ID Created!"));
//		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Device ID Created!")));
//		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Result:")));
//		EOSLogin(UserID, UserID);
//	};
//
//	UE_LOG(LogTemp, Warning, TEXT("Create Device ID Called! 6"));
//
//	EOS_Connect_CreateDeviceId(connect, &deviceOptions, nullptr, CallbackObj->GetCallbackPtr());
//	UE_LOG(LogTemp, Warning, TEXT("Create Device ID Called! 5"));
//
//	
//}
//
////void DeviceIdCallback(const EOS_Connect_CreateDeviceIdCallbackInfo* Data) {
////	UE_LOG(LogTemp, Warning, TEXT("Device ID Created!"));
////}
//
//
//void USessionGameInstance::EOSLogin(FString UserID, FString displayName) {
//	UE_LOG(LogTemp, Warning, TEXT("login EOS User called!"));
//
//	EOS_HPlatform platform = PlatformHandle;
//	EOS_HConnect connect = EOS_Platform_GetConnectInterface(platform);
//	const TCHAR* id = *UserID;
//	void* clientData = TCHAR_TO_UTF8(id);
//
//	EOS_Connect_LoginOptions LoginOptions = {};
//	EOS_Connect_Credentials creds = {};
//	creds.Token = nullptr;
//	creds.Type = EOS_EExternalCredentialType::EOS_ECT_DEVICEID_ACCESS_TOKEN;
//	creds.ApiVersion = 1;
//
//	EOS_Connect_UserLoginInfo info = {};
//	UE_LOG(LogTemp, Warning, TEXT("Display Nme: %s"), *displayName);
//	std::string NameString(TCHAR_TO_UTF8(*displayName));
//	const char* name = NameString.c_str();
//	info.DisplayName = name;
//	info.ApiVersion = 1;
//	
//	LoginOptions.Credentials = &creds;
//	LoginOptions.UserLoginInfo = &info;
//	LoginOptions.ApiVersion = 2;
//
//	FConnectLoginCallback* CallbackObj = new FConnectLoginCallback();
//
//	EOS_Connect_Login(connect, &LoginOptions, (void*)CallbackObj, CallbackObj->GetCallbackPtr());
//
//	CallbackObj->CallbackLambda = [creds, this](const EOS_Connect_LoginCallbackInfo* Data) {
//		EOS_Connect_LoginCallbackInfo LoginData = *Data;
//		EOS_EResult loginResult = LoginData.ResultCode;
//		UE_LOG(LogTemp, Warning, TEXT("Result Code: %s"), EOS_EResult_ToString(loginResult));
//		EOS_ContinuanceToken continuanceToken = LoginData.ContinuanceToken;
//		if (loginResult == EOS_EResult::EOS_Success) {
//			PUID = LoginData.LocalUserId;
//			bool puidString = bool(EOS_ProductUserId_IsValid(PUID));
//
//			UE_LOG(LogTemp, Warning, TEXT("PUID: %d"), puidString);
//		}
//		else if (loginResult == EOS_EResult::EOS_NotFound) {
//			UE_LOG(LogTemp, Warning, TEXT("New User creating!"));
//			CreateEOSUser(continuanceToken);
//		}
//		else {
//			UE_LOG(LogTemp, Warning, TEXT("Login Failed!"));
//		}
//	};
//	
//	
//}
//
//
//
//void USessionGameInstance::CreateEOSUser(EOS_ContinuanceToken continuanceToken) {
//	UE_LOG(LogTemp, Warning, TEXT("Create EOS User called!"));
//
//	EOS_HPlatform platform = PlatformHandle;
//	EOS_HConnect connect = EOS_Platform_GetConnectInterface(platform);
//
//	EOS_Connect_CreateUserOptions CreateUseroptions = {};
//
//	CreateUseroptions.ContinuanceToken = continuanceToken;
//	CreateUseroptions.ApiVersion = 1;
//
//	FCreateUserCallback* CallbackObj = new FCreateUserCallback();
//
//	CallbackObj->CallbackLambda = [this](const EOS_Connect_CreateUserCallbackInfo* Data) {
//		PUID = Data->LocalUserId;
//		bool puidString = bool(EOS_ProductUserId_IsValid(PUID));
//		UE_LOG(LogTemp, Warning, TEXT("PUID: %d"), puidString);
//	};
//
//	EOS_Connect_CreateUser(connect, &CreateUseroptions, (void*)CallbackObj, CallbackObj->GetCallbackPtr());
//
//}
//
//void USessionGameInstance::CreateUserCallback(EOS_Connect_CreateUserCallbackInfo const* CreateUserCallbackData)
//{
//	PUID = CreateUserCallbackData->LocalUserId;
//	bool puidString = bool(EOS_ProductUserId_IsValid(PUID));
//	UE_LOG(LogTemp, Warning, TEXT("PUID: %d"), puidString);
//}

// Initialize EOS SDK

/*
void USessionGameInstance::InitializeEosSdk()
{
	EOS_InitializeOptions initOptions = {};
	initOptions.ApiVersion = 4;
	initOptions.ProductName = "Creatospace";
	initOptions.ProductVersion = 0;
	initOptions.Reserved = NULL;
	initOptions.AllocateMemoryFunction = NULL;
	initOptions.ReallocateMemoryFunction = NULL;
	initOptions.ReleaseMemoryFunction = NULL;
	
	EOS_Initialize(&initOptions);

	EOS_Platform_Options PlatformOptions = {};
	PlatformOptions.ApiVersion = 10;
	if (IsRunningDedicatedServer() ) {
		PlatformOptions.bIsServer = EOS_TRUE;
		PlatformOptions.ClientCredentials.ClientId = "xyza7891A5DhRFWxLyEHljgmyUg9JWrs";
		PlatformOptions.ClientCredentials.ClientSecret = "u1xAD2EI1WiSVJnF2V3Xk0DNDjcPaoRkjfPRNeKsx68";
	}
	else {
		PlatformOptions.bIsServer = EOS_FALSE;
		PlatformOptions.ClientCredentials.ClientId = "xyza7891T0GK7SHklff1YL956mHzFZIk";
		PlatformOptions.ClientCredentials.ClientSecret = "ulConLU6kJDUpGHfSIfON9cM4TOn6Y9U/jfbP+F/6H8";
	}
	PlatformOptions.Reserved = NULL;
	PlatformOptions.ProductId = "a7a339130fe04ef0a84c8e8c5ee19d27";
	PlatformOptions.SandboxId = "0c29dc2cdc744e5c9bebcc1109e264ed";
	PlatformOptions.DeploymentId = "8e95a37730924f708acb27dd265db81d";
	PlatformOptions.EncryptionKey = "b5cf94b2524d4726ff683e651cfa43c54251518696fad88c382da80b4b16d0f0";
	PlatformOptions.OverrideCountryCode = "+91";
	PlatformOptions.OverrideLocaleCode = "en-IN";
	PlatformOptions.Flags = NULL;

	PlatformOptions.IntegratedPlatformOptionsContainerHandle = NULL ;
	PlatformOptions.TickBudgetInMilliseconds = 0;

	EOS_Platform_RTCOptions rtc = {};
	rtc.PlatformSpecificOptions = NULL;
	rtc.ApiVersion = EOS_PLATFORM_RTCOPTIONS_API_LATEST;

	PlatformOptions.RTCOptions = NULL;

	PlatformHandle = EOS_Platform_Create(&PlatformOptions);
}

	*/











/*

FString USessionGameInstance::PUIDtoSTring(EOS_ProductUserId puid) {

	char* outbuffer = "";
	int32_t InOutBufferLength;
	InOutBufferLength = EOS_OSS_STRING_BUFFER_LENGTH;
	EOS_EResult result = EOS_ProductUserId_ToString(puid, outbuffer, &InOutBufferLength);

	if (result == EOS_EResult::EOS_Success) {
		FString idstring = outbuffer;
		//UE_LOG(LogTemp, Warning, TEXT("puid to string: %s"), idstring);
		return idstring;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("puid to string failed!"));
		return FString("");
	}
}

EOS_ProductUserId USessionGameInstance::StringtoPUID(FString id) {
	char* puid = TCHAR_TO_ANSI(*id);
	return EOS_ProductUserId_FromString(puid);
}


void USessionGameInstance::CreateEOSVoiceRoomToken(FString id)
{
	
	UE_LOG(LogTemp, Warning, TEXT("Query Join Room Token Started!"));

	
	//EOS_HRTCAdmin Handle = EOS_Platform_GetRTCAdminInterface();
	FCreateRoomTokenCallback* CallbackObj = new FCreateRoomTokenCallback();
	EOS_ProductUserId puid = StringtoPUID(id);

	EOS_RTCAdmin_QueryJoinRoomTokenOptions roomOptions;
	roomOptions.ApiVersion = EOS_RTCADMIN_QUERYJOINROOMTOKEN_API_LATEST;
	roomOptions.LocalUserId = PUID;
	roomOptions.RoomName = "Room";
	EOS_ProductUserId ids = puid;

	roomOptions.TargetUserIds = &ids;
	roomOptions.TargetUserIdsCount = 1;
	roomOptions.TargetUserIpAddresses = NULL;

	CallbackObj->CallbackLambda = [ids, this](const EOS_RTCAdmin_QueryJoinRoomTokenCompleteCallbackInfo* Data) {
		if (Data->ResultCode == EOS_EResult::EOS_Success)
		{
			UE_LOG(LogTemp, Warning, TEXT("Query Join Room Token Success!"));
			GetRoomToken(ids, Data->QueryId, Data->ClientBaseUrl);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Result Code: %s"), UTF8_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
		}
	};

	EOS_RTCAdmin_QueryJoinRoomToken(EOS_Platform_GetRTCAdminInterface(PlatformHandle), &roomOptions, (void*)CallbackObj, CallbackObj->GetCallbackPtr());

}

void USessionGameInstance::GetRoomToken(EOS_ProductUserId id, uint32_t query, const char* url)
{
	UE_LOG(LogTemp, Warning, TEXT("Getting Room Token"));
	EOS_HRTCAdmin Handle = {};



	EOS_RTCAdmin_CopyUserTokenByUserIdOptions tokenOptions;
	tokenOptions.ApiVersion = EOS_RTCADMIN_COPYUSERTOKENBYUSERID_API_LATEST;
	tokenOptions.QueryId = query;
	tokenOptions.TargetUserId = id;

	EOS_RTCAdmin_UserToken** token = {};

	EOS_RTCAdmin_CopyUserTokenByUserId(EOS_Platform_GetRTCAdminInterface(PlatformHandle), &tokenOptions, token);

	if (token != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Token Copied!"));

		EOS_RTCAdmin_UserToken tok = **token;
		BaseUrl = url;
		VoiceToken = tok.Token;
		//JoinVoiceRoom(id, url, tok.Token);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Token Not Copied!"));
	}

}
*/

void USessionGameInstance::JoinVoiceRoom(FString url, FString token)
{

}
/*
	UE_LOG(LogTemp, Warning, TEXT("Join Voice Join Room Called!"));
	EOS_HRTC Handle = {};
	FJoinRoomCallback* CallbackObj = new FJoinRoomCallback();

	EOS_RTC_JoinRoomOptions joinRoomOptions;
	joinRoomOptions.ApiVersion = EOS_RTC_JOINROOM_API_LATEST;
	joinRoomOptions.bManualAudioInputEnabled = EOS_FALSE;
	joinRoomOptions.bManualAudioOutputEnabled = EOS_FALSE;
	joinRoomOptions.ClientBaseUrl = TCHAR_TO_ANSI(*url);
	joinRoomOptions.Flags = EOS_RTC_JOINROOMFLAGS_ENABLE_ECHO;
	joinRoomOptions.LocalUserId = PUID;
	joinRoomOptions.ParticipantId = PUID;
	const char* tok = token.Token ;
	joinRoomOptions.ParticipantToken = TCHAR_TO_ANSI(*token);;
	joinRoomOptions.RoomName = "Room";

	CallbackObj->CallbackLambda = [this](const EOS_RTC_JoinRoomCallbackInfo* Data) {
		if (Data->ResultCode == EOS_EResult::EOS_Success)
		{
			UE_LOG(LogTemp, Warning, TEXT("Joined Voice Room!"));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Result Code: %s"), UTF8_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
		}
	};
	EOS_RTC_JoinRoom(EOS_Platform_GetRTCInterface(PlatformHandle), &joinRoomOptions, (void*)CallbackObj, CallbackObj->GetCallbackPtr());
}
*/

/*

void USessionGameInstance::EOSLoginWithDeviceID() {
	UE_LOG(LogTemp, Warning, TEXT("login EOS User called!"));


	EOS_Platform_Options PlatformOptions;
	PlatformOptions.ApiVersion = 11;
	if (IsRunningDedicatedServer()) {
		PlatformOptions.bIsServer = EOS_TRUE;

	}
	else {
		PlatformOptions.bIsServer = EOS_FALSE;

	}
	PlatformOptions.ClientCredentials.ClientId = "xyza7891T0GK7SHklff1YL956mHzFZIk";
	PlatformOptions.ClientCredentials.ClientSecret = "ulConLU6kJDUpGHfSIfON9cM4TOn6Y9U/jfbP+F/6H8";
	PlatformOptions.Reserved = NULL;
	PlatformOptions.ProductId = "a7a339130fe04ef0a84c8e8c5ee19d27";
	PlatformOptions.SandboxId = "0c29dc2cdc744e5c9bebcc1109e264ed";
	PlatformOptions.DeploymentId = "8e95a37730924f708acb27dd265db81d";
	PlatformOptions.EncryptionKey = "b5cf94b2524d4726ff683e651cfa43c54251518696fad88c382da80b4b16d0f0";
	PlatformOptions.OverrideCountryCode = "+91";
	PlatformOptions.OverrideLocaleCode = "en-IN";
	uint64 OverlayFlags = 0;
	
	PlatformOptions.Flags = IsRunningGame() ? OverlayFlags : EOS_PF_DISABLE_OVERLAY;;
	PlatformOptions.CacheDirectory = NULL;
	PlatformOptions.IntegratedPlatformOptionsContainerHandle = NULL;
	PlatformOptions.TickBudgetInMilliseconds = 0;

	EOS_Platform_RTCOptions rtc = { 0 };
	EOS_Windows_RTCOptions pltOptions;
	pltOptions.ApiVersion = EOS_WINDOWS_RTCOPTIONS_API_LATEST;
	pltOptions.XAudio29DllPath = "C:\\Program Files\\Epic Games\\UE_5.0_SourceBuild\\Engine\\Source\\ThirdParty\\EOSSDK\\SDK\\Bin\\x64\\xaudio2_9redist.dll";
	rtc.PlatformSpecificOptions = &pltOptions;
	rtc.ApiVersion = EOS_PLATFORM_RTCOPTIONS_API_LATEST;

	PlatformOptions.RTCOptions = &rtc;

	PlatformHandle = EOS_Platform_Create(&PlatformOptions);


	FConnectLoginCallback* CallbackObj = new FConnectLoginCallback();

	if (PlatformHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("Platform Handle Valid!"));
		EOS_HConnect connectHandle = EOS_Platform_GetConnectInterface(PlatformHandle);
		if (connectHandle)
		{
			UE_LOG(LogTemp, Warning, TEXT("Connect Handle Valid!"));
			EOS_Connect_LoginOptions LoginOptions = {};
			EOS_Connect_Credentials creds = {};
			creds.Token = nullptr;
			creds.Type = EOS_EExternalCredentialType::EOS_ECT_DEVICEID_ACCESS_TOKEN;
			creds.ApiVersion = EOS_CONNECT_CREDENTIALS_API_LATEST;

			EOS_Connect_UserLoginInfo info = {};
			UE_LOG(LogTemp, Warning, TEXT("Connect Handle Valid 2!"));
			info.DisplayName = "name";
			info.ApiVersion = EOS_CONNECT_USERLOGININFO_API_LATEST;

			LoginOptions.Credentials = &creds;
			LoginOptions.UserLoginInfo = &info;
			LoginOptions.ApiVersion = EOS_CONNECT_LOGIN_API_LATEST;

			int32 LocalUserNum = 0;
			UE_LOG(LogTemp, Warning, TEXT("Connect Handle Valid 3!"));
			CallbackObj->CallbackLambda = [LocalUserNum, this](const EOS_Connect_LoginCallbackInfo* Data) {
				EOS_EResult loginResult = Data->ResultCode;
				UE_LOG(LogTemp, Warning, TEXT("Result Code: %s"), EOS_EResult_ToString(loginResult));
				if (loginResult == EOS_EResult::EOS_Success) {

					UE_LOG(LogTemp, Warning, TEXT("Logged In with Device ID!"));
					//APlayerController* controller = GetFirstLocalPlayerController(GetWorld());

					AddLocalUser(LocalUserNum, nullptr, Data->LocalUserId);
					FUniqueNetIdEOSPtr NetIdEOS = GetLocalUniqueNetIdEOS(LocalUserNum);
					PUID = Data->LocalUserId;
					PUIDString = PUIDtoSTring(PUID);
					//UE_LOG(LogTemp, Warning, TEXT("PUID String: %s"), PUIDString);
				}
				else if (loginResult == EOS_EResult::EOS_NotFound) {
					UE_LOG(LogTemp, Warning, TEXT("New Device ID creating!"));
					CreateDeviceID();
				}
				else {
					UE_LOG(LogTemp, Warning, TEXT("Login with Device ID Failed: %s"), UTF8_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
				}
			};

			UE_LOG(LogTemp, Warning, TEXT("Connect Handle Valid 4!"));

			EOS_Connect_Login(connectHandle, &LoginOptions, (void*)CallbackObj, CallbackObj->GetCallbackPtr());
			UE_LOG(LogTemp, Warning, TEXT("Connect Handle Valid 5!"));

		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Connect Handle Invalid!"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Platform Handle Invalid!"));
	}
}
*/
	void USessionGameInstance::DeleteDeviceID() {

	}

	void USessionGameInstance::CreateDeviceID() {
		UE_LOG(LogTemp, Warning, TEXT("Create Device ID Called!"));
	}
		/*EOS_HPlatform platform = PlatformHandle;
		EOS_HConnect connect = EOS_Platform_GetConnectInterface(platform);
		const TCHAR* id = *UserID;
		void* clientData = TCHAR_TO_UTF8(id);*/
		//std::string NameString(TCHAR_TO_UTF8(*UserID));
		//const char* name = NameString.c_str();
		//const char* devModel = name;

		/*
		EOS_Connect_CreateDeviceIdOptions deviceOptions = {};
		deviceOptions.ApiVersion = 1;
		deviceOptions.DeviceModel = "PC Windows";

		FCreateDeviceIDCallback* CallbackObj = new FCreateDeviceIDCallback();

		int32 LocalUserNum = 0;
		CallbackObj->CallbackLambda = [this](const EOS_Connect_CreateDeviceIdCallbackInfo* Data) {
			UE_LOG(LogTemp, Warning, TEXT("Device ID Created!"));
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Device ID Created!")));
			if (Data->ResultCode == EOS_EResult::EOS_Success || Data->ResultCode == EOS_EResult::EOS_Success)
			{
				EOSLoginWithDeviceID();
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Device ID Failed to be Created: %s"), EOS_EResult_ToString(Data->ResultCode));
			}
		};
		EOS_Connect_CreateDeviceId(EOS_Platform_GetConnectInterface(PlatformHandle), &deviceOptions, (void*)CallbackObj, CallbackObj->GetCallbackPtr());
	}

	*/



