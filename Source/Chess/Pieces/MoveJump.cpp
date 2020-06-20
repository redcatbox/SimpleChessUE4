// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MoveJump.h"
#include "PieceBase.h"
#include "Chess/Board/ChessBoardCell.h"
#include "Chess/Board/ChessBoard.h"

TArray<FMoveInfo> UMoveJump::CalculateMoveInfos(APieceBase* Piece, FIntPoint CellAddress)
{
	TArray<FMoveInfo> Result;

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
					FMoveInfo MoveInfo = FMoveInfo(OtherPiece->GetPieceValue(), NewAddress, Piece);
					Result.Add(MoveInfo);
				}
			}
			else
			{
				FMoveInfo MoveInfo = FMoveInfo(0, NewAddress, Piece);
				Result.Add(MoveInfo);
			}
		}
	}

	return Result;
}