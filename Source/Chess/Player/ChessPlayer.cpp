// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ChessPlayer.h"
#include "Chess/Board/ChessBoardCell.h"
#include "Chess/Board/ChessBoard.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

AChessPlayer::AChessPlayer()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	RootComponent = SpringArm;
	SpringArm->TargetArmLength = 2300.f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = false;
	bCameraRotation = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArm);
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->FieldOfView = 75.f;

	bCanTraceForCells = false;
}

void AChessPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Axis events
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// Actions
	PlayerInputComponent->BindAxis("Zoom", this, &AChessPlayer::CameraZoom);
	PlayerInputComponent->BindAction("Click", IE_Pressed, this, &AChessPlayer::TriggerClick);

	// Touch events
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AChessPlayer::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AChessPlayer::TouchStopped);
}

void AChessPlayer::CameraZoom(float Val)
{
	float Length = SpringArm->TargetArmLength - 100.f * Val;
	Length = FMath::Clamp(Length, 1500.f, 2500.f);
	SpringArm->TargetArmLength = Length;
}

void AChessPlayer::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (bCanMakeMove && bCanTraceForCells)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult HitResult;
			const ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_Visibility);
			PC->GetHitResultUnderFingerByChannel(FingerIndex, TraceType, true, HitResult);
			CheckTraceForCell(HitResult);
			TriggerClick();
		}
	}
}

void AChessPlayer::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location) {}

void AChessPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bCanMakeMove && bCanTraceForCells)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FVector Start, Dir, End;
			PC->DeprojectMousePositionToWorld(Start, Dir);
			End = Start + Dir * 5000.f;
			TraceForCell(Start, End);
		}
	}
}

/**	Trace for cells
 *	If nothing selected - highlight only cells with our figures
 *	If our figure selected - highlight all other cells
 */
void AChessPlayer::TraceForCell(const FVector& Start, const FVector& End)
{
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	CheckTraceForCell(HitResult);
}

void AChessPlayer::CheckTraceForCell(FHitResult HitResult)
{
	if (HitResult.Actor.IsValid())
	{
		if (AChessBoardCell* HitCell = Cast<AChessBoardCell>(HitResult.Actor.Get()))
		{
			if (CurrentCellFocus != HitCell && !HitCell->bIsNonGameCell)
			{
				if (CurrentCellFocus)
				{
					CurrentCellFocus->Highlight(false);
				}

				CurrentCellFocus = HitCell;

				if (CellsToHighlight.Contains(CurrentCellFocus))
				{
					if (CurrentCellFocus != SelectedCell)
					{
						CurrentCellFocus->Highlight(true);
					}
				}
			}
		}
	}
	else if (CurrentCellFocus)
	{
		CurrentCellFocus->Highlight(false);
		CurrentCellFocus = nullptr;
	}
}

/**	Trigger click action
 *	If nothing selected - select cell with our figure
 *	If our figure selected - select other our figure or perform move
 */
void AChessPlayer::TriggerClick()
{
	if (bCanMakeMove && CurrentCellFocus)
	{
		AChessFigureBase* OtherFigure = GameBoard->GetFigureByAddress(CurrentCellFocus->CellAddress);
		if (OtherFigure)
		{
			if (TeamIndex == OtherFigure->GetTeamIndex())
			{
				SelectFigure(OtherFigure);
			}
			else
			{
				if (SelectedFigure)
				{
					const FMoveResult Move = FMoveResult(0, CurrentCellFocus->CellAddress, SelectedFigure);
					MakeMove(Move);
				}
			}
		}
		else
		{
			if (SelectedFigure)
			{
				const FMoveResult Move = FMoveResult(0, CurrentCellFocus->CellAddress, SelectedFigure);
				MakeMove(Move);
			}
		}
	}
}

void AChessPlayer::TriggerForMakeMove(bool bCondition)
{
	Super::TriggerForMakeMove(bCondition);
	bCanTraceForCells = true;
	UpdateCellsToHighlight();
}

void AChessPlayer::MakeMove(FMoveResult Move)
{
	if (SelectedFigure->CheckIsCellReachable(Move.CellAddress))
	{
		if (Move.Figure)
		{
			bCanTraceForCells = false;
			GameBoard->MoveFigure(Move.Figure, Move.CellAddress);
			CurrentCellFocus->Highlight(false);
			CurrentCellFocus = nullptr;
			Move.Figure->OnFigureMoveAnimFinished.AddDynamic(this, &AChessPlayerBase::PerformMove);
		}
	}
}

void AChessPlayer::UpdateCellsToHighlight()
{
	CellsToHighlight.Empty();

	for (auto& Figure : GameBoard->GetTeam1ActiveFigures())
	{
		AChessBoardCell* Cell = GameBoard->GetCellByAddress(Figure->CellAddress);
		if (Cell)
		{
			CellsToHighlight.Add(Cell);
		}
	}
}

void AChessPlayer::SelectFigure(AChessFigureBase* Figure)
{
	for (auto& Cell : CellsToTurnOff)
	{
		Cell->HighlightMove(false);
		Cell->Highlight(false);
		Cell->Select(false);
	}

	CellsToTurnOff.Empty();
	SelectedFigure = Figure;

	if (SelectedFigure)
	{
		AChessBoardCell* Cell = GameBoard->GetCellByAddress(SelectedFigure->CellAddress);
		if (Cell)
		{
			Cell->Highlight(false);
			Cell->Select(true);
			CellsToTurnOff.Add(Cell);
		}

		SelectedFigure->CalculateMovesResults();

		for (auto& CA : SelectedFigure->CellsAvailableToMove)
		{
			Cell = GameBoard->GetCellByAddress(CA);
			if (Cell)
			{
				CellsToHighlight.Add(Cell);
				Cell->HighlightMove(true);
				CellsToTurnOff.Add(Cell);
			}
		}
	}
}

void AChessPlayer::Resign()
{
	OnGameFinished.Broadcast(this, 2);
}