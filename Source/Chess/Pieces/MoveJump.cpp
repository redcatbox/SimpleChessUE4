// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MoveJump.h"
#include "PieceBase.h"
#include "Chess/Board/ChessBoardCell.h"
#include "Chess/Board/ChessBoard.h"

TArray<UMoveInfo*> UMoveJump::CalculateMoveInfos(APieceBase* Piece, FIntPoint CellAddress)
{
	TArray<UMoveInfo*> Result;

	if (Piece)
	{
		const FIntPoint NewAddress = CellAddress + Move;
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
	}

	return Result;
}
