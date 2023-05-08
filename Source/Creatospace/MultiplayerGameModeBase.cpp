// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameModeBase.h"
#include "MPGPlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "Engine.h"
#include "EngineGlobals.h"
#include "GameLiftServerSDK.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSubsystem.h"

AMultiplayerGameModeBase::AMultiplayerGameModeBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)

{
    bUseSeamlessTravel = true;
	/* Use our custom Player-Controller Class */
	// PlayerControllerClass = AMPGPlayerController::StaticClass();

    //Let's run this code only if GAMELIFT is enabled. Only with Server targets!
#if WITH_GAMELIFT

    //Getting the module first.
    FGameLiftServerSDKModule* gameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));

    //InitSDK establishes a local connection with GameLift's agent to enable communication.
    gameLiftSdkModule->InitSDK();

    //Respond to new game session activation request. GameLift sends activation request 
    //to the game server along with a game session object containing game properties 
    //and other settings. Once the game server is ready to receive player connections, 
    //invoke GameLiftServerAPI.ActivateGameSession()
    auto onGameSession = [=](Aws::GameLift::Server::Model::GameSession gameSession)
    {
        gameLiftSdkModule->ActivateGameSession();
    };

    FProcessParameters* params = new FProcessParameters();
    params->OnStartGameSession.BindLambda(onGameSession);


    FString commandLine = UKismetSystemLibrary::GetCommandLine();

    FString newport;
    TArray<FString> commandLineArgs;
    commandLine.ParseIntoArray(commandLineArgs, TEXT(" "));

    // Parse launch parameters for port
    for (const FString& arg : commandLineArgs)
    {
        if (arg.StartsWith("-port="))
        {
            newport = arg.Mid(6);
            break;
        }
    }

    params->OnTerminate.BindLambda([=]() {gameLiftSdkModule->ProcessEnding(); });

    //HealthCheck callback. GameLift invokes this callback about every 60 seconds. By default, 
    //GameLift API automatically responds 'true'. A game can optionally perform checks on 
    //dependencies and such and report status based on this info. If no response is received  
    //within 60 seconds, health status is recorded as 'false'. 
    //In this example, we're always healthy!
    params->OnHealthCheck.BindLambda([]() {return true; });

    //Here, the game server tells GameLift what port it is listening on for incoming player 
    //connections. In this example, the port is hardcoded for simplicity. Since active game
    //that are on the same instance must have unique ports, you may want to assign port values
    //from a range, such as:
    // const int32 port = FURL::UrlConfig.DefaultPort;
    // params->port;
    params->port = FCString::Atoi(*newport);

    //Here, the game server tells GameLift what set of files to upload when the game session 
    //ends. GameLift uploads everything specified here for the developers to fetch later.
    TArray<FString> logfiles;
    logfiles.Add(TEXT("aLogFile.txt"));
    params->logParameters = logfiles;

    //Call ProcessReady to tell GameLift this game server is ready to receive game sessions!
    gameLiftSdkModule->ProcessReady(*params);

#endif
}

//UClass* AMultiplayerGameModeBase::GetDefaultPawnClassForController(AController* InController)
//{
	//Super::GetDefaultPawnClassForController(InController);
	/* Override Functionality to get Pawn from PlayerController */
	
	/* If we don't get the right Controller, use the Default Pawn */
	//return DefaultPawnClass;
//}

void AMultiplayerGameModeBase::InitGamelift(int port)
{
#if WITH_GAMELIFT
    FGameLiftServerSDKModule* gameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));

    FProcessParameters* params = new FProcessParameters();
    //params->OnStartGameSession.BindLambda(onGameSession);

    //OnProcessTerminate callback. GameLift invokes this before shutting down the instance 
    //that is hosting this game server to give it time to gracefully shut down on its own. 
    //In this example, we simply tell GameLift we are indeed going to shut down.
    
#endif
}

void AMultiplayerGameModeBase::ChangeServerMap(FString MapName) 
{
    UWorld server = UWorld();
    server.ServerTravel(MapName, true);
}

void AMultiplayerGameModeBase::AcceptClient(FString ClientID)
{
    //Getting the module first.
    FGameLiftServerSDKModule* gameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));

    gameLiftSdkModule->AcceptPlayerSession(ClientID);

    UE_LOG(LogTemp, Warning, TEXT("client accepted"));
}

void AMultiplayerGameModeBase::HandleNewPlayer(APlayerController* NewPlayer)
{
    // Super::HandleStartingNewPlayer(NewPlayer);

    if (NewPlayer)
    {

        FUniqueNetIdRepl UniqueNetIDRepl;
        if (NewPlayer->IsLocalController()) {

            ULocalPlayer* LocalPlayerRef = NewPlayer->GetLocalPlayer();
            if (LocalPlayerRef) {
                UniqueNetIDRepl = LocalPlayerRef->GetPreferredUniqueNetId();
            }
            else {
                UNetConnection* RemoteNetConnecionRef = Cast<UNetConnection>(NewPlayer->Player);
                check(IsValid(RemoteNetConnecionRef));
                UniqueNetIDRepl = RemoteNetConnecionRef->PlayerId;
            }
        }
        else {
            UNetConnection* RemoteNetConnecionRef = Cast<UNetConnection>(NewPlayer->Player);
            check(IsValid(RemoteNetConnecionRef));
            UniqueNetIDRepl = RemoteNetConnecionRef->PlayerId;
        }
        TSharedPtr<const FUniqueNetId> UniqueNetId = UniqueNetIDRepl.GetUniqueNetId();
        if (UniqueNetId != nullptr) {
            IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(NewPlayer->GetWorld());
            IOnlineSessionPtr SessionRef = SubsystemRef->GetSessionInterface();
            bool bRegSuccess = SessionRef->RegisterPlayer(FName(NAME_GameSession), *UniqueNetId, false);
            if (bRegSuccess) {
                UE_LOG(LogTemp, Warning, TEXT("Registration Successful!"));
            }
        }
    }
}