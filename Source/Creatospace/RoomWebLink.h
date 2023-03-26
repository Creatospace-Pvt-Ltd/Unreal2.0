// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RoomWebLink.generated.h"
 
/**
 * 
 */
DECLARE_DYNAMIC_DELEGATE_TwoParams(FMainPlatformWebBrowserResponse, const FString&, RoomID, const FString&, UserID);
UCLASS()
class CREATOSPACE_API URoomWebLink : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Main Platform")
		void RoomIDGenerated(FString JsonResponse, FString UserID);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Main Platform Set Callback"), Category = "Main Platform")
		void SetRoomIDCallback(const FMainPlatformWebBrowserResponse& WebBrowserCallback);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Main Platform Get Last RoomID"), Category = "Main Platform")
		FString GetLastRoomID();

private:

	FMainPlatformWebBrowserResponse WebBrowserResponse;
	FString LastRoomID;
	
};
