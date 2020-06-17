// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ChessBoardCell.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
//#include "DrawDebugHelpers.h"

AChessBoardCell::AChessBoardCell()
{
	//PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PSHObj(TEXT("/Game/UI/PS_CellHighlight"));
	if (PSHObj.Object) { HighlightPS = PSHObj.Object; }
	HighlightPSC = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HighlightPSC"));
	HighlightPSC->SetupAttachment(RootComponent);
	HighlightPSC->bAutoActivate = false;
	HighlightPSC->Template = HighlightPS;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PSSObj(TEXT("/Game/UI/PS_CellSelect"));
	if (PSSObj.Object) { SelectPS = PSSObj.Object; }
	SelectPSC = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SelectPSC"));
	SelectPSC->SetupAttachment(RootComponent);
	SelectPSC->bAutoActivate = false;
	SelectPSC->Template = SelectPS;
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PSHMObj(TEXT("/Game/UI/PS_CellHighlightMove"));
	if (PSHMObj.Object) { HighlightMovePS = PSHMObj.Object;	}
	HighlightMovePSC = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HighlightMovePSC"));
	HighlightMovePSC->SetupAttachment(RootComponent);
	HighlightMovePSC->bAutoActivate = false;
	HighlightMovePSC->Template = HighlightMovePS;

	bIsNonGameCell = false;
}

//void AChessBoardCell::Tick(float DeltaSeconds)
//{
//	DrawDebugString(GetWorld(), FVector::ZeroVector, CellAddress.ToString(), this, FColor::White, 0.f, false, 1.f);
//}

void AChessBoardCell::SetCellSize(float Size) const
{
	BoxComponent->SetBoxExtent(FVector(Size / 2, Size / 2, 25.f));
}

void AChessBoardCell::CellClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	Select(true);
}

void AChessBoardCell::OnFingerPressedCell(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	Select(true);
}

void AChessBoardCell::Select(bool bSel)
{
	if (bSel)
	{
		SelectPSC->Activate();
	}
	else
	{
		SelectPSC->DeactivateSystem();
	}
}

void AChessBoardCell::Highlight(bool bOn)
{
	if (bOn)
	{
		HighlightPSC->Activate();
	}
	else
	{
		HighlightPSC->DeactivateSystem();
	}
}

void AChessBoardCell::HighlightMove(bool bOn)
{
	if (bOn)
	{
		HighlightMovePSC->Activate();
	}
	else
	{
		HighlightMovePSC->DeactivateSystem();
	}
}