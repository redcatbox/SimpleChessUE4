// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "FigureKnight.h"
#include "MoveJump.h"

AFigureKnight::AFigureKnight()
{
	FriendlyName = TEXT("Knight");

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SMObj(TEXT("/Game/Figures/SM_Pyramid_01.SM_Pyramid_01"));
	if (SMObj.Object)
	{
		StaticMeshComponent->SetStaticMesh(SMObj.Object);
	}

	// Define legal moves
	const FIntPoint Move1Val = FIntPoint(1, 2);
	const FIntPoint Move2Val = FIntPoint(2, 1);
	const FIntPoint Move3Val = FIntPoint(2, -1);
	const FIntPoint Move4Val = FIntPoint(1, -2);
	const FIntPoint Move5Val = FIntPoint(-1, -2);
	const FIntPoint Move6Val = FIntPoint(-2, -1);
	const FIntPoint Move7Val = FIntPoint(-2, 1);
	const FIntPoint Move8Val = FIntPoint(-1, 2);

	UMoveJump* Move1 = NewObject<UMoveJump>();
	Move1->Move = Move1Val;

	UMoveJump* Move2 = NewObject<UMoveJump>();
	Move2->Move = Move2Val;

	UMoveJump* Move3 = NewObject<UMoveJump>();
	Move3->Move = Move3Val;

	UMoveJump* Move4 = NewObject<UMoveJump>();
	Move4->Move = Move4Val;

	UMoveJump* Move5 = NewObject<UMoveJump>();
	Move5->Move = Move5Val;

	UMoveJump* Move6 = NewObject<UMoveJump>();
	Move6->Move = Move6Val;

	UMoveJump* Move7 = NewObject<UMoveJump>();
	Move7->Move = Move7Val;

	UMoveJump* Move8 = NewObject<UMoveJump>();
	Move8->Move = Move8Val;

	LegalMoves.Add(Move1);
	LegalMoves.Add(Move2);
	LegalMoves.Add(Move3);
	LegalMoves.Add(Move4);
	LegalMoves.Add(Move5);
	LegalMoves.Add(Move6);
	LegalMoves.Add(Move7);
	LegalMoves.Add(Move8);
}