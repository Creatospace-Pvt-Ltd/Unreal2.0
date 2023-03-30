// Fill out your copyright notice in the Description page of Project Settings.


#include "One_Click_EOS_Subsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineUserCloudInterface.h"
#include "Kismet/GameplayStatics.h"




//Login with EOS
void UOne_Click_EOS_Subsystem::Login_With_EOS(FString ID, FString Token, FString LoginType)
{
	IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if(SubsystemRef)
	{
		IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface();
		if (IdentityPointerRef)
			{
			
			FOnlineAccountCredentials AccountDetails;
			AccountDetails.Id = ID;
			AccountDetails.Token = Token;
			AccountDetails.Type = LoginType;
			IdentityPointerRef->OnLoginCompleteDelegates->AddUObject(this,&UOne_Click_EOS_Subsystem::LoginWithEOSCompleted);
			UE_LOG(LogTemp, Warning, TEXT("Login Called!"));
			IdentityPointerRef->Login(0,AccountDetails);
		}
	}
}

void UOne_Click_EOS_Subsystem::LoginWithEOSCompleted(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserId,
	const FString& Error)
{
	LoginResponse.Broadcast(LocalUserNum, bWasSuccess, Error);
	
	if(bWasSuccess)
	{
		UE_LOG(LogTemp,Warning,TEXT("Login Success changed"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Login Fail Reason - %S"), *Error);
	}
}















//Create EOS Sessions
void UOne_Click_EOS_Subsystem::Create_EOS_Session(bool bIsDedicatedServer, bool bIsLanServer, bool bAllowInvites,
                                                  int32 NumberOfPublicConnections, int32 NumberOfPrivateConnections, bool bUseLobbiesIfAvailable,
                                                  bool bUseLobbiesVoiceChatIfAvailable, bool bUsesPresence, bool bAllowJoinViaPresence,
                                                  bool bAllowJoinViaPresenceFriendsOnly, bool bShouldAdvertise, bool bAntiCheatActivated, FString LevelPath)
{
	IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if(SubsystemRef)
	{
		IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
		if(SessionPtrRef)
		{
			OpenLevelName = LevelPath;
			FOnlineSessionSettings SessionCreationInfo;
			SessionCreationInfo.bIsDedicated = bIsDedicatedServer;
			SessionCreationInfo.bAllowInvites = bAllowInvites;
			SessionCreationInfo.bIsLANMatch = bIsLanServer;
			SessionCreationInfo.NumPublicConnections = NumberOfPublicConnections;
			SessionCreationInfo.NumPrivateConnections = NumberOfPrivateConnections;
			SessionCreationInfo.bAntiCheatProtected = bAntiCheatActivated;
			SessionCreationInfo.bUseLobbiesIfAvailable = bUseLobbiesIfAvailable;
			SessionCreationInfo.bUseLobbiesVoiceChatIfAvailable = bUseLobbiesVoiceChatIfAvailable;
			SessionCreationInfo.bUsesPresence =bUsesPresence;
			SessionCreationInfo.bAllowJoinViaPresence = bAllowJoinViaPresence;
			SessionCreationInfo.bAllowJoinViaPresenceFriendsOnly = bAllowJoinViaPresenceFriendsOnly;
			SessionCreationInfo.bShouldAdvertise = bShouldAdvertise;
			SessionCreationInfo.Set(SEARCH_KEYWORDS, FName(NAME_GameSession).ToString(), EOnlineDataAdvertisementType::ViaOnlineService);
			SessionPtrRef->OnCreateSessionCompleteDelegates.AddUObject(this, &UOne_Click_EOS_Subsystem::OnCreateSessionCompleted);
			SessionPtrRef->CreateSession(0, FName(NAME_GameSession), SessionCreationInfo);
		}
	}
}


void UOne_Click_EOS_Subsystem::OnCreateSessionCompleted(FName SessionName, bool bWasSuccesful)
{
	CreateSessionResponse.Broadcast(SessionName, bWasSuccesful);
	if(bWasSuccesful)
	{
		GetWorld()->ServerTravel(OpenLevelName);
	}
}










//Find EOS Sessions

void UOne_Click_EOS_Subsystem::Find_EOS_Sessions(int MaxSearchResults, bool bIsLandQuery, bool bSearchLobby)
{
	IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if(SubsystemRef)
	{
		IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
		if(SessionPtrRef)
		{
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			SessionSearch->bIsLanQuery = bIsLandQuery;
			SessionSearch->MaxSearchResults = MaxSearchResults;
			SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, bSearchLobby,EOnlineComparisonOp::Equals);
			SessionPtrRef->OnFindSessionsCompleteDelegates.AddUObject(this, &UOne_Click_EOS_Subsystem::OnFindSessionCompleted);
			SessionPtrRef->FindSessions(0,SessionSearch.ToSharedRef());
		}
	}
}


void UOne_Click_EOS_Subsystem::OnFindSessionCompleted(bool bWasSuccess)
{
	TArray<FBlueprintSessionResult> LocalResultArray;
	for(int i=0; i<SessionSearch->SearchResults.Num();i++)
	{
		FBlueprintSessionResult LocalResult;
		LocalResult.OnlineResult = SessionSearch->SearchResults[i];
		LocalResultArray.Add(LocalResult);
	}
	FindSessionResponse.Broadcast(bWasSuccess, LocalResultArray);	
}










//Join EOS Sessions
void UOne_Click_EOS_Subsystem::Join_EOS_Sessions(FBlueprintSessionResult SessionToJoin)
{
	IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if(SubsystemRef)
	{
		IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
		if(SessionPtrRef)
		{
			if(SessionToJoin.OnlineResult.IsValid())
			{
				SessionPtrRef->OnJoinSessionCompleteDelegates.AddUObject(this, &UOne_Click_EOS_Subsystem::OnJoinSessionCompleted);
				SessionPtrRef->JoinSession(0, FName("EOS_ExampleSession"), SessionToJoin.OnlineResult);
			}
			else
			{
				JoinSessionResponse.Broadcast(false, "Check Log");
				UE_LOG(LogTemp,Error, TEXT("Session Not Valid"));
			}
		}
	}
}


void UOne_Click_EOS_Subsystem::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if(Result == EOnJoinSessionCompleteResult::Success)
	{
		if(APlayerController* PlayerControllerRef = UGameplayStatics::GetPlayerController(GetWorld(),0))
		{
			FString JoinAddress;
			IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld());
			if(SubsystemRef)
			{
				IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
				if(SessionPtrRef)
				{
					SessionPtrRef->GetResolvedConnectString(FName("EOS_ExampleSession"),JoinAddress);
					UE_LOG(LogTemp,Warning,TEXT("Join Address is %s"), *JoinAddress);
					if(!JoinAddress.IsEmpty())
					{
						JoinSessionResponse.Broadcast(true, LexToString(Result));
						PlayerControllerRef->ClientTravel(JoinAddress,ETravelType::TRAVEL_Absolute);
					}
				}
			}
		}
	}
	else
	{
		JoinSessionResponse.Broadcast(false, "Check Log");
	}
}






//Extra Functions
FString UOne_Click_EOS_Subsystem::GetPlayerUsername()
{
	IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if(SubsystemRef)
	{
		IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface();
		if(IdentityPointerRef)
		{
			if(IdentityPointerRef->GetLoginStatus(0) == ELoginStatus::LoggedIn)
			{
				return IdentityPointerRef->GetPlayerNickname(0);
			}
		}
	}
	return FString();
			
}

bool UOne_Click_EOS_Subsystem::IsPlayerLoggedIn()
{
	IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if(SubsystemRef)
	{
		IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface();
		if(IdentityPointerRef)
		{
			if(IdentityPointerRef->GetLoginStatus(0) == ELoginStatus::LoggedIn)
			{
				return true;
				
			}
		}
	}
	return false;
}

FString UOne_Click_EOS_Subsystem::getVoiceToken()
{
	FString token = "";
	IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubsystemRef)
	{
		IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface();
		if (IdentityPointerRef)
		{
			token = IdentityPointerRef->GetAuthToken(-2);
		}
	}
	return token;
}

FString UOne_Click_EOS_Subsystem::getBaseUrl()
{
	FString url = "";
	IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubsystemRef)
	{
		IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface();
		if (IdentityPointerRef)
		{
			url = IdentityPointerRef->GetAuthToken(-3);
		}
	}
	return url;
}

FString UOne_Click_EOS_Subsystem::getPUID()
{
	FString puid = "";
	IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubsystemRef)
	{
		IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface();
		if (IdentityPointerRef)
		{
			puid = IdentityPointerRef->GetAuthToken(-4);
		}
	}
	return puid;
}










