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
	virtual void SelectFigure(AChessFigureBase* Figure) override;
	virtual void MakeMove(FMoveResult Move) override;
	virtual void Resign() override;

protected:
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);
	void CameraZoom(float Val);
	void TraceForCell(const FVector& Start, const FVector& End);
	void CheckTraceForCell(FHitResult HitResult);
	void TriggerClick();


	UPROPERTY()
		class USpringArmComponent* SpringArm;

	UPROPERTY()
		class UCameraComponent* CameraComponent;

	UPROPERTY()
		AChessBoardCell* CurrentCellFocus;

	UPROPERTY()
		bool bCameraRotation;

	UPROPERTY()
		bool bCanTraceForCells;

	UPROPERTY()
		TArray<AChessBoardCell*> CellsToHighlight;

	void UpdateCellsToHighlight();
};