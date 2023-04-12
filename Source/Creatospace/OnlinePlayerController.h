// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OnlinePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CREATOSPACE_API AOnlinePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AOnlinePlayerController(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Dedicated Server")
		void AcceptOnlineClient(FString ClientID);

	UFUNCTION(BlueprintCallable, Category = "Dedicated Server")
		void TerminateOnlineGameSession();

	UFUNCTION(BlueprintCallable, Category = "Dedicated Server")
		void RemoveOnlinePlayerSession(FString SessionId);

	UFUNCTION(BlueprintCallable, Category = "Dedicated Server")
		void ManageNewOnlinePlayer(APlayerController* NewPlayer);
};
