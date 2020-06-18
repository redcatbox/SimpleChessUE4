// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ChessPlayerBase.h"
#include "ChessPlayerAI.generated.h"

UCLASS()
class CHESS_API AChessPlayerAI : public AChessPlayerBase
{
	GENERATED_BODY()

public:
	virtual void TriggerForMakeMove(bool bCondition) override;
	
	/** Calculate best move */
	UFUNCTION()
		FMoveResult CalculateBestMove();
	
	/** Recursive search */
	UFUNCTION()
		FMoveResult Search(int32 Depth, bool bMax);
};