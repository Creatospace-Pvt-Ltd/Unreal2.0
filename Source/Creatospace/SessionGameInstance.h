// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <OnlineSubsystemUtils/Private/Tests/TestSessionInterface.h>
//#include <EOSSDK/SDK/Include/eos_sessions_types.h>
//#include "eos_sdk.h"
//#include <EOSSDK/SDK/Include/eos_types.h>
//#include <EOSSDK/SDK/Include/eos_common.h>
//#include "UserManagerEOS.h"
#include "SessionGameInstance.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRoomNotFound);

UCLASS()
class CREATOSPACE_API USessionGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

	/**
	*	Function to host a game!
	*
	*	@Param		UserID			User that started the request
	*	@Param		SessionName		Name of the Session
	*	@Param		bIsLAN			Is this is LAN Game?
	*	@Param		bIsPresence		"Is the Session to create a presence Session"
	*	@Param		MaxNumPlayers	        Number of Maximum allowed players on this "Session" (Server)
	*/
	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers, FString MapName);


	/* Delegate called when session created */
	FOnCreateSessionCompleteDelegate  OnCreateSessionCompleteDelegate;
	/* Delegate called when session started */
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	/** Handles to registered delegates for creating/starting a session */
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	TSharedPtr<class FOnlineSessionSettings> SessionSettings;
	


	USessionGameInstance(const FObjectInitializer& ObjectInitializer);
/**
*	Function fired when a session create request has completed
*
*	@param SessionName the name of the session this callback is for
*	@param bWasSuccessful true if the async action completed without error, false if there was an error
*/
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	/**
	*	Function fired when a session start request has completed
	*/
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);




	void FindOnlineSessions(bool bIsLAN, bool bIsPresence);

/** Delegate for searching for sessions */
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

	/** Handle to registered delegate for searching a session */
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	TSharedPtr<class FOnlineSessionSearch > SessionSearch;

	void OnFindSessionsComplete(bool bWasSuccessful);


/**
*	Joins a session via a search result
*
*	@param SessionName name of session
*	@param SearchResult Session to join
*
*	@return bool true if successful, false otherwise
*/
	bool JoinOnlineSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

/** Delegate for joining a session */
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	/** Handle to registered delegate for joining a session */
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

/**
*	Delegate fired when a session join request has completed
*
*	@param SessionName the name of the session this callback is for
*	@param bWasSuccessful true if the async action completed without error, false if there was an error
*/
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);


    /** Delegate for destroying a session */
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	/** Handle to registered delegate for destroying a session */
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

/**
*	Delegate fired when a destroying an online session has completed
*
*	@param SessionName the name of the session this callback is for
*	@param bWasSuccessful true if the async action completed without error, false if there was an error
*/
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);



	FString MapName;
	FString RoomId;





	
	//FString PUIDtoSTring(EOS_ProductUserId puid);
	//EOS_ProductUserId StringtoPUID(FString id);

	
	
	//void GetRoomToken(EOS_ProductUserId id, uint32_t query, const char* url);
	//EOS_ProductUserId PUID;

	// BlueprintCallable
	public:
		
	UFUNCTION(BlueprintCallable, Category = "Network|Test")
		void StartOnlineGame(const FString Mapname);

	UFUNCTION(BlueprintCallable, Category = "Network|Test")
		void FindOnlineGames(const FString RoomID);

	UFUNCTION(BlueprintCallable, Category = "Network|Test")
		void JoinOnlineGame(const FString Mapname, const FString RoomID);

	UFUNCTION(BlueprintCallable, Category = "Network|Test")
		void DestroySessionAndLeaveGame();

	
	//UFUNCTION(BlueprintCallable, Category = "EOS Auth")
		//void InitializeEosSdk();

	//UFUNCTION(BlueprintCallable, Category = "Network|Test")
	//void EOSLoginWithDeviceID();

	UFUNCTION(BlueprintCallable, Category = "Network|Test")
	void DeleteDeviceID();

	UFUNCTION(BlueprintCallable, Category = "Network|Test")
	void CreateDeviceID();

//	UFUNCTION(BlueprintCallable, Category = "Network|Test")
//	void CreateEOSVoiceRoomToken(FString id);

	UFUNCTION(BlueprintCallable, Category = "Network|Test")
	void JoinVoiceRoom(FString url, FString token);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network|Test")
	FString PUIDString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network|Test")
	FString VoiceToken;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network|Test")
	FString BaseUrl;

	/*UFUNCTION(BlueprintCallable, Category = "EOS Voice")
		void EOSVoiceChatLogin();
	
	IVoiceChatUser* VoiceChatUserRef;*/

	public:
		
		/*typedef TEOSCallback<EOS_Connect_OnLoginCallback, EOS_Connect_LoginCallbackInfo> FConnectLoginCallback;
		typedef TEOSCallback<EOS_RTC_OnJoinRoomCallback, EOS_RTC_JoinRoomCallbackInfo> FJoinRoomCallback;
		typedef TEOSCallback<EOS_RTCAdmin_OnQueryJoinRoomTokenCompleteCallback, EOS_RTCAdmin_QueryJoinRoomTokenCompleteCallbackInfo> FCreateRoomTokenCallback;
		typedef TEOSCallback<EOS_Connect_OnCreateUserCallback, EOS_Connect_CreateUserCallbackInfo> FCreateUserCallback;
		typedef TEOSCallback<EOS_Connect_OnCreateDeviceIdCallback, EOS_Connect_CreateDeviceIdCallbackInfo> FCreateDeviceIDCallback;*/

		
		//EOS_HPlatform PlatformHandle;

		UPROPERTY(BlueprintAssignable)
			FRoomNotFound RoomNotFound;
};
