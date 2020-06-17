// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MoveRegular.h"
#include "ChessFigureBase.h"
#include "Chess/Board/ChessBoardCell.h"
#include "Chess/Board/ChessBoard.h"

TArray<FMoveResult> UMoveRegular::CalculateMoveResults(AChessFigureBase* Figure, FIntPoint CellAddress)
{
	TArray<FMoveResult> Result;

	if (Figure)
	{
		for (int32 Step = 1; Step <= MaxSteps; Step++)
		{
			const FIntPoint NewAddress = CellAddress + Direction * Step;
			AChessBoardCell* NextCell = Figure->GameBoard->GetCellByAddress(NewAddress);
			if (NextCell)
			{
				AChessFigureBase* OtherFigure = Figure->GameBoard->GetFigureByAddress(NewAddress);
				if (OtherFigure)
				{
					if (Figure->GetTeamIndex() != OtherFigure->GetTeamIndex())
					{
						FMoveResult MoveResult = FMoveResult(OtherFigure->GetFigureValue(), NewAddress, Figure);
						Result.Add(MoveResult);
						break;
					}
					else
					{
						break;
					}
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