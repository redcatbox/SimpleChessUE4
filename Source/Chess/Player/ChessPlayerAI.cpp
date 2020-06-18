// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ChessPlayerAI.h"
#include "Chess/Board/ChessBoard.h"

void AChessPlayerAI::TriggerForMakeMove(bool bCondition)
{
	Super::TriggerForMakeMove(bCondition);

	if (bCondition)
	{
		FMoveResult Move = Search(0, true);

		if (Move.Figure)
		{
			MakeMove(Move);
		}
	}
}

FMoveResult AChessPlayerAI::Search(int32 Depth, bool bMax)
{
	if (Depth == 0)
	{
		return CalculateBestMove();
	}

	TArray<FMoveResult> MoveResults = GameBoard->CalculateFiguresMoves();
	FMoveResult Result;

	//if (bMax)
	//{
	//	// Team 2
	//	for (auto& MR : MoveResults)
	//	{
	//		//GameBoard;
	//		Result = FMath::Max(Result, Search(Depth - 1, bMax));
	//	}
	//}
	//else
	//{
	//	// Team 1
	//	for (auto& MR : MoveResults)
	//	{
	//		//GameBoard;
	//		Result = FMath::Min(Result, Search(Depth - 1, !bMax));
	//	}
	//}

	return Result;
}

FMoveResult AChessPlayerAI::CalculateBestMove()
{
	TArray<FMoveResult> AllMoves = GameBoard->CalculateFiguresMoves();
	TArray<FMoveResult> TeamMoves;

	for (auto& MR : AllMoves)
	{
		if (MR.Figure->GetTeamIndex() == TeamIndex)
		{
			TeamMoves.Add(MR);
		}
	}

	// Process all move results and find best result
	FMoveResult BestMoveResult;
	for (auto& MR : TeamMoves)
	{
		if (BestMoveResult <= MR)
		{
			BestMoveResult = MR;
		}
	}

	// Check is there are similar results
	TArray<FMoveResult> SimilarResults;
	for (auto& MR : TeamMoves)
	{
		if (BestMoveResult == MR)
		{
			SimilarResults.Add(MR);
		}
	}

	// Get one randomly
	if (SimilarResults.Num() > 0)
	{
		const int32 Index = FMath::RandRange(0, SimilarResults.Num() - 1);
		BestMoveResult = SimilarResults[Index];
	}

	//if (BestMoveResult.Figure)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("BestMoveResult = %d"), BestMoveResult.Value);
	//}

	return BestMoveResult;
}