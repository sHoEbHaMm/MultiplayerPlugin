// Fill out your copyright notice in the Description page of Project Settings.


#include "GoMultiplayerSubsystem.h"
#include "OnlineSubsystem.h"

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
}

void UGoMultiplayerSubsystem::FindSessions(int32 maxSearchResults)
{
}

void UGoMultiplayerSubsystem::JoinSession(const FOnlineSessionSearchResult& sessionResult)
{
}

void UGoMultiplayerSubsystem::DestroySession()
{
}

void UGoMultiplayerSubsystem::StartSession()
{
}

void UGoMultiplayerSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
}

void UGoMultiplayerSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
}

void UGoMultiplayerSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
}

void UGoMultiplayerSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
}

void UGoMultiplayerSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
}
