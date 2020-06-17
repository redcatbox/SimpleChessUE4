// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "FigureBishop.h"
#include "MoveRegular.h"

AFigureBishop::AFigureBishop()
{
	FriendlyName = TEXT("Bishop");

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SMObj(TEXT("/Game/Figures/SM_Cube_03.SM_Cube_03"));
	if (SMObj.Object)
	{
		StaticMeshComponent->SetStaticMesh(SMObj.Object);
	}

	// Define legal moves
	const FIntPoint Move1Dir = FIntPoint(1, 1);
	const FIntPoint Move2Dir = FIntPoint(-1, 1);
	const FIntPoint Move3Dir = FIntPoint(1, -1);
	const FIntPoint Move4Dir = FIntPoint(-1, -1);
	const int32 MoveMaxSteps = 7;

	UMoveRegular* Move1 = NewObject<UMoveRegular>();
	Move1->Direction = Move1Dir;
	Move1->MaxSteps = MoveMaxSteps;

	UMoveRegular* Move2 = NewObject<UMoveRegular>();
	Move2->Direction = Move2Dir;
	Move2->MaxSteps = MoveMaxSteps;

	UMoveRegular* Move3 = NewObject<UMoveRegular>();
	Move3->Direction = Move3Dir;
	Move3->MaxSteps = MoveMaxSteps;

	UMoveRegular* Move4 = NewObject<UMoveRegular>();
	Move4->Direction = Move4Dir;
	Move4->MaxSteps = MoveMaxSteps;

	LegalMoves.Add(Move1);
	LegalMoves.Add(Move2);
	LegalMoves.Add(Move3);
	LegalMoves.Add(Move4);
}