// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WebBrowser.h"
#include "RoomWebLink.h"
#include "SWebBrowser.h"
#include "MainPlatformWebBrowser.generated.h"

/**
 * 
 */
UCLASS()
class CREATOSPACE_API UMainPlatformWebBrowser : public UWebBrowser
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Setup Main Browser"), Category = "Main Platform|Web Browser")
		void SetupMainBrowser(const FMainPlatformWebBrowserResponse& Response);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Bind Browser To Object"), Category = "Main Platform|Web Browser")
		void BindBrowserToObject();

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	UPROPERTY()
		URoomWebLink* WebLinkObject;
	
};
