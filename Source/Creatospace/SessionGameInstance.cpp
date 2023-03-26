// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionGameInstance.h"
#include <Kismet/GameplayStatics.h>
#include <OnlineSessionClient.h>
#include <EOSSDK/SDK/Include/eos_sessions_types.h>
#include <GameFramework/GameModeBase.h>
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSubsystem.h"
#include <OnlineSubsystemEOS.h>
#include <EOSSDK/SDK/Include/eos_integratedplatform.h>
#include <EOSSDK/SDK/Include/eos_integratedplatform_types.h>
#include <EOSSDK/SDK/Include/eos_common.h>
#include <IEOSSDKManager.h>
#include <UserManagerEOS.h>
#include "../../Plugins/OneClickEOS/Source/OneClickEOS/Public/One_Click_EOS_Subsystem.h"

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

void USessionGameInstance::CreateDeviceID(FString UserID) {
	UE_LOG(LogTemp, Warning, TEXT("Create Device ID Called!"));

	EOS_HPlatform platform = PlatformHandle;
	EOS_HConnect connect = EOS_Platform_GetConnectInterface(platform);
	const TCHAR* id = *UserID;
	void* clientData = TCHAR_TO_UTF8(id);
	std::string NameString(TCHAR_TO_UTF8(*UserID));
	const char* name = NameString.c_str();
	const char* devModel = name;
	UE_LOG(LogTemp, Warning, TEXT("Create Device ID Called 2!"));
	EOS_Connect_CreateDeviceIdOptions deviceOptions = {};
	deviceOptions.ApiVersion = 1;
	deviceOptions.DeviceModel = "PC Windows";

	/*EOS_Connect_OnCreateDeviceIdCallback createDeviceIDcallback = [](const EOS_Connect_CreateDeviceIdCallbackInfo* CreatedeviceCallbackData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Device ID Created!"));
		EOSLogin(UserID, UserID);
	};*/
	UE_LOG(LogTemp, Warning, TEXT("Create Device ID Called! 3"));
	FCreateDeviceIDCallback *CallbackObj = new FCreateDeviceIDCallback();
	UE_LOG(LogTemp, Warning, TEXT("Create Device ID Called! 4"));

	CallbackObj->CallbackLambda = [UserID, this](const EOS_Connect_CreateDeviceIdCallbackInfo* Data) {
		UE_LOG(LogTemp, Warning, TEXT("Device ID Created!"));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Device ID Created!")));
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Result:")));
		EOSLogin(UserID, UserID);
	};

	UE_LOG(LogTemp, Warning, TEXT("Create Device ID Called! 6"));

	EOS_Connect_CreateDeviceId(connect, &deviceOptions, nullptr, CallbackObj->GetCallbackPtr());
	UE_LOG(LogTemp, Warning, TEXT("Create Device ID Called! 5"));

	
}

//void DeviceIdCallback(const EOS_Connect_CreateDeviceIdCallbackInfo* Data) {
//	UE_LOG(LogTemp, Warning, TEXT("Device ID Created!"));
//}


void USessionGameInstance::EOSLogin(FString UserID, FString displayName) {
	UE_LOG(LogTemp, Warning, TEXT("login EOS User called!"));

	EOS_HPlatform platform = PlatformHandle;
	EOS_HConnect connect = EOS_Platform_GetConnectInterface(platform);
	const TCHAR* id = *UserID;
	void* clientData = TCHAR_TO_UTF8(id);

	EOS_Connect_LoginOptions LoginOptions = {};
	EOS_Connect_Credentials creds = {};
	creds.Token = nullptr;
	creds.Type = EOS_EExternalCredentialType::EOS_ECT_DEVICEID_ACCESS_TOKEN;
	creds.ApiVersion = 1;

	EOS_Connect_UserLoginInfo info = {};
	UE_LOG(LogTemp, Warning, TEXT("Display Nme: %s"), *displayName);
	std::string NameString(TCHAR_TO_UTF8(*displayName));
	const char* name = NameString.c_str();
	info.DisplayName = name;
	info.ApiVersion = 1;
	
	LoginOptions.Credentials = &creds;
	LoginOptions.UserLoginInfo = &info;
	LoginOptions.ApiVersion = 2;

	FConnectLoginCallback* CallbackObj = new FConnectLoginCallback();

	EOS_Connect_Login(connect, &LoginOptions, (void*)CallbackObj, CallbackObj->GetCallbackPtr());

	CallbackObj->CallbackLambda = [creds, this](const EOS_Connect_LoginCallbackInfo* Data) {
		EOS_Connect_LoginCallbackInfo LoginData = *Data;
		EOS_EResult loginResult = LoginData.ResultCode;
		UE_LOG(LogTemp, Warning, TEXT("Result Code: %s"), EOS_EResult_ToString(loginResult));
		EOS_ContinuanceToken continuanceToken = LoginData.ContinuanceToken;
		if (loginResult == EOS_EResult::EOS_Success) {
			PUID = LoginData.LocalUserId;
			bool puidString = bool(EOS_ProductUserId_IsValid(PUID));

			UE_LOG(LogTemp, Warning, TEXT("PUID: %d"), puidString);
		}
		else if (loginResult == EOS_EResult::EOS_NotFound) {
			UE_LOG(LogTemp, Warning, TEXT("New User creating!"));
			CreateEOSUser(continuanceToken);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Login Failed!"));
		}
	};
	
	
}

//EOS_Connect_OnLoginCallback LoginCallback(EOS_Connect_LoginCallbackInfo const* Data)
//{
//	EOS_Connect_LoginCallbackInfo LoginData = *Data;
//	EOS_EResult loginResult = LoginData.ResultCode;
//	EOS_ContinuanceToken continuanceToken = LoginData.ContinuanceToken;
//	if (*EOS_EResult_ToString(loginResult) == *("EOS_InvalidUser")) {
//		this->CreateEOSUser(continuanceToken);
//	}
//	else {
//		PUID = LoginData.LocalUserId;
//		bool puidString = bool(EOS_ProductUserId_IsValid(PUID));
//		UE_LOG(LogTemp, Warning, TEXT("PUID: %d"), puidString);
//	}
//
//};

void USessionGameInstance::CreateEOSUser(EOS_ContinuanceToken continuanceToken) {
	UE_LOG(LogTemp, Warning, TEXT("Create EOS User called!"));

	EOS_HPlatform platform = PlatformHandle;
	EOS_HConnect connect = EOS_Platform_GetConnectInterface(platform);

	EOS_Connect_CreateUserOptions CreateUseroptions = {};

	CreateUseroptions.ContinuanceToken = continuanceToken;
	CreateUseroptions.ApiVersion = 1;

	FCreateUserCallback* CallbackObj = new FCreateUserCallback();

	CallbackObj->CallbackLambda = [this](const EOS_Connect_CreateUserCallbackInfo* Data) {
		PUID = Data->LocalUserId;
		bool puidString = bool(EOS_ProductUserId_IsValid(PUID));
		UE_LOG(LogTemp, Warning, TEXT("PUID: %d"), puidString);
	};

	EOS_Connect_CreateUser(connect, &CreateUseroptions, (void*)CallbackObj, CallbackObj->GetCallbackPtr());

}

void USessionGameInstance::CreateUserCallback(EOS_Connect_CreateUserCallbackInfo const* CreateUserCallbackData)
{
	PUID = CreateUserCallbackData->LocalUserId;
	bool puidString = bool(EOS_ProductUserId_IsValid(PUID));
	UE_LOG(LogTemp, Warning, TEXT("PUID: %d"), puidString);
}

// Initialize EOS SDK

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

	/*EOS_Platform_RTCOptions rtc = {};
	rtc.PlatformSpecificOptions = NULL;
	rtc.ApiVersion = EOS_PLATFORM_RTCOPTIONS_API_LATEST;*/

	PlatformOptions.RTCOptions = NULL;

	PlatformHandle = EOS_Platform_Create(&PlatformOptions);

}




//void USessionGameInstance::EOSVoiceChatLogin()
//{
//
//	IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
//	if (SubsystemRef) {
//		IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface();
//		if (IdentityPointerRef) {
//			if (IsPlayerLoggedIn()) {
//				IVoiceChat* VoiceChatRef = IVoiceChat::Get();
//				if (VoiceChatRef) {
//					VoiceChatUserRef = VoiceChatRef->CreateUser();
//					if (VoiceChatUserRef) {
//						TSharedPtr<const FUniqueNetId> UserIdRef = IdentityPointerRef->GetUniquePlayerId(0);
//						FPlatformUserId PlatformUserIdRef = IdentityPointerRef->GetPlatformUserIdFromUniqueNetId(*UserIdRef);
//						VoiceChatUserRef->Login(PlatformUserIdRef, UserIdRef->ToString(), TEXT(""), FOnVoiceChatLoginCompleteDelegate::CreateUObject(this, &USessionGameInstance::VoiceLoginCallback));
//					}
//				}
//			}
//		}
//	}
//}
//
//void USessionGameInstance::OnVoiceLoginComplete(const FString& PlayerName, const FVoiceChatResult& Result)
//{
//	if(Result.IsSuccess()) {
//		UE_LOG(LogTemp, Warning, TEXT("Voice Chat Login Success!"));
//	}
//	else{
//		UE_LOG(LogTemp, Warning, TEXT("Voice Chat Login Failed!"));
//	}
//}