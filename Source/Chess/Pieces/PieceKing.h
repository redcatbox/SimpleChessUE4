// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "PieceBase.h"
#include "PieceKing.generated.h"

UCLASS()
class APieceKing : public APieceBase
{
	GENERATED_BODY()

public:
	APieceKing();
	void CalculateMovesResults() override;
};
