// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineError.h"
#include "OnlineSessionSettings.h"
#include "FindSessionsCallbackProxy.h"
#include "VoiceChat.h"
#include "GameFramework/SaveGame.h"
#include "Interfaces/OnlineStatsInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "One_Click_EOS_Subsystem.generated.h"


//Declaring Dynamic Multicast
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCreateSessionDelegate, FName, SessionName, bool, bWasSuccesful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FLoginDelegate, int32, LocalUserNum, bool, bWasSuccess, FString, Error);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFindSessionDelegate, bool, bWasSuccess,const TArray<FBlueprintSessionResult>&, SearchResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FJoinSessionDelegate, bool, bWasSuccess, FString, Result);


UCLASS()
class ONECLICKEOS_API UOne_Click_EOS_Subsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	//Login with Epic Online Services
	UFUNCTION(BlueprintCallable, Category="EOS Functions")
	void Login_With_EOS(FString ID, FString Token, FString LoginType);

	//Create Session for Epic Online Services
	UFUNCTION(BlueprintCallable, Category="EOS Functions")
	void Create_EOS_Session(bool bIsDedicatedServer, bool bIsLanServer, bool bAllowInvites, int32 NumberOfPublicConnections, int32 NumberOfPrivateConnections, bool bUseLobbiesIfAvailable, bool bUseLobbiesVoiceChatIfAvailable, bool bUsesPresence, bool bAllowJoinViaPresence, bool bAllowJoinViaPresenceFriendsOnly, bool bShouldAdvertise, bool bAntiCheatActivated, FString LevelPath);

	//Find Session for Epic Online Services
	UFUNCTION(BlueprintCallable, Category="EOS Functions")
	void Find_EOS_Sessions(int MaxSearchResults = 20, bool bIsLandQuery = false, bool bSearchLobby = true);

	//Join Session for Epic Online Services
	UFUNCTION(BlueprintCallable, Category="EOS Functions")
	void Join_EOS_Sessions(FBlueprintSessionResult SessionToJoin);

	//Get EOS Player Username
	UFUNCTION(BlueprintCallable,BlueprintPure, Category="EOS Functions")
	FString GetPlayerUsername();

	//Get EOS Player Logged In
	UFUNCTION(BlueprintCallable,BlueprintPure, Category="EOS Functions")
	bool IsPlayerLoggedIn();





	//Delegates for Epic Online Services
	UPROPERTY(BlueprintAssignable, Category="EOS Delegates")
	FCreateSessionDelegate CreateSessionResponse;

	UPROPERTY(BlueprintAssignable, Category="EOS Delegates")
	FLoginDelegate LoginResponse;

	UPROPERTY(BlueprintAssignable, Category="EOS Delegates")
	FFindSessionDelegate FindSessionResponse;

	UPROPERTY(BlueprintAssignable, Category="EOS Delegates")
	FJoinSessionDelegate JoinSessionResponse;

	//Responses for Epic Online Services
	void LoginWithEOSCompleted(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserId, const FString& Error);
	void OnCreateSessionCompleted(FName SessionName, bool bWasSuccesful);
	void OnFindSessionCompleted(bool bWasSuccess);
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnVoiceLoginComplete(const FString& PlayerName, const FVoiceChatResult& Result);


	//Variables for Epic Online Services
	FString OpenLevelName;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

};
