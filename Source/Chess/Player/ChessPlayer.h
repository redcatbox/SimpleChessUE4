// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ChessPlayerBase.h"
#include "ChessPlayer.generated.h"

UCLASS()
class CHESS_API AChessPlayer : public AChessPlayerBase
{
	GENERATED_BODY()

public:
	AChessPlayer();
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void TriggerForMakeMove(bool bCondition) override;
	virtual void SelectPiece(APieceBase* Piece) override;
	virtual void MakeMove(FMoveInfo Move) override;
	virtual void Resign() override;
	void AdjustCamera();

protected:
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);
	void TraceForCell(const FVector& Start, const FVector& End);
	void CheckTraceForCell(FHitResult HitResult);
	void TriggerClick();

	UPROPERTY(VisibleDefaultsOnly, Category = "Camera")
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly, Category = "Camera")
		class UCameraComponent* CameraComponent;

	UPROPERTY()
		AChessBoardCell* CurrentCellFocus;

	UPROPERTY()
		bool bCanTraceForCells;

	UPROPERTY()
		TArray<AChessBoardCell*> CellsToHighlight;

	void UpdateCellsToHighlight();
};
