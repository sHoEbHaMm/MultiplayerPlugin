// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"
#include "Components/Button.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GoMultiplayerSubsystem.h"


void UMenu::SetupMenu(int32 NumberOfPublicConnections, FString TypeOfMatch, FString LobbyPath)
{
	PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);
	NumPublicConnections = NumberOfPublicConnections;
	MatchType = TypeOfMatch;

	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* world = GetWorld();

	if (world)
	{
		APlayerController* playerController = world->GetFirstPlayerController();

		if (playerController)
		{
			FInputModeUIOnly inputMode;
			inputMode.SetWidgetToFocus(TakeWidget());
			inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			playerController->SetInputMode(inputMode);
			playerController->SetShowMouseCursor(true);
		}
	}

	GoMultiplayerSubsystem = GetGameInstance()->GetSubsystem<UGoMultiplayerSubsystem>();

	if (GoMultiplayerSubsystem)
	{
		GoMultiplayerSubsystem->GMSOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSessionComplete);
		GoMultiplayerSubsystem->GMSOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessionsComplete);
		GoMultiplayerSubsystem->GMSOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSessionComplete);
		GoMultiplayerSubsystem->GMSOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySessionComplete);
		GoMultiplayerSubsystem->GMSOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSessionComplete);
	}
}

bool UMenu::Initialize()
{
	if (!Super::Initialize())
		return false;

	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &ThisClass::OnClicked_HostButton);
	}

	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &ThisClass::OnClicked_JoinButton);
	}

	return true;
}

void UMenu::OnClicked_HostButton()
{
	HostButton->SetIsEnabled(false);
	if (GoMultiplayerSubsystem)
	{
		GoMultiplayerSubsystem->CreateSession(NumPublicConnections, MatchType);
	}
}

void UMenu::OnClicked_JoinButton()
{
	JoinButton->SetIsEnabled(false);
	if (GoMultiplayerSubsystem)
	{
		GoMultiplayerSubsystem->FindSessions(10000);
	}
}


void UMenu::MenuTearDown()
{
	RemoveFromParent();
	UWorld* world = GetWorld();

	if (world)
	{
		APlayerController* playerController = world->GetFirstPlayerController();

		if (playerController)
		{
			FInputModeGameOnly inputMode;
			playerController->SetInputMode(inputMode);
			playerController->SetShowMouseCursor(false);
		}
	}
}

void UMenu::NativeDestruct()
{
	Super::NativeDestruct();
	MenuTearDown();
}

void UMenu::OnCreateSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, 10.0f, FColor::Green, FString::Printf(TEXT("Session Created Successfully"))
			);
		}

		UWorld* World = GetWorld();

		if (World)
		{
			World->ServerTravel(PathToLobby);
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, 10.0f, FColor::Red, FString::Printf(TEXT("Session Not Created"))
			);
		}
		HostButton->SetIsEnabled(true);
	}
}

void UMenu::OnFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& sessionResults, bool bWasSuccessful)
{	
	if (GoMultiplayerSubsystem == nullptr)
		return;

	for (auto result : sessionResults)
	{
		FString foundMatchType;
		result.Session.SessionSettings.Get(FName("MatchType"), foundMatchType);

		if (foundMatchType == MatchType)
		{
			GoMultiplayerSubsystem->JoinSession(result);
			return;
		}
	}

	if (!bWasSuccessful || sessionResults.Num() == 0)
	{
		JoinButton->SetIsEnabled(true);
	}
}

void UMenu::OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();

	if (OnlineSubsystem)
	{
		IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();

		if (!SessionInterface.IsValid())
			return;

		FString Address;

		SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

		APlayerController* playerController = GetGameInstance()->GetFirstLocalPlayerController();

		if (playerController)
		{
			playerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
		}

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("Joined"))
			);
		}
	}

	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		JoinButton->SetIsEnabled(true);
	}
}

void UMenu::OnDestroySessionComplete(bool bWasSuccessful)
{
}

void UMenu::OnStartSessionComplete(bool bWasSuccessful)
{
}
