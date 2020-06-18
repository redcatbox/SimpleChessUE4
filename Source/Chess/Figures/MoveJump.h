// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MoveBase.h"
#include "MoveJump.generated.h"

/** Jump move
 *	Ignores intermediate cells, immediately jumps to final one
 *	Includes empty cells and cells containing hostile figures
 */
UCLASS()
class CHESS_API UMoveJump : public UMoveBase
{
	GENERATED_BODY()

public:
	virtual TArray<FMoveResult> CalculateMoveResults(AFigureBase* Figure, FIntPoint CellAddress) override;

	UPROPERTY()
		FIntPoint Move;
};