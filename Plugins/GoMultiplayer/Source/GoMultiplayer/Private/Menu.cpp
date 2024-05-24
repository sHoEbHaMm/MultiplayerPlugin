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
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 10.0f, FColor::Green, FString::Printf(TEXT("HOSTING"))
		);
	}
	
	if (GoMultiplayerSubsystem)
	{
		GoMultiplayerSubsystem->CreateSession(NumPublicConnections, MatchType);

		UWorld* World = GetWorld();

		if (World)
		{
			World->ServerTravel("/Game/ThirdPerson/Maps/Lobby?listen");
		}
	}
}

void UMenu::OnClicked_JoinButton()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 10.0f, FColor::Green, FString::Printf(TEXT("JOINING"))
		);
	}

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
