// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MoveRegular.h"
#include "MoveBeatOnly.generated.h"

/**	Move only for beat
 *	Includes only cells containing hostile figures
 */
UCLASS()
class CHESS_API UMoveBeatOnly : public UMoveRegular
{
	GENERATED_BODY()

public:
	virtual TArray<FMoveResult> CalculateMoveResults(AFigureBase* Figure, FIntPoint CellAddress) override;
};