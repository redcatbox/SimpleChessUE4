// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MoveOnly.h"
#include "PieceBase.h"
#include "Chess/Board/ChessBoardCell.h"
#include "Chess/Board/ChessBoard.h"

TArray<UMoveInfo*> UMoveOnly::CalculateMoveInfos(APieceBase* Piece, FIntPoint CellAddress)
{
	TArray<UMoveInfo*> Result;

	if (Piece)
	{
		FIntPoint ReverseDir = FIntPoint(1, 1);
		if (Piece->GetTeamIndex() == 2)
		{
			ReverseDir = FIntPoint(1, -1);
		}

		for (int32 Step = 1; Step <= MaxSteps; Step++)
		{
			const FIntPoint NewAddress = CellAddress + Direction * Step * ReverseDir;
			AChessBoardCell* NextCell = Piece->GameBoard->GetCellByAddress(NewAddress);
			if (NextCell)
			{
				APieceBase* OtherPiece = Piece->GameBoard->GetPieceByAddress(NewAddress);
				if (OtherPiece)
				{
					break;
				}
				else
				{
					UMoveInfo* MoveInfo = NewObject<UMoveInfo>();
					MoveInfo->Value = 0;
					MoveInfo->CellAddress = NewAddress;
					MoveInfo->Piece = Piece;
					Result.Add(MoveInfo);
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
