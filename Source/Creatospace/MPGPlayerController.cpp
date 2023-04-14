// Fill out your copyright notice in the Description page of Project Settings.


#include "MPGPlayerController.h"
#include <Net/UnrealNetwork.h>
#include "Engine/World.h"
#include "MultiplayerGameModeBase.h"
#include "EngineGlobals.h"
#include "GameLiftServerSDK.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSubsystem.h"

AMPGPlayerController::AMPGPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	/* Initialize The Values */
	//PawnToUseA = NULL;
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Creatospace/Blueprints/BP_RPM_ThirdPersonChar"));

	/* Make sure the PawnClass is Replicated */
	bReplicates = true;
}

void AMPGPlayerController::AcceptClient(FString ClientID)
{
	//Getting the module first.
	FGameLiftServerSDKModule* gameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));

	gameLiftSdkModule->AcceptPlayerSession(ClientID);

	UE_LOG(LogTemp, Warning, TEXT("client accepted"));
}

void AMPGPlayerController::TerminateGameSession()
{
	FGameLiftServerSDKModule* gameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));

	gameLiftSdkModule->ProcessEnding();

	UE_LOG(LogTemp, Warning, TEXT("game session terminated"));
}

void AMPGPlayerController::RemovePlayerSession(FString SessionId)
{
	FGameLiftServerSDKModule* gameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));

	gameLiftSdkModule->RemovePlayerSession(SessionId);

	UE_LOG(LogTemp, Warning, TEXT("player session terminated"));
}

void AMPGPlayerController::ManageNewPlayer(APlayerController* NewPlayer)
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
                UE_LOG(LogTemp, Warning, TEXT("1"));
                if (IsValid(RemoteNetConnecionRef)) {
                    UniqueNetIDRepl = RemoteNetConnecionRef->PlayerId;
                    UE_LOG(LogTemp, Warning, TEXT("2"));
                }
            }
        }
        else {
            UNetConnection* RemoteNetConnecionRef = Cast<UNetConnection>(NewPlayer->Player);
            UE_LOG(LogTemp, Warning, TEXT("4"));
            if (IsValid(RemoteNetConnecionRef)) {
                UniqueNetIDRepl = RemoteNetConnecionRef->PlayerId;
                UE_LOG(LogTemp, Warning, TEXT("UniqueIdRepl %s"), *UniqueNetIDRepl.ToString());
            }
        }
        TSharedPtr<const FUniqueNetId> UniqueNetId = UniqueNetIDRepl.GetUniqueNetId();
        if (UniqueNetId != nullptr) {
            IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(NewPlayer->GetWorld());
            IOnlineSessionPtr SessionRef = SubsystemRef->GetSessionInterface();
            bool bRegSuccess = SessionRef->RegisterPlayer(FName("EOS_ExampleSession"), *UniqueNetId, false);
            if (bRegSuccess) {
                UE_LOG(LogTemp, Warning, TEXT("Registration Successful!"));
            }
            else {
                UE_LOG(LogTemp, Warning, TEXT("Registration Nahi hua!"));
            }
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("Unique ID is null ptr!"));
        }
    }
}

void AMPGPlayerController::StartVoice()
{
    
}