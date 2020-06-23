// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Pawn.h"
#include "Chess/Pieces/MoveInfo.h"
#include "ChessPlayerBase.generated.h"

class APieceBase;
class AChessBoardCell;

/** Event to notify that move was performed */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMovePerformed);

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
		virtual void SelectPiece(APieceBase* Piece);

	UFUNCTION()
		virtual void SelectCell(AChessBoardCell* Cell);

	UFUNCTION()
		virtual void PerformMove();

	/** OnMovePerformed event */
	UPROPERTY()
		FOnMovePerformed OnMovePerformed;
	
	UFUNCTION(BlueprintCallable, Category = "ChessPlayer")
		virtual void Resign();
	
protected:
	UFUNCTION()
		virtual void MakeMove(FMoveInfo Move);

	UPROPERTY()
		APieceBase* SelectedPiece;

	UPROPERTY()
		AChessBoardCell* SelectedCell;

	UPROPERTY()
		TArray<AChessBoardCell*> CellsToTurnOff;
};
