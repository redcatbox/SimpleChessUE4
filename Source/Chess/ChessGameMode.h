// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "ChessGameMode.generated.h"

class AChessPlayerBase;

/** GameMode class to specify pawn, playercontroller, hud and more */
UCLASS()
class CHESS_API AChessGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AChessGameMode();
	void StartChessGame();
	bool TogglePause();
	void GameReset();

	UPROPERTY()
		bool bHasActiveGame;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY()
		bool bPlayersMovesSwitch;

	UPROPERTY()
		int32 MovesCounter;

	UPROPERTY()
		class AChessBoard* GameBoard;

	UPROPERTY()
		class AChessPlayerBase* Player1;

	UPROPERTY()
		class AChessPlayerBase* Player2;

	void SetupChessPlayer(AChessPlayerBase* ChessPlayer, int32 TeamId);

	UFUNCTION()
		void GameEvaluate();
	
	UFUNCTION()
		void GameContinue();

	UFUNCTION()
		void GameFinished(int32 WinTeamId, int32 WinCode);
};