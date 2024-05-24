// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Menu.generated.h"

/**
 * 
 */
UCLASS()
class GOMULTIPLAYER_API UMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetupMenu(int32 NumberOfPublicConnections = 4, FString TypeOfMatch = FString(TEXT("FreeForAll")), FString LobbyPath = FString("/Game/ThirdPerson/Maps/Lobby"));

protected:
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;

	/* Callbacks for custom delegates created in plugin */
	UFUNCTION()
	void OnCreateSessionComplete(bool bWasSuccessful);

	void OnFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& sessionResults, bool bWasSuccessful);
	void OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type Result);

	UFUNCTION()
	void OnDestroySessionComplete(bool bWasSuccessful);
	UFUNCTION()
	void OnStartSessionComplete(bool bWasSuccessful);

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UFUNCTION()
	void OnClicked_HostButton();
	UFUNCTION()
	void OnClicked_JoinButton();

	void MenuTearDown();

	/* Subsystem designed to handle multiplayer functionality */
	class UGoMultiplayerSubsystem* GoMultiplayerSubsystem;

	int32 NumPublicConnections{ 4 };
	FString MatchType{ TEXT("FreeForAll") };
	FString PathToLobby{ TEXT("FreeForAll") };
};
