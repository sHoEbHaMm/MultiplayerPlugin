// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

void ALobbyGameMode::PostLogin(APlayerController* newPlayer)
{
	Super::PostLogin(newPlayer);

	if (GameState)
	{
		int32 numberOfPlayers = GameState.Get()->PlayerArray.Num();

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("Players in game: %d"), numberOfPlayers)
			);

			APlayerState* playerState = newPlayer->GetPlayerState<APlayerState>();
			if (playerState)
			{
				FString playerName = playerState->GetPlayerName();

				GEngine->AddOnScreenDebugMessage(
					-1, 10.0f, FColor::Cyan, FString::Printf(TEXT("%s has joined the game"), *playerName)
				);
			}
		}
	}
}

void ALobbyGameMode::Logout(AController* exitingPlayer)
{
	Super::Logout(exitingPlayer);

	if (GameState)
	{
		int32 numberOfPlayers = GameState.Get()->PlayerArray.Num();

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("Players in game: %d"), numberOfPlayers - 1)
			);

			APlayerState* playerState = exitingPlayer->GetPlayerState<APlayerState>();
			if (playerState)
			{
				FString playerName = playerState->GetPlayerName();

				GEngine->AddOnScreenDebugMessage(
					-1, 10.0f, FColor::Cyan, FString::Printf(TEXT("%s has left the game"), *playerName)
				);
			}
		}
	}
}
