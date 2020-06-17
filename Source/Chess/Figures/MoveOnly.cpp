// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MoveOnly.h"
#include "ChessFigureBase.h"
#include "Chess/Board/ChessBoardCell.h"
#include "Chess/Board/ChessBoard.h"

TArray<FMoveResult> UMoveOnly::CalculateMoveResults(AChessFigureBase* Figure, FIntPoint CellAddress)
{
	TArray<FMoveResult> Result;

	if (Figure)
	{
		FIntPoint ReverseDir = FIntPoint(1, 1);
		if (Figure->GetTeamIndex() == 2)
		{
			ReverseDir = FIntPoint(1, -1);
		}

		for (int32 Step = 1; Step <= MaxSteps; Step++)
		{
			const FIntPoint NewAddress = CellAddress + Direction * Step * ReverseDir;
			AChessBoardCell* NextCell = Figure->GameBoard->GetCellByAddress(NewAddress);
			if (NextCell)
			{
				AChessFigureBase* OtherFigure = Figure->GameBoard->GetFigureByAddress(NewAddress);
				if (OtherFigure)
				{
					break;
				}
				else
				{
					FMoveResult MoveResult = FMoveResult(0, NewAddress, Figure);
					Result.Add(MoveResult);
				}
			}
			else
			{
				break;
			}
		}
	}

	return Result;
}