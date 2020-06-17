// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "MoveResult.generated.h"

class AChessBoardCell;
class AChessFigureBase;

USTRUCT()
struct FMoveResult
{
	GENERATED_BODY()

	UPROPERTY()
		int32 Value;

	UPROPERTY()
		FIntPoint CellAddress;

	UPROPERTY()
		AChessFigureBase* Figure;

	FMoveResult()
	{
		this->Value = 0;
		this->CellAddress = FIntPoint(-1, -1);
		this->Figure = nullptr;
	}

	FMoveResult(int32 Value, FIntPoint CellAddress, AChessFigureBase* Figure)
	{
		this->Value = Value;
		this->CellAddress = CellAddress;
		this->Figure = Figure;
	}
	
	FORCEINLINE bool operator==(const FMoveResult &Other) const
	{
		return Value == Other.Value;
	}

	FORCEINLINE bool operator<=(const FMoveResult &Other) const
	{
		return Value <= Other.Value;
	}

	FORCEINLINE bool operator>=(const FMoveResult &Other) const
	{
		return Value >= Other.Value;
	}
};