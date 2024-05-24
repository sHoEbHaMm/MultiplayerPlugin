// Fill out your copyright notice in the Description page of Project Settings.


#include "GoMultiplayerSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include <Online/OnlineSessionNames.h>

UGoMultiplayerSubsystem::UGoMultiplayerSubsystem() :
	CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
	FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete)),
	JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete)),
	DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete)),
	StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete))
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();

	if (OnlineSubsystem)
	{
		SessionInterface = OnlineSubsystem->GetSessionInterface();
	}
} 





void UGoMultiplayerSubsystem::CreateSession(int32 NumOfPublicConnections, FString MatchType)
{
	if (!SessionInterface.IsValid())
		return;

	auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);

	if (ExistingSession != nullptr) //Check if there is already existing session, if yes, delete it
	{
		SessionInterface->DestroySession(NAME_GameSession);
	}

	CreateSessionCompleteDelegate_Handle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate); //Added delegate to delegates list

	lastSessionSettings = MakeShareable(new FOnlineSessionSettings());

	lastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	lastSessionSettings->NumPublicConnections = NumOfPublicConnections;
	lastSessionSettings->bAllowJoinInProgress = true;
	lastSessionSettings->bAllowJoinViaPresence = true;
	lastSessionSettings->bShouldAdvertise = true;
	lastSessionSettings->bUsesPresence = true;
	lastSessionSettings->bUseLobbiesIfAvailable = true;

	lastSessionSettings->Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	lastSessionSettings->BuildUniqueId = 1;

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	if (!SessionInterface->CreateSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *lastSessionSettings))
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate_Handle);
		GMSOnCreateSessionComplete.Broadcast(false);
	}
}




void UGoMultiplayerSubsystem::FindSessions(int32 maxSearchResults)
{
	if (!SessionInterface.IsValid())
		return;

	FindSessionsCompleteDelegate_Handle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

	lastSessionSearch = MakeShareable(new FOnlineSessionSearch());

	lastSessionSearch->MaxSearchResults = maxSearchResults;
	lastSessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	lastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	if (!SessionInterface->FindSessions(*localPlayer->GetPreferredUniqueNetId(), lastSessionSearch.ToSharedRef()))
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate_Handle);
		GMSOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
	}
	
}




void UGoMultiplayerSubsystem::JoinSession(const FOnlineSessionSearchResult& sessionResult)
{
	if (!SessionInterface.IsValid())
	{
		GMSOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	JoinSessionCompleteDelegate_Handle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	if (!SessionInterface->JoinSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, sessionResult))
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate_Handle);
		GMSOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

void UGoMultiplayerSubsystem::DestroySession()
{
}




void UGoMultiplayerSubsystem::StartSession()
{
}

























void UGoMultiplayerSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate_Handle);
	}
	GMSOnCreateSessionComplete.Broadcast(bWasSuccessful); //This will call OnCreateSessionComplete from Menu class
}




void UGoMultiplayerSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate_Handle);
	}

	if(lastSessionSearch->SearchResults.Num() <= 0)
	{
		GMSOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("<0 error"))
			);
		}
		return;
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("Joining"))
		);
	}
	GMSOnFindSessionsComplete.Broadcast(lastSessionSearch->SearchResults, bWasSuccessful); //This will call OnFindSessionsComplete from Menu class
}




void UGoMultiplayerSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate_Handle);
	}
	GMSOnJoinSessionComplete.Broadcast(Result);
}




void UGoMultiplayerSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
}




void UGoMultiplayerSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
}



