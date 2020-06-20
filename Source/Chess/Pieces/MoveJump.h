// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MoveBase.h"
#include "MoveJump.generated.h"

/** Jump move
 *	Ignores intermediate cells, immediately jumps to final one
 *	Includes empty cells and cells containing hostile Pieces
 */
UCLASS()
class CHESS_API UMoveJump : public UMoveBase
{
	GENERATED_BODY()

public:
	virtual TArray<FMoveInfo> CalculateMoveInfos(APieceBase* Piece, FIntPoint CellAddress) override;

	UPROPERTY()
		FIntPoint Move;
};