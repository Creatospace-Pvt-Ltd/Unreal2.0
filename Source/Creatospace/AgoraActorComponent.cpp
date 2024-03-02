// India © Creatospace Pvt. Ltd. All Rights Reserved


#include "AgoraActorComponent.h"

// Sets default values for this component's properties
UAgoraActorComponent::UAgoraActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAgoraActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

ILocalSpatialAudioEngine* localSpatial = nullptr;
//IRtcEngine* agoraEngine = nullptr;

// Called every frame
void UAgoraActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int UAgoraActorComponent::InitAgoraEngine(FString APP_ID, FString TOKEN, FString CHANNEL_NAME)
{
	// Create an engine instance.
	agoraEngine = agora::rtc::ue::createAgoraRtcEngine();

	// Specify a context for the engine.
	RtcEngineContext context;
	std::string StdStrAppId = TCHAR_TO_UTF8(*APP_ID);
	context.appId = StdStrAppId.c_str();
	context.eventHandler = this;
	// Choose the communication profile for voice calling.
	context.channelProfile = CHANNEL_PROFILE_TYPE::CHANNEL_PROFILE_LIVE_BROADCASTING;
	// Initialize the engine instance with the context.
	return agoraEngine->initialize(context);
	// Enable the local audio capture to init the local audio stream.
	agoraEngine->enableAudio();
}

int UAgoraActorComponent::configureSpatialAudioEngine(FVector position, FVector frwd, FVector ryt, FVector upp)
{
	LocalSpatialAudioConfig localSpatialAudioConfig;
	localSpatialAudioConfig.rtcEngine = agoraEngine;
	//agoraEngine->queryInterface(AGORA_IID_LOCAL_SPATIAL_AUDIO, (void**)&localSpatial);
	//localSpatial->initialize(localSpatialAudioConfig);
	// By default Agora subscribes to the audio streams of all remote users.
	// Unsubscribe all remote users; otherwise, the audio reception range you set
	// is invalid.
	//localSpatial->muteLocalAudioStream(true);
	//localSpatial->muteAllRemoteAudioStreams(true);

	// Set the audio reception range, in meters, of the local user
	//localSpatial->setAudioRecvRange(50);
	// Set the length, in meters, of unit distance
	//localSpatial->setDistanceUnit(1);

	// Update self position
	float pos[] = { float(position.X), float(position.Y), float(position.Z) };
	float forward[] = { float(frwd.X), float(frwd.Y), float(frwd.Z) };
	float right[] = { float(ryt.X), float(ryt.Y), float(ryt.Z) };
	float up[] = { float(upp.X), float(upp.Y), float(upp.Z) };
	//return localSpatial->updateSelfPosition(pos, forward, right, up);
	return 0;
}

int UAgoraActorComponent::refreshLocalPosition(FVector position, FVector frwd, FVector ryt, FVector upp)
{
	float pos[] = { float(position.X), float(position.Y), float(position.Z)};
	float forward[] = { float(frwd.X), float(frwd.Y), float(frwd.Z) };
	float right[] = { float(ryt.X), float(ryt.Y), float(ryt.Z) };
	float up[] = { float(upp.X), float(upp.Y), float(upp.Z) };
	myPosition = position;
//	localSpatial->updateSelfPosition(pos, forward, right, up);
	return 0;
}

int UAgoraActorComponent::refreshRemotePosition(int id, FVector position, FVector frwd)
{


	SpatialAudioParams params;
	params.speaker_attenuation = 0.2;
	params.speaker_distance = (position - myPosition).Length();
	params.enable_air_absorb = true;

	agoraEngine->setRemoteUserSpatialAudioParams(id, params);
	if ((position - myPosition).Length()<10.0) {
		UE_LOG(LogTemp, Warning, TEXT("remote audio params: %u"), (position - myPosition).Length());

		agoraEngine->adjustUserPlaybackSignalVolume(id, int(100*(position - myPosition).Length() / 10.0));
	}
	else{
		UE_LOG(LogTemp, Warning, TEXT("Out of zone!"));
		agoraEngine->adjustUserPlaybackSignalVolume(id, 0);
	}
	RemoteVoicePositionInfo positionInfo;
	// Set the coordinates in the world coordinate system.
	// This parameter is an array of length 3
	// The three values represent the front, right, and top coordinates
	// Set the unit vector of the x axis in the coordinate system.
	positionInfo.position[0] = float(position.X);
	positionInfo.position[1] = float(position.Y);
	positionInfo.position[2] = float(position.Z);
	// This parameter is an array of length 3,
	// The three values represent the front, right, and top coordinates
	positionInfo.forward[0] = float(frwd.X);
	positionInfo.forward[1] = float(frwd.Y);
	positionInfo.forward[2] = float(frwd.Z);
	//localSpatial->updateRemotePosition(id, positionInfo);
	return 0;
}

void UAgoraActorComponent::onLeaveChannel(const RtcStats& stats)
{
	AsyncTask(ENamedThreads::GameThread, [=]()
		{
			localSpatial->clearRemotePositions();
			localSpatial->release();
			agoraEngine->leaveChannel();
			agoraEngine->unregisterEventHandler(this);
			agoraEngine->release();
			UE_LOG(LogTemp, Warning, TEXT("onLeaveChannel: You left the channel"));
		});
}
void UAgoraActorComponent::onJoinChannelSuccess(const char* channel, uid_t uid, int elapsed)
{
	AsyncTask(ENamedThreads::GameThread, [=]()
		{
			agoraEngine->enableSpatialAudio(true);
			
			UE_LOG(LogTemp, Warning, TEXT("JoinChannelSuccess uid: %u"), uid);
		});
}
void UAgoraActorComponent::onUserJoined(uid_t uid, int elapsed)
{
	AsyncTask(ENamedThreads::GameThread, [=]()
		{
			UE_LOG(LogTemp, Warning, TEXT("onUserJoined uid: %u"), uid);
			remoteUId = uid;
			
		});
}
void UAgoraActorComponent::onUserOffline(uid_t uid, USER_OFFLINE_REASON_TYPE reason)
{
	AsyncTask(ENamedThreads::GameThread, [=]()
		{
			UE_LOG(LogTemp, Warning, TEXT("onUserOffline uid: %u"), uid);
		});
}

int UAgoraActorComponent::OnJoinButtonClick(int UID, FString TOKEN, FString CHANNEL_NAME)
{
	UE_LOG(LogTemp, Warning, TEXT("UMyUserWidget:: OnJoinButtonClick ======"));
	// Set the user role to Host.
	agoraEngine->setClientRole(CLIENT_ROLE_TYPE::CLIENT_ROLE_BROADCASTER);
	ChannelMediaOptions options;
	//options.autoSubscribeAudio = true;
	//options.publishCustomAudioTrack = true;
	//options.isInteractiveAudience = true;
	//options.enableAudioRecordingOrPlayout = true;
	options.publishMicrophoneTrack = true;
	// Join the channel.
	std::string StdStrToken = TCHAR_TO_UTF8(*TOKEN);
	std::string StdStrChannel = TCHAR_TO_UTF8(*CHANNEL_NAME);
	return agoraEngine->joinChannel(StdStrToken.c_str(), StdStrChannel.c_str(), UID, options);
	isJoin = true;
}



