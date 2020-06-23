// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ChessPlayerAI.h"
#include "Chess/Board/ChessBoard.h"

void AChessPlayerAI::TriggerForMakeMove(bool bCondition)
{
	Super::TriggerForMakeMove(bCondition);

	if (bCondition)
	{
		FMoveInfo Move = Search(0, true);

		if (Move.Piece)
		{
			MakeMove(Move);
		}
	}
}

FMoveInfo AChessPlayerAI::Search(int32 Depth, bool bMax)
{
	if (Depth == 0)
	{
		return CalculateBestMove();
	}

	TArray<FMoveInfo> MoveInfos = GameBoard->CalculatePiecesMoves();
	FMoveInfo Result;

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

FMoveInfo AChessPlayerAI::CalculateBestMove()
{
	TArray<FMoveInfo> AllMoves = GameBoard->CalculatePiecesMoves();
	TArray<FMoveInfo> TeamMoves;

	for (auto& MI : AllMoves)
	{
		if (MI.Piece->GetTeamIndex() == TeamIndex)
		{
			TeamMoves.Add(MI);
		}
	}

	// Process all move results and find best result
	FMoveInfo BestMoveInfo;
	for (auto& MI : TeamMoves)
	{
		if (BestMoveInfo <= MI)
		{
			BestMoveInfo = MI;
		}
	}

	// Check is there are similar results
	TArray<FMoveInfo> SimilarResults;
	for (auto& MI : TeamMoves)
	{
		if (BestMoveInfo == MI)
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

	//if (BestMoveInfo.Piece)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("BestMoveInfo = %d"), BestMoveInfo.Value);
	//}

	return BestMoveInfo;
}
