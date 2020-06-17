// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ChessPlayerController.h"
#include "ChessPlayerCameraManager.h"

AChessPlayerController::AChessPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;

	PlayerCameraManagerClass = AChessPlayerCameraManager::StaticClass();
}

void AChessPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetControlRotation(FRotator(-60.f, 0.f, 0.f));
}