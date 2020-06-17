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
	void GameFinished();
	bool CheckGameFinished();

	UFUNCTION()
		void TogglePlayersMoves();

	UPROPERTY()
		bool bHasActiveGame;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
		bool bGamePaused;
	
	UPROPERTY()
		bool bPlayersMoveSwitch;

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
		void GameFinished(AChessPlayerBase* WinPlayer, int32 WinCode);
};