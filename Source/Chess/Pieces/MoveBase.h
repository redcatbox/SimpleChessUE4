// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "MoveInfo.h"
#include "MoveBase.generated.h"

class AChessBoardCell;
class APieceBase;

/**	Base abstract class for Piece move */
UCLASS(Abstract)
class CHESS_API UMoveBase : public UObject
{
	GENERATED_BODY()

public:
	/** Calculates array of move results with move values and cells */
	UFUNCTION()
		virtual TArray<FMoveInfo> CalculateMoveInfos(APieceBase* Piece, FIntPoint CellAddress);
};
