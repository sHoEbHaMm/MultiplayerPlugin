// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "GoMultiplayerSubsystem.generated.h"

/**
 * Declaring custom delegates
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGMSOnCreateSessionComplete, bool, bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FGMSOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& sessionResults, bool bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_OneParam(FGMSOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGMSOnDestroySessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGMSOnStartSessionComplete, bool, bWasSuccessful);


UCLASS()
class GOMULTIPLAYER_API UGoMultiplayerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UGoMultiplayerSubsystem();

	/* To handle session functionality */
	void CreateSession(int32 NumOfPublicConnections, FString MatchType);
	void FindSessions(int32 maxSearchResults);
	void JoinSession(const FOnlineSessionSearchResult& sessionResult);
	void DestroySession();
	void StartSession();

	/* Custom delegates for communication with Menu Class */
	FGMSOnCreateSessionComplete GMSOnCreateSessionComplete;
	FGMSOnFindSessionsComplete GMSOnFindSessionsComplete;
	FGMSOnJoinSessionComplete GMSOnJoinSessionComplete;
	FGMSOnDestroySessionComplete GMSOnDestroySessionComplete;
	FGMSOnStartSessionComplete GMSOnStartSessionComplete;

protected:
	/* Internal callbacks for the delegates added to the Online Session Interface's delegate list
	* Need not to be called outside this class. */
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful); 
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);

private:
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSettings> lastSessionSettings;
	TSharedPtr<FOnlineSessionSearch> lastSessionSearch;


	/* To add to Online Session Interface's delegate list 
	* We will also bind our GoMultiplayerSubsystem internal callbacks to these. */
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegate_Handle;

	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegate_Handle;

	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegate_Handle;

	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegate_Handle;

	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegate_Handle;

	bool bCreateSessionOnDestroy{ false };
	int32 lastNumPublicConnections;
	FString lastMatchType;
};
