// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MoveOnly.h"
#include "MoveRegular.generated.h"

/**	Regular move
 *	Passed cell by cell in specified direction
 *	Passing empty cells and cells containing hostile Pieces
 */
UCLASS()
class CHESS_API UMoveRegular : public UMoveOnly
{
	GENERATED_BODY()

public:
	virtual TArray<FMoveInfo> CalculateMoveInfos(APieceBase* Piece, FIntPoint CellAddress) override;
};
