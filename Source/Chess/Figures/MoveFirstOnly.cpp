// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MoveFirstOnly.h"
#include "FigureBase.h"

TArray<FMoveResult> UMoveFirstOnly::CalculateMoveResults(AFigureBase* Figure, FIntPoint CellAddress)
{
	TArray<FMoveResult> Result;

	if (Figure)
	{
		if (!Figure->bIsMoved)
		{
			Result = Super::CalculateMoveResults(Figure, CellAddress);
		}
	}

	return Result;
}