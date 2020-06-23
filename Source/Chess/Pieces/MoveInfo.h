// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "MoveInfo.generated.h"

class AChessBoardCell;
class APieceBase;

/**	Piece move result info */
UCLASS()
class CHESS_API UMoveInfo : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
		int32 Value;

	UPROPERTY()
		FIntPoint CellAddress;

	UPROPERTY()
		APieceBase* Piece;
};
