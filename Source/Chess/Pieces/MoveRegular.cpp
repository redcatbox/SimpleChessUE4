// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MoveRegular.h"
#include "PieceBase.h"
#include "Chess/Board/ChessBoardCell.h"
#include "Chess/Board/ChessBoard.h"

TArray<UMoveInfo*> UMoveRegular::CalculateMoveInfos(APieceBase* Piece, FIntPoint CellAddress)
{
	TArray<UMoveInfo*> Result;

	if (Piece)
	{
		for (int32 Step = 1; Step <= MaxSteps; Step++)
		{
			const FIntPoint NewAddress = CellAddress + Direction * Step;
			AChessBoardCell* NextCell = Piece->GameBoard->GetCellByAddress(NewAddress);
			if (NextCell)
			{
				APieceBase* OtherPiece = Piece->GameBoard->GetPieceByAddress(NewAddress);
				if (OtherPiece)
				{
					if (Piece->GetTeamIndex() != OtherPiece->GetTeamIndex())
					{
						UMoveInfo* MoveInfo = NewObject<UMoveInfo>();
						MoveInfo->Value = OtherPiece->GetPieceValue();
						MoveInfo->CellAddress = NewAddress;
						MoveInfo->Piece = Piece;
						Result.Add(MoveInfo);
						break;
					}
					else
					{
						break;
					}
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
