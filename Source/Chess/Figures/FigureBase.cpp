// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "FigureBase.h"
#include "Chess/Board/ChessBoardCell.h"
#include "MoveRegular.h"

#include "FigureBishop.h"
#include "FigureKing.h"
#include "FigureKnight.h"
#include "FigurePawn.h"
#include "FigureQueen.h"
#include "FigureRook.h"

AFigureBase::AFigureBase()
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
			: Team1Mat(TEXT("/Game/Figures/MI_Team1.MI_Team1"))
			, Team2Mat(TEXT("/Game/Figures/MI_Team2.MI_Team2"))
		{}
	};
	static FConstructorStatics ConstructorStatics;

	TeamsMaterials.Add(ConstructorStatics.Team1Mat.Get());
	TeamsMaterials.Add(ConstructorStatics.Team2Mat.Get());

	TeamIndex = -1;
	bIsMoved = false;
	bIsBeaten = false;

	bMoveFigure = false;
	Alpha = 0.f;
}

void AFigureBase::SetTeam(int32 TeamId)
{
	TeamIndex = TeamId;
	SetMaterialByTeam(TeamId);
}

void AFigureBase::SetMaterialByTeam(int32 TeamId)
{
	if (TeamsMaterials.Num() >= TeamIndex)
	{
		StaticMeshComponent->SetMaterial(0, TeamsMaterials[TeamId - 1]);
	}
}

bool AFigureBase::CheckIsCellReachable(FIntPoint Cell)
{
	if (CellsAvailableToMove.Contains(Cell))
	{
		return true;
	}

	return false;
}

void AFigureBase::CalculateMovesResults()
{
	MoveResults.Empty();

	for (auto& Move : LegalMoves)
	{
		TArray<FMoveResult> MR = Move->CalculateMoveResults(this, CellAddress);
		MoveResults.Append(MR);
	}

	CellsAvailableToMove.Empty();

	for (auto& MR : MoveResults)
	{
		CellsAvailableToMove.Add(MR.CellAddress);
	}
}

void AFigureBase::Tick(float DeltaSeconds)
{
	if (bMoveFigure)
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
			bMoveFigure = false;
			OnFigureMoveAnimFinished.Broadcast();
		}
	}
}

void AFigureBase::PlayMoveFigureAnim(FVector LocFrom, FVector LocTo)
{
	bMoveFigure = true;
	MoveFrom = LocFrom;
	MoveTo = LocTo;
}

int32 AFigureBase::GetFigureValue()
{
	int32 Result = 0;

	if (GetClass() == AFigurePawn::StaticClass())
	{
		Result = 10;
	}
	else if (GetClass() == AFigureBishop::StaticClass())
	{
		Result = 25;
	}
	else if (GetClass() == AFigureKnight::StaticClass())
	{
		Result = 50;
	}
	else if (GetClass() == AFigureRook::StaticClass())
	{
		Result = 75;
	}
	else if (GetClass() == AFigureQueen::StaticClass())
	{
		Result = 100;
	}
	else if (GetClass() == AFigureKing::StaticClass())
	{
		Result = 900;
	}

	if (TeamIndex == 2)
	{
		Result = -Result;
	}

	return Result;
}