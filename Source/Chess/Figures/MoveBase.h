// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "MoveResult.h"
#include "MoveBase.generated.h"

class AChessBoardCell;
class AChessFigureBase;

/**	Base abstract class for figure move */
UCLASS(Abstract)
class CHESS_API UMoveBase : public UObject
{
	GENERATED_BODY()

public:
	/** Calculates array of move results with move values and cells */
	UFUNCTION()
		virtual TArray<FMoveResult> CalculateMoveResults(AChessFigureBase* Figure, FIntPoint CellAddress);
};