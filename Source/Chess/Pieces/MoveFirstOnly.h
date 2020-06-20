// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MoveOnly.h"
#include "MoveFirstOnly.generated.h"

/** The same as MoveOnly, but if Piece never has been moved before */
UCLASS()
class CHESS_API UMoveFirstOnly : public UMoveOnly
{
	GENERATED_BODY()

public:
	virtual TArray<FMoveInfo> CalculateMoveInfos(APieceBase* Piece, FIntPoint CellAddress) override;
};