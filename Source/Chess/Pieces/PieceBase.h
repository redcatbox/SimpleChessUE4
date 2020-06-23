// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "MoveBase.h"
#include "PieceBase.generated.h"

class AChessBoard;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPieceMoveAnimFinished);

UCLASS(Abstract)
class CHESS_API APieceBase : public AActor
{
	GENERATED_BODY()

public:
	APieceBase();
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
		class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY()
		TArray<class UMaterialInstance*> TeamsMaterials;

protected:
	/** Update Piece material */
	void SetMaterialByTeam(int32 TeamId);

	/** Team id for this Piece */
	UPROPERTY()
		int32 TeamIndex;

public:
	FORCEINLINE int32 GetTeamIndex() const { return TeamIndex; }

	/** Set Piece team */
	UFUNCTION()
		void SetTeam(int32 TeamId);

	/** Is Piece ever moved? */
	UPROPERTY()
		bool bIsMoved;

	/** Is Piece beaten? */
	UPROPERTY()
		bool bIsBeaten;

	/** Get Piece value for move results calculations */
	UFUNCTION()
		int32 GetPieceValue();
	
protected:
	/** Legal moves this Piece can make */
	UPROPERTY()
		TArray<UMoveBase*> LegalMoves;

public:
	/** Piece address on board */
	UPROPERTY()
		FIntPoint CellAddress;
	
	/** Check is provided cell can be reached by available moves */
	UFUNCTION()
		virtual bool CheckIsCellReachable(FIntPoint Cell);

	/** Returns all moves results */
	UFUNCTION()
		void CalculateMovesResults();
	
	/** Reference to cell that this Piece staying on */
	UPROPERTY()
		AChessBoard* GameBoard;

	UPROPERTY()
		TArray<FIntPoint> CellsAvailableToMove;

	UPROPERTY()
		TArray<FMoveInfo> MoveInfos;

	/** Animation of Piece movement */
	UFUNCTION()
		void PlayMovePieceAnim(FVector LocFrom, FVector LocTo, bool bNotifyFinished = true);

	/** OnPieceMoveAnimFinished event */
	UPROPERTY()
		FOnPieceMoveAnimFinished OnPieceMoveAnimFinished;

protected:
	// Variables for tick-handled movement animation
	UPROPERTY()
		bool bMovePiece;

	UPROPERTY()
		float Alpha;

	UPROPERTY()
		FVector MoveFrom;

	UPROPERTY()
		FVector MoveTo;

	UPROPERTY()
		bool bNotifyMoveAnimFinished;
	// Variables for tick-handled movement animation
};
