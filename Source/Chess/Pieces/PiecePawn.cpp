// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PiecePawn.h"
#include "MoveOnly.h"
#include "MoveFirstOnly.h"
#include "MoveBeatOnly.h"
#include "Chess/Board/ChessBoard.h"

APiecePawn::APiecePawn()
{
	FriendlyName = TEXT("Pawn");

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SMObj(TEXT("/Game/Pieces/SM_Cube_01.SM_Cube_01"));
	if (SMObj.Object)
	{
		StaticMeshComponent->SetStaticMesh(SMObj.Object);
	}

	// Define legal moves
	const FIntPoint Move1Dir = FIntPoint(0, 1);
	const FIntPoint Move3Dir = FIntPoint(1, 1);
	const FIntPoint Move4Dir = FIntPoint(-1, 1);

	UMoveOnly* Move1 = NewObject<UMoveOnly>();
	Move1->Direction = Move1Dir;
	Move1->MaxSteps = 1;

	UMoveFirstOnly* Move2 = NewObject<UMoveFirstOnly>();
	Move2->Direction = Move1Dir;
	Move2->MaxSteps = 2;

	UMoveBeatOnly* Move3 = NewObject<UMoveBeatOnly>();
	Move3->Direction = Move3Dir;
	Move3->MaxSteps = 1;

	UMoveBeatOnly* Move4 = NewObject<UMoveBeatOnly>();
	Move4->Direction = Move4Dir;
	Move4->MaxSteps = 1;

	LegalMoves.Add(Move1);
	LegalMoves.Add(Move2);
	LegalMoves.Add(Move3);
	LegalMoves.Add(Move4);
}