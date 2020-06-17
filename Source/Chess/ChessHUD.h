// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "ChessHUD.generated.h"

UCLASS()
class CHESS_API AChessHUD : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "ChessHUD")
		void ShowMenuMain();

	UFUNCTION(BlueprintCallable, Category = "ChessHUD")
		void HideMenuMain();

	UFUNCTION(BlueprintCallable, Category = "ChessHUD")
		void ShowGameUI();

	UFUNCTION(BlueprintCallable, Category = "ChessHUD")
		void HideGameUI();

	UFUNCTION(BlueprintCallable, Category = "ChessHUD")
		void ShowInfoScreen();

	UFUNCTION(BlueprintCallable, Category = "ChessHUD")
		void HideInfoScreen();
	
	UFUNCTION(BlueprintCallable, Category = "ChessHUD")
		void StartGame();

	UFUNCTION(BlueprintCallable, Category = "ChessHUD")
		void ResetGame();

	UFUNCTION(BlueprintCallable, Category = "ChessHUD")
		void FinishGame(FText InfoText);

	UFUNCTION(BlueprintCallable, Category = "ChessHUD")
		void TogglePause();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChessHUD")
		TSubclassOf<class UUserWidget> MenuMainClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChessHUD")
		TSubclassOf<class UUserWidget> GameUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChessHUD")
		TSubclassOf<class UUserWidget> InfoScreenClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ChessHUD")
		FText InfoScreenText;
	
protected:
	UPROPERTY()
		class UUserWidget* MenuMainWidget;

	UPROPERTY()
		class UUserWidget* GameUIWidget;

	UPROPERTY()
		class UUserWidget* InfoScreenWidget;
};