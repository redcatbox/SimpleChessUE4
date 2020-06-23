// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MoveCastling.h"
#include "PieceRook.h"
#include "Chess/Board/ChessBoard.h"

TArray<UMoveInfo*> UMoveCastling::CalculateMoveInfos(APieceBase* Piece, FIntPoint CellAddress)
{
	TArray<UMoveInfo*> Result;

	if (Piece)
	{
		if (!Piece->bIsMoved)
		{
			TArray<APieceBase*> ActivePieces = Piece->GameBoard->GetTeamActivePieces(Piece->GetTeamIndex());
			for (auto& AP : ActivePieces)
			{
				if (AP->GetClass() == APieceRook::StaticClass() && !AP->bIsMoved)
				{
					const bool bKingSide = AP->CellAddress.X > Piece->CellAddress.X
						? true
						: false;

					if (bKingSide)
					{
						// King side castling
						const FIntPoint Direction = FIntPoint(1, 0);
						const int32 MaxSteps = 3;

						for (int32 Step = 1; Step <= MaxSteps; Step++)
						{
							const FIntPoint NewAddress = CellAddress + Direction * Step;
							APieceBase* OtherPiece = Piece->GameBoard->GetPieceByAddress(NewAddress);
							if (OtherPiece)
							{
								if (OtherPiece == AP)
								{
									UMoveInfo* MoveInfo = NewObject<UMoveInfo>();
									MoveInfo->Value = 0;
									MoveInfo->CellAddress = NewAddress;
									MoveInfo->Piece = Piece;
									Result.Add(MoveInfo);
								}
								else
								{
									return Result;
								}
							}
						}
					}
					else
					{
						// Queen side castling
						const FIntPoint Direction = FIntPoint(-1, 0);
						const int32 MaxSteps = 4;

						for (int32 Step = 1; Step <= MaxSteps; Step++)
						{
							const FIntPoint NewAddress = CellAddress + Direction * Step;
							APieceBase* OtherPiece = Piece->GameBoard->GetPieceByAddress(NewAddress);
							if (OtherPiece)
							{
								if (OtherPiece == AP)
								{
									UMoveInfo* MoveInfo = NewObject<UMoveInfo>();
									MoveInfo->Value = 0;
									MoveInfo->CellAddress = NewAddress;
									MoveInfo->Piece = Piece;
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
	}

	return Result;
}
