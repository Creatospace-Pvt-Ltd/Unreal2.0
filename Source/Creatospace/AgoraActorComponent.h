// India © Creatospace Pvt. Ltd. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <AgoraCppPlugin/IAgoraSpatialAudio.h>
#include <AgoraPluginInterface.h>

#include "AgoraActorComponent.generated.h"

using namespace agora::rtc;
using namespace agora;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CREATOSPACE_API UAgoraActorComponent : public UActorComponent
	, public IRtcEngineEventHandler
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAgoraActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	IRtcEngine* agoraEngine;
	std::string appId = "";
	std::string channelName = "";
	std::string token = "";
	bool isJoin = false;
	int remoteUId;
	FVector myPosition;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	int InitAgoraEngine(FString APP_ID, FString TOKEN, FString CHANNEL_NAME);

	UFUNCTION(BlueprintCallable)
	int configureSpatialAudioEngine(FVector position, FVector frwd, FVector ryt, FVector upp);

	UFUNCTION(BlueprintCallable)
	int refreshLocalPosition(FVector position, FVector frwd, FVector ryt, FVector upp);

	UFUNCTION(BlueprintCallable)
	int refreshRemotePosition(int id, FVector position, FVector frwd);

	// Occurs when a remote user joins the channel.
	void onUserJoined(uid_t uid, int elapsed) override;
	// Occurs when a local user joins the channel.
	void onJoinChannelSuccess(const char* channel, uid_t uid, int elapsed) override;
	// Occurs when you leave the channel.
	void onLeaveChannel(const RtcStats& stats) override;
	// Occurs when the remote user drops offline.
	void onUserOffline(uid_t uid, USER_OFFLINE_REASON_TYPE reason) override;

	UFUNCTION(BlueprintCallable)
	int OnJoinButtonClick(int UID, FString TOKEN, FString CHANNEL_NAME);
};
