// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MultiplayerGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class CREATOSPACE_API AMultiplayerGameModeBase : public AGameModeBase
{
	GENERATED_UCLASS_BODY()
	
		/* Override To Read In Pawn From Custom Controller */
		//UClass* GetDefaultPawnClassForController(AController* InController);
public:
	
	UFUNCTION(BlueprintCallable, Category = "Dedicated Server")
		void HandleNewPlayer(APlayerController* NewPlayer);

	UFUNCTION(BlueprintCallable, Category = "Dedicated Server")
		void ChangeServerMap(FString MapName);

	UFUNCTION(BlueprintCallable, Category = "Dedicated Server")
		void AcceptClient(FString ClientID);

};
