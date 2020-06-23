// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ChessGameMode.h"
#include "Player/ChessPlayer.h"
#include "Player/ChessPlayerAI.h"
#include "ChessPlayerController.h"
#include "ChessHUD.h"
#include "Board/ChessBoard.h"
#include "Kismet/GameplayStatics.h"

AChessGameMode::AChessGameMode()
{
	// Default classes
	DefaultPawnClass = AChessPlayer::StaticClass();
	PlayerControllerClass = AChessPlayerController::StaticClass();
	HUDClass = AChessHUD::StaticClass();

	bHasActiveGame = false;
	bPlayersMovesSwitch = false;
	MovesCounter = -1;
}

void AChessGameMode::BeginPlay()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		if (AChessHUD* ChessHUD = Cast<AChessHUD>(PC->GetHUD()))
		{
			ChessHUD->ShowMenuMain();
		}
	}
}

void AChessGameMode::StartChessGame()
{
	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		TogglePause();
	}

	if (bHasActiveGame)
	{
		GameReset();
	}

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FTransform SpawnTransform;

	// Spawn and prepare game board
	AActor* SpawnedActor = GetWorld()->SpawnActor(AChessBoard::StaticClass(), &SpawnTransform, SpawnInfo);
	GameBoard = Cast<AChessBoard>(SpawnedActor);
	GameBoard->PrepareGameBoard();
	GameBoard->OnGameContinue.AddDynamic(this, &AChessGameMode::GameContinue);
	GameBoard->OnGameFinished.AddDynamic(this, &AChessGameMode::GameFinished);

	// Reference and setup player pawn
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	Player1 = Cast<AChessPlayerBase>(PlayerPawn);
	SetupChessPlayer(Player1, 1);

	// Spawn, reference and setup AI pawn
	SpawnedActor = GetWorld()->SpawnActor(AChessPlayerAI::StaticClass(), &SpawnTransform, SpawnInfo);
	Player2 = Cast<AChessPlayerAI>(SpawnedActor);
	SetupChessPlayer(Player2, 2);
	
	bHasActiveGame = true;
	GameContinue();
}

void AChessGameMode::SetupChessPlayer(AChessPlayerBase* ChessPlayer, int32 TeamId)
{
	ChessPlayer->TeamIndex = TeamId;
	ChessPlayer->GameBoard = GameBoard;
	ChessPlayer->OnMovePerformed.AddDynamic(this, &AChessGameMode::GameEvaluate);
}

void AChessGameMode::GameContinue()
{
	bPlayersMovesSwitch = !bPlayersMovesSwitch;
	MovesCounter++;

	if (bPlayersMovesSwitch)
	{
		Player1->TriggerForMakeMove(true);
		Player2->TriggerForMakeMove(false);
	}
	else
	{
		Player1->TriggerForMakeMove(false);
		Player2->TriggerForMakeMove(true);
	}
}

void AChessGameMode::GameEvaluate()
{
	GameBoard->EvaluateGame();
}

void AChessGameMode::GameReset()
{
	if (bHasActiveGame)
	{
		bHasActiveGame = false;
		bPlayersMovesSwitch = false;
		MovesCounter = -1;

		if (GameBoard)
		{
			GameBoard->Cleanup();
			GameBoard->OnGameContinue.RemoveDynamic(this, &AChessGameMode::GameContinue);
			GameBoard->OnGameFinished.RemoveDynamic(this, &AChessGameMode::GameFinished);
			GameBoard = nullptr;
		}

		Player1->OnMovePerformed.RemoveDynamic(this, &AChessGameMode::GameEvaluate);
		
		Player1 = nullptr;

		if (Player2)
		{
			Player2->OnMovePerformed.RemoveDynamic(this, &AChessGameMode::GameEvaluate);
			Player2->Destroy();
			Player2 = nullptr;
		}
	}
}

void AChessGameMode::GameFinished(int32 WinTeamId, int32 WinCode)
{
	Player1->bCanMakeMove = false;
	Player2->bCanMakeMove = false;

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		if (AChessHUD* ChessHUD = Cast<AChessHUD>(PC->GetHUD()))
		{
			FString InfoText;
			
			if (WinCode == 0)
			{
				InfoText = TEXT("Checkmate! ") + FString::Printf(TEXT("Team %d wins!"), WinTeamId);
			}
			else if (WinCode == 1)
			{
				InfoText = TEXT("Stalemate! ") + FString::Printf(TEXT("Team %d wins!"), WinTeamId);
			}
			else if (WinCode == 2)
			{
				InfoText = FString::Printf(TEXT("Team %d resigned!"), WinTeamId);
			}

			ChessHUD->FinishGame(FText::FromString(InfoText));
		}
	}
}

bool AChessGameMode::TogglePause()
{
	return UGameplayStatics::SetGamePaused(this, !UGameplayStatics::IsGamePaused(GetWorld()));
}
