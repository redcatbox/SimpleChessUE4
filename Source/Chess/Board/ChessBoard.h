// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "Chess/Pieces/MoveInfo.h"
#include "Chess/Pieces/PieceInfo.h"
#include "Chess/Pieces/PieceBase.h"
#include "Chess/Board/ChessBoardCell.h"
#include "ChessBoard.generated.h"

class AChessPlayerBase;

/** Event to notify game continue */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameContinue);

/**	Event to notify game finished
 *	Finish codes:
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

	/** Create Pieces */
	void CreatePieces();

public:
	FORCEINLINE TArray<AChessBoardCell*> GetCells() const { return GameCells; };
	FORCEINLINE TArray<APieceBase*> GetTeam1ActivePieces() const { return Team1ActivePieces; }
	FORCEINLINE TArray<APieceBase*> GetTeam2ActivePieces() const { return Team2ActivePieces; }
	FORCEINLINE TArray<APieceBase*> GetTeam1BeatenPieces() const { return Team1BeatenPieces; }
	FORCEINLINE TArray<APieceBase*> GetTeam2BeatenPieces() const { return Team2BeatenPieces; }
	
	UFUNCTION()
		void MovePiece(APieceBase* Piece, FIntPoint Address);

	/** Calculate available moves of Pieces */
	TArray<FMoveInfo> CalculatePiecesMoves();
	
	AChessBoardCell* GetCellByAddress(FIntPoint Address);
	APieceBase* GetPieceByAddress(FIntPoint Address);

	/** Cleanup game board */
	void Cleanup();
	
protected:
	UPROPERTY()
		TArray<AChessBoardCell*> GameCells;

	UPROPERTY()
		TArray<APieceBase*> Team1ActivePieces;

	UPROPERTY()
		TArray<APieceBase*> Team2ActivePieces;

	UPROPERTY()
		TArray<AChessBoardCell*> CellsBeaten1;

	UPROPERTY()
		TArray<AChessBoardCell*> CellsBeaten2;

	UPROPERTY()
		TArray<APieceBase*> Team1BeatenPieces;

	UPROPERTY()
		TArray<APieceBase*> Team2BeatenPieces;

	/** Sets piece beaten and moves it to beaten cells */
	void SetPieceBeaten(APieceBase* Piece);

	/** Swaps pieces on provided cells */
	void SwapPiecesOnCells(AChessBoardCell* FirstCell, AChessBoardCell* SecondCell);

	/** Convert CellAddress to A-H 1-8 format */
	static FString CellAddressToHumanFormat(FIntPoint& Address);

	/** Convert A-H 1-8 format to CellAddress */
	static FIntPoint HumanFormatToCellAddress(FString& Info);

	/** Convert 1/2, K/Q/R/B/N/(P) and A-H 1-8 format to piece info */
	static FPieceInfo HumanFormatToPieceInfo(FString& Info);
	
	UPROPERTY()
		APieceBase* King1;

	UPROPERTY()
		APieceBase* King2;

private:
	//UPROPERTY()
	//	TMap<int32, TSubclassOf<class APieceBase>> InitialPiecesLocations;

	UPROPERTY()
		TArray<FString> StartingPositions;
};