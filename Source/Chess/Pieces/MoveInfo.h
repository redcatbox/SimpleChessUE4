// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "MoveInfo.generated.h"

class AChessBoardCell;
class APieceBase;

USTRUCT()
struct FMoveInfo
{
	GENERATED_BODY()

	UPROPERTY()
		int32 Value;

	UPROPERTY()
		FIntPoint CellAddress;

	UPROPERTY()
		APieceBase* Piece;

	FMoveInfo()
	{
		this->Value = 0;
		this->CellAddress = FIntPoint(-1, -1);
		this->Piece = nullptr;
	}

	FMoveInfo(int32 Value, FIntPoint CellAddress, APieceBase* Piece)
	{
		this->Value = Value;
		this->CellAddress = CellAddress;
		this->Piece = Piece;
	}
	
	FORCEINLINE bool operator==(const FMoveInfo &Other) const
	{
		return Value == Other.Value;
	}

	FORCEINLINE bool operator<=(const FMoveInfo &Other) const
	{
		return Value <= Other.Value;
	}

	FORCEINLINE bool operator>=(const FMoveInfo &Other) const
	{
		return Value >= Other.Value;
	}
};