// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlatformWebBrowser.h"
#include <WebJSFunction.h>

static const FString LinkObjectName = TEXT("mainplatformlinkobject");
static const FString JSAddRoomIDGeneratedListener = TEXT("window.addEventListener('message', function(event){ window.ue.mainplatformlinkobject.roomidgenerated(event.data.roomId, event.data.userId);});");


void UMainPlatformWebBrowser::SetupMainBrowser(const FMainPlatformWebBrowserResponse& Response)
{
	BindBrowserToObject();
	WebLinkObject->SetRoomIDCallback(Response);
	ExecuteJavascript(JSAddRoomIDGeneratedListener);
}

void UMainPlatformWebBrowser::BindBrowserToObject()
{
	if (!WebLinkObject) {
		WebLinkObject = NewObject<URoomWebLink>(this, *LinkObjectName);
	}
	WebBrowserWidget->BindUObject(LinkObjectName, WebLinkObject);
}

TSharedRef<SWidget> UMainPlatformWebBrowser::RebuildWidget()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Rebuild")));

		if (CommunityID!="")
		{
			InitialURL += "feed/community/" + CommunityID;
		}
		

	return Super::RebuildWidget();
}