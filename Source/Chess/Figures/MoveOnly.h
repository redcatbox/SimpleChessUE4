// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MoveBase.h"
#include "MoveOnly.generated.h"

/**	Only move
 *	Passing cell by cell in specified direction
 *	Only empty cells
 */
UCLASS()
class CHESS_API UMoveOnly : public UMoveBase
{
	GENERATED_BODY()

public:
	virtual TArray<FMoveResult> CalculateMoveResults(AChessFigureBase* Figure, FIntPoint CellAddress) override;

	UPROPERTY()
		FIntPoint Direction;

	UPROPERTY()
		int32 MaxSteps;
};