// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ChessPlayerController.h"

AChessPlayerController::AChessPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;
}
