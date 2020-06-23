// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MoveBase.h"
#include "MoveCastling.generated.h"

/** Castling move */
UCLASS()
class CHESS_API UMoveCastling : public UMoveBase
{
	GENERATED_BODY()

public:
	virtual TArray<UMoveInfo*> CalculateMoveInfos(APieceBase* Piece, FIntPoint CellAddress) override;
};
