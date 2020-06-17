// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "ChessBoardCell.h"
#include "ChessBoard.generated.h"

class AChessBoardCell;

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

protected:
	/** Create game cells */
	void CreateGameCells();

	/** Create non game cells */
	void CreateNonGameCells();

	/** Create figures */
	void CreateFigures();

public:
	FORCEINLINE TArray<AChessBoardCell*> GetCells() const { return GameCells; };
	FORCEINLINE TArray<AChessFigureBase*> GetTeam1ActiveFigures() const { return Team1ActiveFigures; }
	FORCEINLINE TArray<AChessFigureBase*> GetTeam2ActiveFigures() const { return Team2ActiveFigures; }
	FORCEINLINE TArray<AChessFigureBase*> GetTeam1BeatenFigures() const { return Team1BeatenFigures; }
	FORCEINLINE TArray<AChessFigureBase*> GetTeam2BeatenFigures() const { return Team2BeatenFigures; }

	FORCEINLINE AChessFigureBase* GetKing1() const { return King1; }
	FORCEINLINE AChessFigureBase* GetKing2() const { return King2; }
	
	UFUNCTION()
		void MoveFigure(AChessFigureBase* Figure, FIntPoint Address);

	AChessBoardCell* GetCellByAddress(FIntPoint Address);
	AChessFigureBase* GetFigureByAddress(FIntPoint Address);

	/** Cleanup game board */
	void Cleanup();
	
protected:
	UPROPERTY()
		TArray<AChessBoardCell*> GameCells;

	UPROPERTY()
		TArray<AChessFigureBase*> Team1ActiveFigures;

	UPROPERTY()
		TArray<AChessFigureBase*> Team2ActiveFigures;

	UPROPERTY()
		TArray<AChessBoardCell*> CellsBeaten;

	UPROPERTY()
		TArray<AChessFigureBase*> Team1BeatenFigures;

	UPROPERTY()
		TArray<AChessFigureBase*> Team2BeatenFigures;
	
	void SetFigureBeaten(AChessFigureBase* Figure);

	void SwapFiguresOnCellsIds(int32 FirstCellId, int32 SecondCellId);

	/** Convert CellAddress to A-H 1-8 format */
	static FString CellAddressToHumanFormat(FIntPoint Address);

	UPROPERTY()
		AChessFigureBase* King1;

	UPROPERTY()
		AChessFigureBase* King2;
	
private:
	UPROPERTY()
		TMap<int32, TSubclassOf<class AChessFigureBase>> InitialFiguresLocations;
};