// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "MoveBase.h"
#include "FigureBase.generated.h"

class AChessBoard;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFigureMoveAnimFinished);

UCLASS(Abstract)
class CHESS_API AFigureBase : public AActor
{
	GENERATED_BODY()

public:
	AFigureBase();
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
		class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY()
		TArray<class UMaterialInstance*> TeamsMaterials;

protected:
	/** Update figure material */
	void SetMaterialByTeam(int32 TeamId);

	/** Team id for this figure */
	UPROPERTY()
		int32 TeamIndex;

public:
	FORCEINLINE int32 GetTeamIndex() const { return TeamIndex; }

	/** Set figure team */
	UFUNCTION()
		void SetTeam(int32 TeamId);

	UPROPERTY()
		FString FriendlyName;

	/** Is figure ever moved? */
	UPROPERTY()
		bool bIsMoved;

	/** Is figure beaten? */
	UPROPERTY()
		bool bIsBeaten;

	/** Get figure value for move results calculations */
	UFUNCTION()
		int32 GetFigureValue();
	
protected:
	/** Legal moves this figure can make */
	UPROPERTY()
		TArray<UMoveBase*> LegalMoves;

public:
	/** Figure address on board */
	UPROPERTY()
		FIntPoint CellAddress;
	
	/** Check is provided cell can be reached by available moves */
	UFUNCTION()
		virtual bool CheckIsCellReachable(FIntPoint Cell);

	/** Returns all moves results */
	UFUNCTION()
		void CalculateMovesResults();
	
	/** Reference to cell that this figure staying on */
	UPROPERTY()
		AChessBoard* GameBoard;

	UPROPERTY()
		TArray<FIntPoint> CellsAvailableToMove;

	UPROPERTY()
		TArray<FMoveResult> MoveResults;

	/** Animation of figure movement */
	UFUNCTION()
		void PlayMoveFigureAnim(FVector LocFrom, FVector LocTo);

	/** OnFigureMoveAnimFinished event */
	UPROPERTY()
		FOnFigureMoveAnimFinished OnFigureMoveAnimFinished;

protected:
	// Variables for tick-handled movement animation
	UPROPERTY()
		bool bMoveFigure;

	UPROPERTY()
		float Alpha;

	UPROPERTY()
		FVector MoveFrom;

	UPROPERTY()
		FVector MoveTo;
	// Variables for tick-handled movement animation
};