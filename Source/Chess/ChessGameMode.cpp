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
	// Default player class
	DefaultPawnClass = AChessPlayer::StaticClass();

	// Player controller class
	PlayerControllerClass = AChessPlayerController::StaticClass();

	// HUD class
	HUDClass = AChessHUD::StaticClass();

	bHasActiveGame = false;
	bGamePaused = false;
	bPlayersMoveSwitch = false;
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
	if (bGamePaused)
	{
		TogglePause();
	}

	if (bHasActiveGame)
	{
		GameFinished();
	}

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FTransform SpawnTransform = FTransform();

	// Spawn and prepare game board
	AActor* SpawnedActor = GetWorld()->SpawnActor(AChessBoard::StaticClass(), &SpawnTransform, SpawnInfo);
	GameBoard = Cast<AChessBoard>(SpawnedActor);
	GameBoard->PrepareGameBoard();

	// Reference and setup player pawn
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	Player1 = Cast<AChessPlayerBase>(PlayerPawn);
	SetupChessPlayer(Player1, 1);

	// Spawn, reference and setup AI pawn
	SpawnedActor = GetWorld()->SpawnActor(AChessPlayerAI::StaticClass(), &SpawnTransform, SpawnInfo);
	Player2 = Cast<AChessPlayerAI>(SpawnedActor);
	SetupChessPlayer(Player2, 2);

	bHasActiveGame = true;
	TogglePlayersMoves();
}

void AChessGameMode::SetupChessPlayer(AChessPlayerBase* ChessPlayer, int32 TeamId)
{
	ChessPlayer->TeamIndex = TeamId;
	ChessPlayer->GameBoard = GameBoard;
	ChessPlayer->OnMovePerformed.AddDynamic(this, &AChessGameMode::TogglePlayersMoves);
	ChessPlayer->OnGameFinished.AddDynamic(this, &AChessGameMode::GameFinished);
}

void AChessGameMode::TogglePlayersMoves()
{
	bPlayersMoveSwitch = !bPlayersMoveSwitch;
	MovesCounter++;

	if (CheckGameFinished())
	{
		return;
	}
	
	if (bPlayersMoveSwitch)
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

void AChessGameMode::GameFinished()
{
	if (bHasActiveGame)
	{
		bHasActiveGame = false;
		bPlayersMoveSwitch = false;
		MovesCounter = -1;

		if (GameBoard)
		{
			GameBoard->Cleanup();
			GameBoard = nullptr;
		}

		Player1->OnMovePerformed.RemoveDynamic(this, &AChessGameMode::TogglePlayersMoves);
		Player1->OnGameFinished.RemoveDynamic(this, &AChessGameMode::GameFinished);
		Player1 = nullptr;

		if (Player2)
		{
			Player2->OnMovePerformed.RemoveDynamic(this, &AChessGameMode::TogglePlayersMoves);
			Player2->OnGameFinished.RemoveDynamic(this, &AChessGameMode::GameFinished);
			Player2->Destroy();
			Player2 = nullptr;
		}
	}
}

void AChessGameMode::GameFinished(AChessPlayerBase* WinPlayer, int32 WinCode)
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
				InfoText = TEXT("Checkmate!" ) + FString::Printf(TEXT("Team %d wins!"), WinPlayer->TeamIndex);
			}
			else if (WinCode == 1)
			{
				InfoText = TEXT("Stalemate!" ) + FString::Printf(TEXT("Team %d wins!"), WinPlayer->TeamIndex);
			}
			else if (WinCode == 2)
			{
				InfoText = FString::Printf(TEXT("Team %d resigned!"), WinPlayer->TeamIndex);
			}

			ChessHUD->FinishGame(FText::FromString(InfoText));
		}
	}
}

bool AChessGameMode::CheckGameFinished()
{
	TArray<FMoveResult> AllMoves = Player2->CalculateAvailableMoves();
	TArray<FMoveResult> Player1Moves;
	TArray<FMoveResult> Player2Moves;

	for (auto& M : AllMoves)
	{
		if (M.Figure->GetTeamIndex() == 1)
		{
			Player1Moves.Add(M);
		}

		if (M.Figure->GetTeamIndex() == 2)
		{
			Player2Moves.Add(M);
		}
	}

	
	//Stalemate
	if (Player1Moves.Num() == 0)
	{
		Player1->OnGameFinished.Broadcast(Player1, 1);
		return true;
	}

	if (Player2Moves.Num() == 0)
	{
		Player2->OnGameFinished.Broadcast(Player2, 1);
		return true;
	}
	//Stalemate

	
	//Checkmate 1
	if (GameBoard->GetKing1()->bIsBeaten)
	{
		Player1->OnGameFinished.Broadcast(Player2, 0);
		return true;
	}
	
	bool bKing1HasNoMoves = false;
	bool bKing1IsUnderAttack = false;
	GameBoard->GetKing1()->CalculateMovesResults();
	if (GameBoard->GetKing1()->MoveResults.Num() == 0)
	{
		bKing1HasNoMoves = true;
	}

	AChessBoardCell* King1Cell = GameBoard->GetCellByAddress(GameBoard->GetKing1()->CellAddress);
	for (auto& M : Player2Moves)
	{
		if (GameBoard->GetCellByAddress(M.CellAddress) == King1Cell)
		{
			bKing1IsUnderAttack = true;
			break;
		}
	}

	const bool bKing1Checkmate = bKing1HasNoMoves && bKing1IsUnderAttack;
	if (bKing1Checkmate)
	{
		Player1->OnGameFinished.Broadcast(Player1, 0);
		return true;
	}
	//Checkmate 1


	//Checkmate 2
	if (GameBoard->GetKing2()->bIsBeaten)
	{
		Player2->OnGameFinished.Broadcast(Player1, 0);
		return true;
	}
	
	bool bKing2HasNoMoves = false;
	bool bKing2IsUnderAttack = false;
	GameBoard->GetKing2()->CalculateMovesResults();
	if (GameBoard->GetKing2()->MoveResults.Num() == 0)
	{
		bKing2HasNoMoves = true;
	}

	AChessBoardCell* King2Cell = GameBoard->GetCellByAddress(GameBoard->GetKing2()->CellAddress);
	for (auto& M : Player1Moves)
	{
		if (GameBoard->GetCellByAddress(M.CellAddress) == King2Cell)
		{
			bKing2IsUnderAttack = true;
			break;
		}
	}
	
	const bool bKing2Checkmate = bKing2HasNoMoves && bKing2IsUnderAttack;
	if (bKing2Checkmate)
	{
		Player2->OnGameFinished.Broadcast(Player1, 0);
		return true;
	}

	return false;
}

bool AChessGameMode::TogglePause()
{
	bGamePaused = !bGamePaused;
	UGameplayStatics::SetGamePaused(this, bGamePaused);
	return bGamePaused;
}