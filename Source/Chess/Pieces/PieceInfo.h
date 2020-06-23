// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PieceInfo.generated.h"

class APieceBase;

USTRUCT()
struct FPieceInfo
{
	GENERATED_BODY()
	
	UPROPERTY()
		int32 TeamId;

	UPROPERTY()
		TSubclassOf<APieceBase> PieceClass;

	UPROPERTY()
		FIntPoint CellAddress;

	FPieceInfo() {}
	FPieceInfo(int32 TeamId, TSubclassOf<APieceBase> PieceClass, FIntPoint CellAddress)
	{
		this->TeamId = TeamId;
		this->PieceClass = PieceClass;
		this->CellAddress = CellAddress;
	}

	FString ToString() const
	{
		return FString::Printf(TEXT("TeamId = %d, PieceClass = %s, CellAddress = %s"), TeamId, PieceClass, *CellAddress.ToString());
	}
};
