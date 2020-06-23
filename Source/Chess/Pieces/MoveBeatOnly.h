// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MoveRegular.h"
#include "MoveBeatOnly.generated.h"

/**	Move only for beat
 *	Includes only cells containing hostile Pieces
 */
UCLASS()
class CHESS_API UMoveBeatOnly : public UMoveRegular
{
	GENERATED_BODY()

public:
	virtual TArray<UMoveInfo*> CalculateMoveInfos(APieceBase* Piece, FIntPoint CellAddress) override;
};
