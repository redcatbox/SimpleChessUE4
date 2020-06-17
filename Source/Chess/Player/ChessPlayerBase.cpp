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
	if (SelectedFigure)
	{
		SelectedFigure->OnFigureMoveAnimFinished.RemoveDynamic(this, &AChessPlayerBase::PerformMove);
		SelectFigure(nullptr);
	}

	if (SelectedCell)
	{
		SelectCell(nullptr);
	}

	OnMovePerformed.Broadcast();
}

void AChessPlayerBase::SelectFigure(AChessFigureBase* Figure)
{
	for (auto& Cell : CellsToTurnOff)
	{
		Cell->HighlightMove(false);
		Cell->Highlight(false);
		Cell->Select(false);
	}

	CellsToTurnOff.Empty();
	SelectedFigure = Figure;

	if (SelectedFigure)
	{
		AChessBoardCell* Cell = GameBoard->GetCellByAddress(SelectedFigure->CellAddress);
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

void AChessPlayerBase::MakeMove(FMoveResult Move)
{
	if (Move.Figure)
	{
		SelectFigure(Move.Figure);

		AChessBoardCell* Cell = GameBoard->GetCellByAddress(Move.CellAddress);
		if (Cell)
		{
			SelectCell(Cell);
		}

		GameBoard->MoveFigure(Move.Figure, Move.CellAddress);
		SelectedFigure->OnFigureMoveAnimFinished.AddDynamic(this, &AChessPlayerBase::PerformMove);
	}
}

TArray<FMoveResult> AChessPlayerBase::CalculateAvailableMoves()
{
	TArray<FMoveResult> Result;
	return Result;
}

void AChessPlayerBase::Resign() {}