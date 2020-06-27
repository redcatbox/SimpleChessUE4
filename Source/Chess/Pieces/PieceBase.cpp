// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PieceBase.h"
#include "Chess/Board/ChessBoardCell.h"
#include "MoveRegular.h"

#include "PieceBishop.h"
#include "PieceKing.h"
#include "PieceKnight.h"
#include "PiecePawn.h"
#include "PieceQueen.h"
#include "PieceRook.h"

APieceBase::APieceBase()
{
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;
	StaticMeshComponent->SetMobility(EComponentMobility::Movable);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Structure to hold one-time initialization for materials
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> Team1Mat;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> Team2Mat;
		FConstructorStatics()
			: Team1Mat(TEXT("/Game/Pieces/MI_Team1.MI_Team1"))
			, Team2Mat(TEXT("/Game/Pieces/MI_Team2.MI_Team2"))
		{}
	};
	static FConstructorStatics ConstructorStatics;

	TeamsMaterials.Add(ConstructorStatics.Team1Mat.Get());
	TeamsMaterials.Add(ConstructorStatics.Team2Mat.Get());

	TeamIndex = -1;
	bIsMoved = false;
	bIsBeaten = false;

	bMovePiece = false;
	Alpha = 0.f;
	bNotifyMoveAnimFinished = false;
}

void APieceBase::SetTeam(int32 TeamId)
{
	TeamIndex = TeamId;
	SetMaterialByTeam(TeamId);
}

void APieceBase::SetMaterialByTeam(int32 TeamId)
{
	if (TeamsMaterials.Num() >= TeamIndex)
	{
		StaticMeshComponent->SetMaterial(0, TeamsMaterials[TeamId - 1]);
	}
}

bool APieceBase::CheckIsCellReachable(FIntPoint& Address)
{
	if (CellsAvailableToMove.Contains(Address))
	{
		return true;
	}

	return false;
}

void APieceBase::CalculateMovesResults()
{
	// Calculate moves
	MoveInfos.Empty(); 

	for (auto& Move : LegalMoves)
	{
		TArray<UMoveInfo*> MI = Move->CalculateMoveInfos(this, CellAddress);
		MoveInfos.Append(MI);
	}

	// Get cells from calculated moves
	CellsAvailableToMove.Empty();

	for (auto& MI : MoveInfos)
	{
		CellsAvailableToMove.Add(MI->CellAddress);
	}
}

void APieceBase::Tick(float DeltaSeconds)
{
	if (bMovePiece)
	{
		Alpha += DeltaSeconds;
		if (Alpha >= 1.f)
		{
			Alpha = 1.f;
		}

		SetActorLocation(FMath::Lerp(MoveFrom, MoveTo, Alpha));

		if (Alpha == 1.f)
		{
			Alpha = 0.f;
			bMovePiece = false;
			
			if (bNotifyMoveAnimFinished)
			{
				OnPieceMoveAnimFinished.Broadcast();
			}
		}
	}
}

void APieceBase::PlayMovePieceAnim(FVector LocFrom, FVector LocTo, bool bNotifyWhenFinished)
{
	bMovePiece = true;
	MoveFrom = LocFrom;
	MoveTo = LocTo;
	bNotifyMoveAnimFinished = bNotifyWhenFinished;
}

int32 APieceBase::GetPieceValue()
{
	int32 Result = 0;

	if (GetClass() == APiecePawn::StaticClass())
	{
		Result = 10;
	}
	else if (GetClass() == APieceKnight::StaticClass())
	{
		Result = 30;
	}
	else if (GetClass() == APieceBishop::StaticClass())
	{
		Result = 35;
	}
	else if (GetClass() == APieceRook::StaticClass())
	{
		Result = 50;
	}
	else if (GetClass() == APieceQueen::StaticClass())
	{
		Result = 90;
	}
	else if (GetClass() == APieceKing::StaticClass())
	{
		Result = 900;
	}

	if (TeamIndex == 2)
	{
		Result = -Result;
	}

	return Result;
}
