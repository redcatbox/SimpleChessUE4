// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ChessHUD.h"
#include "ChessGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Chess/Player/ChessPlayer.h"

void AChessHUD::ShowMenuMain()
{	
	if (MenuMainClass)
	{
		MenuMainWidget = CreateWidget(GetOwningPlayerController(), MenuMainClass);
		MenuMainWidget->AddToViewport();
	}
}

void AChessHUD::HideMenuMain()
{
	if (AChessPlayer* ChessPlayer = Cast<AChessPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		ChessPlayer->AdjustCamera();
	}
	
	if (MenuMainWidget)
	{
		MenuMainWidget->RemoveFromParent();
		MenuMainWidget = nullptr;
	}
}

void AChessHUD::ShowGameUI()
{
	if (GameUIClass)
	{
		GameUIWidget = CreateWidget(GetOwningPlayerController(), GameUIClass);
		GameUIWidget->AddToViewport();
	}
}

void AChessHUD::HideGameUI()
{
	if (GameUIWidget)
	{
		GameUIWidget->RemoveFromParent();
		GameUIWidget = nullptr;
	}
}

void AChessHUD::ShowInfoScreen()
{
	if (InfoScreenClass)
	{
		InfoScreenWidget = CreateWidget(GetOwningPlayerController(), InfoScreenClass);
		InfoScreenWidget->AddToViewport();
	}
}

void AChessHUD::HideInfoScreen()
{
	InfoScreenText = FText();
	if (InfoScreenWidget)
	{
		InfoScreenWidget->RemoveFromParent();
		InfoScreenWidget = nullptr;
	}
}

void AChessHUD::StartGame()
{
	if (AChessGameMode* ChessGameMode = Cast<AChessGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (ChessGameMode->bHasActiveGame)
		{
			HideGameUI();
		}

		HideMenuMain();
		ShowGameUI();
		ChessGameMode->StartChessGame();
	}
}

void AChessHUD::ResetGame()
{
	HideInfoScreen();

	if (AChessGameMode* ChessGameMode = Cast<AChessGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		ChessGameMode->GameReset();
	}

	ShowMenuMain();
}

void AChessHUD::FinishGame(FText InfoText)
{
	HideGameUI();
	InfoScreenText = InfoText;
	ShowInfoScreen();
}

void AChessHUD::TogglePause()
{
	if (AChessGameMode* ChessGameMode = Cast<AChessGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (ChessGameMode->TogglePause())
		{
			HideGameUI();
			ShowMenuMain();
		}
		else
		{
			HideMenuMain();
			ShowGameUI();
		}
	}
}