// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MoveFirstOnly.h"
#include "PieceBase.h"

TArray<UMoveInfo*> UMoveFirstOnly::CalculateMoveInfos(APieceBase* Piece, FIntPoint CellAddress)
{
	TArray<UMoveInfo*> Result;

	if (Piece)
	{
		if (!Piece->bIsMoved)
		{
			Result = Super::CalculateMoveInfos(Piece, CellAddress);
		}
	}

	return Result;
}
