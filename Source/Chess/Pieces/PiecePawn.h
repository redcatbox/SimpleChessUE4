// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "PieceBase.h"
#include "PiecePawn.generated.h"

UCLASS()
class APiecePawn : public APieceBase
{
	GENERATED_BODY()

public:
	APiecePawn();

	/** Check is there are beaten pieces from this team and promote pawn */
	UFUNCTION()
		void PromotePawn();
};
