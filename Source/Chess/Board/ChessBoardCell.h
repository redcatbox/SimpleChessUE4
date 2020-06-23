// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "ChessBoardCell.generated.h"

/** A board cell that can be clicked */
UCLASS()
class CHESS_API AChessBoardCell : public AActor
{
	GENERATED_BODY()

	/** Box collision component for player tracing */
	UPROPERTY()
		class UBoxComponent* BoxComponent;

public:
	AChessBoardCell();
	//virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
		void SetCellSize(float Size) const;

	UPROPERTY()
		bool bIsNonGameCell;
	
	/** Board that owns cell */
	UPROPERTY()
		class AChessBoard* GameBoard;

	/** Handle the cell being clicked */
	UFUNCTION()
		void CellClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

	/** Handle the cell being touched  */
	UFUNCTION()
		void OnFingerPressedCell(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

	void Highlight(bool bOn);
	void Select(bool bSel);
	void HighlightMove(bool bOn);

	/** Cell 2D address for moves calculations */
	UPROPERTY()
		FIntPoint CellAddress;

protected:
	/** PS asset for highlight */
	UPROPERTY()
		UParticleSystem* HighlightPS;
	
	/** PS component for highlight */
	UPROPERTY(Transient)
		UParticleSystemComponent* HighlightPSC;

	/** PS asset for highlight when selected */
	UPROPERTY()
		UParticleSystem* SelectPS;

	/** PS component for highlight when selected */
	UPROPERTY(Transient)
		UParticleSystemComponent* SelectPSC;

	/** PS asset for highlight */
	UPROPERTY()
		UParticleSystem* HighlightMovePS;

	/** PS component for highlight */
	UPROPERTY(Transient)
		UParticleSystemComponent* HighlightMovePSC;
};
