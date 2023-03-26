// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MPGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CREATOSPACE_API AMPGPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	/* Constructor */
	AMPGPlayerController(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Dedicated Server")
		void AcceptClient(FString ClientID);

	UFUNCTION(BlueprintCallable, Category = "Dedicated Server")
		void TerminateGameSession();

	UFUNCTION(BlueprintCallable, Category = "Dedicated Server")
		void RemovePlayerSession(FString SessionId);

	UFUNCTION(BlueprintCallable, Category = "Dedicated Server")
		void ManageNewPlayer(APlayerController* NewPlayer);

	UFUNCTION(BlueprintCallable, Category = "Dedicated Server")
		void StartVoice();
};
