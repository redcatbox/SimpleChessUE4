// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MoveCastling.h"
#include "PieceRook.h"
#include "Chess/Board/ChessBoard.h"

TArray<FMoveInfo> UMoveCastling::CalculateMoveInfos(APieceBase* Piece, FIntPoint CellAddress)
{
	TArray<FMoveInfo> Result;

	if (Piece)
	{
		if (!Piece->bIsMoved)
		{
			TArray<APieceBase*> ActivePieces = Piece->GameBoard->GetTeamActivePieces(Piece->GetTeamIndex());
			for (auto& AP : ActivePieces)
			{
				if (AP->GetClass() == APieceRook::StaticClass() && !AP->bIsMoved)
				{
					const ECastlingType RookSide = AP->CellAddress.X > Piece->CellAddress.X
						? ECastlingType::ECT_KingSide
						: ECastlingType::ECT_QueenSide;

					if (CastlingType == RookSide)
					{
						for (int32 Step = 1; Step <= MaxSteps; Step++)
						{
							const FIntPoint NewAddress = CellAddress + Direction * Step;
							APieceBase* OtherPiece = Piece->GameBoard->GetPieceByAddress(NewAddress);
							if (OtherPiece == AP)
							{
								FMoveInfo MoveInfo = FMoveInfo(0, NewAddress, Piece);
								Result.Add(MoveInfo);
							}
							else
							{
								return Result;
							}
						}
					}
				}
			}
		}
	}

	return Result;
}
