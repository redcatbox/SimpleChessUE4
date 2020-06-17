// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Pawn.h"
#include "Chess/Figures/MoveResult.h"
#include "ChessPlayerBase.generated.h"

class AChessFigureBase;
class AChessBoardCell;

/** Event to notify that move was performed */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMovePerformed);

/**	Event to notify game finished
 *	Win codes:
 *	0 - Checkmate
 *	1 - Stalemate
 *	2 - Resign
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameFinished, AChessPlayerBase*, WinPlayer, int32, WinCode);


UCLASS()
class CHESS_API AChessPlayerBase : public APawn
{
	GENERATED_BODY()

public:
	AChessPlayerBase();

	UPROPERTY()
		int32 TeamIndex;

	UPROPERTY()
		class AChessBoard* GameBoard;

	UPROPERTY()
		bool bCanMakeMove;

	UFUNCTION()
		virtual void TriggerForMakeMove(bool bCondition);

	UFUNCTION()
		virtual void SelectFigure(AChessFigureBase* Figure);

	UFUNCTION()
		virtual void SelectCell(AChessBoardCell* Cell);

	UFUNCTION()
		virtual void PerformMove();

	/** OnMovePerformed event */
	UPROPERTY()
		FOnMovePerformed OnMovePerformed;

	/** OnGameFinished event */
	UPROPERTY()
		FOnGameFinished OnGameFinished;

	/** Calculate available moves of of figures */
	UFUNCTION()
		virtual TArray<FMoveResult> CalculateAvailableMoves();
	
	UFUNCTION(BlueprintCallable, Category = "ChessPlayer")
		virtual void Resign();
	
protected:
	UFUNCTION()
		virtual void MakeMove(FMoveResult Move);

	UPROPERTY()
		AChessFigureBase* SelectedFigure;

	UPROPERTY()
		AChessBoardCell* SelectedCell;

	UPROPERTY()
		TArray<AChessBoardCell*> CellsToTurnOff;
};