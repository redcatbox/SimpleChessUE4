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
	SpringArm->TargetArmLength = 2000.f;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetRelativeRotation(FRotator(-89.f, 0.f, 0.f));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArm);
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->FieldOfView = 75.f;

	bCanTraceForCells = false;
}

void AChessPlayer::AdjustCamera()
{
	if (CameraComponent && GEngine->GameViewport->Viewport)
	{
		const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
		if (ViewportSize.Y != 0)
		{
			const float ViewportAspectRatio = ViewportSize.X / ViewportSize.Y;

			if (ViewportAspectRatio <= 1.51f)
			{
				SpringArm->TargetArmLength = 1500.f;
			}
			else if (ViewportAspectRatio <= 1.8f)
			{
				SpringArm->TargetArmLength = 2000.f;
			}
			else
			{
				SpringArm->TargetArmLength = 2300.f;
			}
		}
	}
}

void AChessPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Actions
	PlayerInputComponent->BindAction("Click", IE_Pressed, this, &AChessPlayer::TriggerClick);

	// Touch events
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AChessPlayer::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AChessPlayer::TouchStopped);
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
 *	If nothing selected - highlight only cells with our Pieces
 *	If our Piece selected - highlight all other cells
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
 *	If nothing selected - select cell with our Piece
 *	If our Piece selected - select other our Piece or perform move
 */
void AChessPlayer::TriggerClick()
{
	if (bCanMakeMove && CurrentCellFocus)
	{
		APieceBase* OtherPiece = GameBoard->GetPieceByAddress(CurrentCellFocus->CellAddress);
		if (OtherPiece)
		{
			if (TeamIndex == OtherPiece->GetTeamIndex())
			{
				SelectPiece(OtherPiece);
			}
			else
			{
				if (SelectedPiece)
				{
					const FMoveInfo Move = FMoveInfo(0, CurrentCellFocus->CellAddress, SelectedPiece);
					MakeMove(Move);
				}
			}
		}
		else
		{
			if (SelectedPiece)
			{
				const FMoveInfo Move = FMoveInfo(0, CurrentCellFocus->CellAddress, SelectedPiece);
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

void AChessPlayer::MakeMove(FMoveInfo Move)
{
	if (SelectedPiece->CheckIsCellReachable(Move.CellAddress))
	{
		if (Move.Piece)
		{
			bCanTraceForCells = false;
			GameBoard->MovePiece(Move.Piece, Move.CellAddress);
			CurrentCellFocus->Highlight(false);
			CurrentCellFocus = nullptr;
			Move.Piece->OnPieceMoveAnimFinished.AddDynamic(this, &AChessPlayerBase::PerformMove);
		}
	}
}

void AChessPlayer::UpdateCellsToHighlight()
{
	CellsToHighlight.Empty();

	for (auto& Piece : GameBoard->GetTeam1ActivePieces())
	{
		AChessBoardCell* Cell = GameBoard->GetCellByAddress(Piece->CellAddress);
		if (Cell)
		{
			CellsToHighlight.Add(Cell);
		}
	}
}

void AChessPlayer::SelectPiece(APieceBase* Piece)
{
	for (auto& Cell : CellsToTurnOff)
	{
		Cell->HighlightMove(false);
		Cell->Highlight(false);
		Cell->Select(false);
	}

	CellsToTurnOff.Empty();
	SelectedPiece = Piece;

	if (SelectedPiece)
	{
		AChessBoardCell* Cell = GameBoard->GetCellByAddress(SelectedPiece->CellAddress);
		if (Cell)
		{
			Cell->Highlight(false);
			Cell->Select(true);
			CellsToTurnOff.Add(Cell);
		}

		SelectedPiece->CalculateMovesResults();

		for (auto& CA : SelectedPiece->CellsAvailableToMove)
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
	GameBoard->OnGameFinished.Broadcast(TeamIndex, 2);
}