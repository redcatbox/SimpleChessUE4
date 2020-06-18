// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MoveJump.h"
#include "FigureBase.h"
#include "Chess/Board/ChessBoardCell.h"
#include "Chess/Board/ChessBoard.h"

TArray<FMoveResult> UMoveJump::CalculateMoveResults(AFigureBase* Figure, FIntPoint CellAddress)
{
	TArray<FMoveResult> Result;

	if (Figure)
	{
		const FIntPoint NewAddress = CellAddress + Move;
		AChessBoardCell* NextCell = Figure->GameBoard->GetCellByAddress(NewAddress);
		if (NextCell)
		{
			AFigureBase* OtherFigure = Figure->GameBoard->GetFigureByAddress(NewAddress);
			if (OtherFigure)
			{
				if (Figure->GetTeamIndex() != OtherFigure->GetTeamIndex())
				{
					FMoveResult MoveResult = FMoveResult(OtherFigure->GetFigureValue(), NewAddress, Figure);
					Result.Add(MoveResult);
				}
			}
			else
			{
				FMoveResult MoveResult = FMoveResult(0, NewAddress, Figure);
				Result.Add(MoveResult);
			}
		}
	}

	return Result;
}