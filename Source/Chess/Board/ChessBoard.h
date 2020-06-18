// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "ChessBoardCell.h"
#include "ChessBoard.generated.h"

class AChessBoardCell;
class AChessPlayerBase;

/** Event to notify game continue */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameContinue);

/**	Event to notify game finished
 *	Win codes:
 *	0 - Checkmate
 *	1 - Stalemate
 *	2 - Resign
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameFinished, int32, WinTeamId, int32, WinCode);

UCLASS()
class CHESS_API AChessBoard : public AActor
{
	GENERATED_BODY()

public:
	AChessBoard();

	/** Default root component */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		class USceneComponent* DefaultRoot;

	/** Number of cells along each side of board */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Board")
		int32 NumCells;

	/** Spacing of cells */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Board")
		float CellSpacing;

	/** Create chess board */
	void PrepareGameBoard();
	
	/** Evaluate with game rules */
	void EvaluateGame();
	
	/** OnGameContinue event */
	UPROPERTY()
		FOnGameContinue OnGameContinue;
	
	/** OnGameFinished event */
	UPROPERTY()
		FOnGameFinished OnGameFinished;

protected:
	/** Create game cells */
	void CreateGameCells();

	/** Create non game cells */
	void CreateNonGameCells();

	/** Create figures */
	void CreateFigures();

public:
	FORCEINLINE TArray<AChessBoardCell*> GetCells() const { return GameCells; };
	FORCEINLINE TArray<AFigureBase*> GetTeam1ActiveFigures() const { return Team1ActiveFigures; }
	FORCEINLINE TArray<AFigureBase*> GetTeam2ActiveFigures() const { return Team2ActiveFigures; }
	FORCEINLINE TArray<AFigureBase*> GetTeam1BeatenFigures() const { return Team1BeatenFigures; }
	FORCEINLINE TArray<AFigureBase*> GetTeam2BeatenFigures() const { return Team2BeatenFigures; }
	
	UFUNCTION()
		void MoveFigure(AFigureBase* Figure, FIntPoint Address);

	/** Calculate available moves of figures */
	TArray<FMoveResult> CalculateFiguresMoves();
	
	AChessBoardCell* GetCellByAddress(FIntPoint Address);
	AFigureBase* GetFigureByAddress(FIntPoint Address);

	/** Cleanup game board */
	void Cleanup();
	
protected:
	UPROPERTY()
		TArray<AChessBoardCell*> GameCells;

	UPROPERTY()
		TArray<AFigureBase*> Team1ActiveFigures;

	UPROPERTY()
		TArray<AFigureBase*> Team2ActiveFigures;

	UPROPERTY()
		TArray<AChessBoardCell*> CellsBeaten;

	UPROPERTY()
		TArray<AFigureBase*> Team1BeatenFigures;

	UPROPERTY()
		TArray<AFigureBase*> Team2BeatenFigures;
	
	void SetFigureBeaten(AFigureBase* Figure);

	void SwapFiguresOnCellsIds(int32 FirstCellId, int32 SecondCellId);

	/** Convert CellAddress to A-H 1-8 format */
	static FString CellAddressToHumanFormat(FIntPoint Address);

	UPROPERTY()
		AFigureBase* King1;

	UPROPERTY()
		AFigureBase* King2;
	
private:
	UPROPERTY()
		TMap<int32, TSubclassOf<class AFigureBase>> InitialFiguresLocations;
};