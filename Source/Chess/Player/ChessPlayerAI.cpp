// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ChessPlayerAI.h"
#include "Chess/Board/ChessBoard.h"

void AChessPlayerAI::TriggerForMakeMove(bool bCondition)
{
	Super::TriggerForMakeMove(bCondition);

	if (bCondition)
	{
		UMoveInfo* MoveInfo = Search(0, true);
		if (MoveInfo)
		{
			MakeMove(MoveInfo);
		}
	}
}

UMoveInfo* AChessPlayerAI::Search(int32 Depth, bool bMax)
{
	if (Depth == 0)
	{
		return CalculateBestMove();
	}

	TArray<UMoveInfo*> MoveInfos = GameBoard->CalculatePiecesMoves();
	UMoveInfo* Result = nullptr;

	//if (bMax)
	//{
	//	// Team 2
	//	for (auto& MI : MoveInfos)
	//	{
	//		//GameBoard;
	//		Result = FMath::Max(Result, Search(Depth - 1, bMax));
	//	}
	//}
	//else
	//{
	//	// Team 1
	//	for (auto& MI : MoveInfos)
	//	{
	//		//GameBoard;
	//		Result = FMath::Min(Result, Search(Depth - 1, !bMax));
	//	}
	//}

	return Result;
}

UMoveInfo* AChessPlayerAI::CalculateBestMove()
{
	TArray<UMoveInfo*> AllMoves = GameBoard->CalculatePiecesMoves();
	TArray<UMoveInfo*> TeamMoves;

	for (auto& MI : AllMoves)
	{
		if (MI->Piece->GetTeamIndex() == TeamIndex)
		{
			TeamMoves.Add(MI);
		}
	}

	// Process all move results and find best result
	UMoveInfo* BestMoveInfo = nullptr;
	for (auto& MI : TeamMoves)
	{
		if (BestMoveInfo)
		{
			if (BestMoveInfo->Value <= MI->Value)
			{
				BestMoveInfo = MI;
			}
		}
		else
		{
			BestMoveInfo = MI;
		}
	}

	// Check is there are similar results
	TArray<UMoveInfo*> SimilarResults;
	for (auto& MI : TeamMoves)
	{
		if (BestMoveInfo->Value == MI->Value)
		{
			SimilarResults.Add(MI);
		}
	}

	// Get one randomly
	if (SimilarResults.Num() > 0)
	{
		const int32 Index = FMath::RandRange(0, SimilarResults.Num() - 1);
		BestMoveInfo = SimilarResults[Index];
	}

	return BestMoveInfo;
}
