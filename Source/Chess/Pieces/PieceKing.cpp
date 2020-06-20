// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PieceKing.h"
#include "MoveRegular.h"

APieceKing::APieceKing()
{
	FriendlyName = TEXT("King");

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SMObj(TEXT("/Game/Pieces/SM_Cube_02.SM_Cube_02"));
	if (SMObj.Object)
	{
		StaticMeshComponent->SetStaticMesh(SMObj.Object);
	}

	// Define legal moves
	const FIntPoint Move1Dir = FIntPoint(1, 0);
	const FIntPoint Move2Dir = FIntPoint(-1, 0);
	const FIntPoint Move3Dir = FIntPoint(0, 1);
	const FIntPoint Move4Dir = FIntPoint(0, -1);
	const FIntPoint Move5Dir = FIntPoint(1, 1);
	const FIntPoint Move6Dir = FIntPoint(-1, 1);
	const FIntPoint Move7Dir = FIntPoint(1, -1);
	const FIntPoint Move8Dir = FIntPoint(-1, -1);
	const int32 MoveMaxSteps = 1;

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

	UMoveRegular* Move5 = NewObject<UMoveRegular>();
	Move5->Direction = Move5Dir;
	Move5->MaxSteps = MoveMaxSteps;

	UMoveRegular* Move6 = NewObject<UMoveRegular>();
	Move6->Direction = Move6Dir;
	Move6->MaxSteps = MoveMaxSteps;

	UMoveRegular* Move7 = NewObject<UMoveRegular>();
	Move7->Direction = Move7Dir;
	Move7->MaxSteps = MoveMaxSteps;

	UMoveRegular* Move8 = NewObject<UMoveRegular>();
	Move8->Direction = Move8Dir;
	Move8->MaxSteps = MoveMaxSteps;

	LegalMoves.Add(Move1);
	LegalMoves.Add(Move2);
	LegalMoves.Add(Move3);
	LegalMoves.Add(Move4);
	LegalMoves.Add(Move5);
	LegalMoves.Add(Move6);
	LegalMoves.Add(Move7);
	LegalMoves.Add(Move8);

	//Castling
	//Mirror for second team
}