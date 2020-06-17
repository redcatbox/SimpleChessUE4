// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MoveRegular.h"
#include "MoveCastling.generated.h"

/** Castling types */
UENUM()
enum class ECastlingType : uint8
{
	ECT_KingSide	UMETA(DisplayName = "KingSide"),
	ECT_QueenSide	UMETA(DisplayName = "QueenSide")
};

/** Castling move */
UCLASS()
class CHESS_API UMoveCastling : public UMoveBase
{
	GENERATED_BODY()

	UPROPERTY()
		ECastlingType CastlingType;
};