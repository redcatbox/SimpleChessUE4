// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ChessPlayerBase.h"
#include "Chess/Board/ChessBoard.h"

AChessPlayerBase::AChessPlayerBase()
{
	TeamIndex = -1;
	bCanMakeMove = false;
}

void AChessPlayerBase::TriggerForMakeMove(bool bCondition)
{
	bCanMakeMove = bCondition;
}

void AChessPlayerBase::PerformMove()
{
	if (SelectedPiece)
	{
		SelectedPiece->OnPieceMoveAnimFinished.RemoveDynamic(this, &AChessPlayerBase::PerformMove);
		SelectPiece(nullptr);
	}

	if (SelectedCell)
	{
		SelectCell(nullptr);
	}

	OnMovePerformed.Broadcast();
}

void AChessPlayerBase::SelectPiece(APieceBase* Piece)
{
	for (auto& Cell : CellsToTurnOff)
	{
		Cell->HighlightMove(false);
		Cell->Highlight(false);
		Cell->Select(false);
	}

	CellsToTurnOff.Empty();
	SelectedPiece = Piece;

	if (SelectedPiece)
	{
		AChessBoardCell* Cell = GameBoard->GetCellByAddress(SelectedPiece->CellAddress);
		if (Cell)
		{
			Cell->Highlight(false);
			Cell->Select(true);
			CellsToTurnOff.Add(Cell);
		}
	}
}

void AChessPlayerBase::SelectCell(AChessBoardCell* Cell)
{
	if (SelectedCell)
	{
		SelectedCell->Highlight(false);
		SelectedCell->Select(false);
	}

	SelectedCell = Cell;

	if (SelectedCell)
	{
		SelectedCell->Select(true);
		CellsToTurnOff.Add(SelectedCell);
	}
}

void AChessPlayerBase::MakeMove(FMoveInfo Move)
{
	if (Move.Piece)
	{
		SelectPiece(Move.Piece);

		AChessBoardCell* Cell = GameBoard->GetCellByAddress(Move.CellAddress);
		if (Cell)
		{
			SelectCell(Cell);
		}

		GameBoard->MovePiece(Move.Piece, Move.CellAddress);
		SelectedPiece->OnPieceMoveAnimFinished.AddDynamic(this, &AChessPlayerBase::PerformMove);
	}
}

void AChessPlayerBase::Resign() {}
