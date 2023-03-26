// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomWebLink.h"

void URoomWebLink::RoomIDGenerated(FString RoomID, FString UserID)
{
	FString Url = "";
	FString Uid = "";

		UE_LOG(LogTemp, Warning, TEXT("room found "));
		Url = RoomID;
		Uid = UserID;
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Room ID %s"), *Url));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("User ID %s"), *Uid));

	if (Url.IsEmpty() || Uid.IsEmpty())
	{
		return;
	}

	WebBrowserResponse.Execute(Url, Uid);
	LastRoomID = Url;
}

void URoomWebLink::SetRoomIDCallback(const FMainPlatformWebBrowserResponse& WebBrowserCallback)
{
	WebBrowserResponse = WebBrowserCallback;
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("SetRoomID Callback")));

}

FString URoomWebLink::GetLastRoomID()
{
	return LastRoomID;
}