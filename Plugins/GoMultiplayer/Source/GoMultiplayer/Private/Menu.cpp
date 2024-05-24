// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"
#include "Components/Button.h"
#include "GoMultiplayerSubsystem.h"


void UMenu::SetupMenu(int32 NumberOfPublicConnections, FString TypeOfMatch)
{
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
	
	if (GoMultiplayerSubsystem)
	{
		GoMultiplayerSubsystem->CreateSession(NumPublicConnections, MatchType);
	}
}

void UMenu::OnClicked_JoinButton()
{

	if (GoMultiplayerSubsystem)
	{
		
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
			World->ServerTravel("/Game/ThirdPerson/Maps/Lobby?listen");
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
	}
}

void UMenu::OnFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& sessionResults, bool bWasSuccessful)
{
}

void UMenu::OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type Result)
{
}

void UMenu::OnDestroySessionComplete(bool bWasSuccessful)
{
}

void UMenu::OnStartSessionComplete(bool bWasSuccessful)
{
}
