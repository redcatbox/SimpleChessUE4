// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ChessBoard.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#include "Chess/Figures/FigurePawn.h"
#include "Chess/Figures/FigureBishop.h"
#include "Chess/Figures/FigureKnight.h"
#include "Chess/Figures/FigureRook.h"
#include "Chess/Figures/FigureKing.h"
#include "Chess/Figures/FigureQueen.h"

#define LOCTEXT_NAMESPACE "ChessBoard"

AChessBoard::AChessBoard()
{
	// Create dummy root scene component
	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	RootComponent = DefaultRoot;

	// Set defaults
	NumCells = 8;
	CellSpacing = 200.f;

	// Map initial locations for Team1 figures, Team2 will be mirrored
	InitialFiguresLocations.Add(0, AFigureRook::StaticClass());
	InitialFiguresLocations.Add(1, AFigureKnight::StaticClass());
	InitialFiguresLocations.Add(2, AFigureBishop::StaticClass());
	InitialFiguresLocations.Add(3, AFigureQueen::StaticClass());
	InitialFiguresLocations.Add(4, AFigureKing::StaticClass());
	InitialFiguresLocations.Add(5, AFigureBishop::StaticClass());
	InitialFiguresLocations.Add(6, AFigureKnight::StaticClass());
	InitialFiguresLocations.Add(7, AFigureRook::StaticClass());
	InitialFiguresLocations.Add(8, AFigurePawn::StaticClass());
	InitialFiguresLocations.Add(9, AFigurePawn::StaticClass());
	InitialFiguresLocations.Add(10, AFigurePawn::StaticClass());
	InitialFiguresLocations.Add(11, AFigurePawn::StaticClass());
	InitialFiguresLocations.Add(12, AFigurePawn::StaticClass());
	InitialFiguresLocations.Add(13, AFigurePawn::StaticClass());
	InitialFiguresLocations.Add(14, AFigurePawn::StaticClass());
	InitialFiguresLocations.Add(15, AFigurePawn::StaticClass());
}

void AChessBoard::PrepareGameBoard()
{
	CreateGameCells();
	CreateNonGameCells();
	CreateFigures();
}

void AChessBoard::CreateGameCells()
{
	// Number of cells
	const int32 NumCellsTotal = NumCells * NumCells;
	const float BoardSize = NumCells * CellSpacing;

	// Spawn cells
	for (int32 CellIndex = 0; CellIndex < NumCellsTotal; CellIndex++)
	{
		// Calculate cells positions from board center
		const int32 CellIndexAH = CellIndex % NumCells; // A-H
		const int32 CellIndex18 = CellIndex / NumCells; // 1-8
		const float YOffset = CellIndexAH * CellSpacing - BoardSize / 2 + CellSpacing / 2;
		const float XOffset = CellIndex18 * CellSpacing - BoardSize / 2 + CellSpacing / 2;

		// Make position vector, offset from board location
		const FVector CellLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

		// Spawn a cell
		AChessBoardCell* NewCell = GetWorld()->SpawnActor<AChessBoardCell>(CellLocation, FRotator::ZeroRotator);
		NewCell->CellAddress = FIntPoint(CellIndexAH, CellIndex18);
		NewCell->SetCellSize(CellSpacing);
		GameCells.Add(NewCell);

		// Tell the cell about its owner
		if (NewCell != nullptr)
		{
			NewCell->GameBoard = this;
		}
	}
}

void AChessBoard::CreateNonGameCells()
{
	// Non game cells for beaten figures
	const int32 NumCellsX = InitialFiguresLocations.Num() / 4;
	const int32 NumCellsY = NumCellsX * 2;
	const int32 NumCellsTotal = NumCellsX * NumCellsY;
	const FVector2D Size = FVector2D(NumCellsY * CellSpacing, NumCellsX * CellSpacing);
	const FVector InitialOffset = FVector(0.f, CellSpacing * 7, 0.f) + GetActorLocation();

	for (int32 CellIndex = 0; CellIndex < NumCellsTotal; CellIndex++)
	{
		const int32 CellIndexX = CellIndex % NumCellsX;
		const int32 CellIndexY = CellIndex * 2 / NumCellsY;
		const float YOffset = CellIndexX * CellSpacing - Size.Y / 2 + CellSpacing / 2;
		const float XOffset = CellIndexY * CellSpacing - Size.X / 2 + CellSpacing / 2;
		const FVector CellLocation = FVector(XOffset, YOffset, 0.f) + InitialOffset;
		AChessBoardCell* NewCell = GetWorld()->SpawnActor<AChessBoardCell>(CellLocation, FRotator::ZeroRotator);
		NewCell->SetCellSize(CellSpacing);
		NewCell->CellAddress = FIntPoint(CellIndexX + 100, CellIndexY + 100);
		NewCell->bIsNonGameCell = true;
		CellsBeaten.Add(NewCell);
	}
}

void AChessBoard::CreateFigures()
{
	if (GameCells.Num() > 0)
	{
		for (auto& Entry : InitialFiguresLocations)
		{
			if (Entry.Key <= GameCells.Num())
			{
				AChessFigureBase* Figure;
				FActorSpawnParameters SpawnInfo;
				SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				//Team1
				int32 Index = Entry.Key;
				Figure = GetWorld()->SpawnActor<AChessFigureBase>(Entry.Value, GameCells[Index]->GetActorTransform(), SpawnInfo);
				Figure->SetTeam(1);
				Figure->GameBoard = this;
				Figure->CellAddress = GameCells[Index]->CellAddress;
				Team1ActiveFigures.Add(Figure);
				if (Figure->GetClass() == AFigureKing::StaticClass())
				{
					King1 = Figure;
				}

				//Team2
				Index = GameCells.Num() - 1 - Entry.Key;
				Figure = GetWorld()->SpawnActor<AChessFigureBase>(Entry.Value, GameCells[Index]->GetActorTransform(), SpawnInfo);
				Figure->SetTeam(2);
				Figure->GameBoard = this;
				Figure->CellAddress = GameCells[Index]->CellAddress;
				Team2ActiveFigures.Add(Figure);
				if (Figure->GetClass() == AFigureKing::StaticClass())
				{
					King2 = Figure;
				}
			}
		}

		// Hack to mirror queen and king figures from Team2
		SwapFiguresOnCellsIds(59, 60);
	}
}

void AChessBoard::Cleanup()
{
	// Destroy game cells
	for (auto& Cell : GameCells)
	{
		Cell->Highlight(false);
		Cell->Select(false);
		Cell->Destroy();
	}
	GameCells.Empty();

	// Destroy non-game cells
	for (auto& Cell : CellsBeaten)
	{
		Cell->Highlight(false);
		Cell->Select(false);
		Cell->Destroy();
	}
	CellsBeaten.Empty();

	// Destroy team 1 active figures
	King1 = nullptr;
	for (auto& Figure : Team1ActiveFigures)
	{
		Figure->Destroy();
	}
	Team1ActiveFigures.Empty();

	// Destroy team 2 active figures
	King2 = nullptr;
	for (auto& Figure : Team2ActiveFigures)
	{
		Figure->Destroy();
	}
	Team2ActiveFigures.Empty();

	// Destroy team 1 beaten figures
	for (auto& Figure : Team1BeatenFigures)
	{
		Figure->Destroy();
	}
	Team1BeatenFigures.Empty();

	// Destroy team 2 beaten figures
	for (auto& Figure : Team2BeatenFigures)
	{
		Figure->Destroy();
	}
	Team2BeatenFigures.Empty();
}

void AChessBoard::SwapFiguresOnCellsIds(int32 FirstCellId, int32 SecondCellId)
{
	AChessFigureBase* F1 = GetFigureByAddress(GameCells[FirstCellId]->CellAddress);
	AChessFigureBase* F2 = GetFigureByAddress(GameCells[SecondCellId]->CellAddress);

	if (F1 && F2)
	{
		F1->CellAddress = GameCells[SecondCellId]->CellAddress;
		F1->SetActorLocation(GameCells[SecondCellId]->GetActorLocation());
		F2->CellAddress = GameCells[FirstCellId]->CellAddress;
		F2->SetActorLocation(GameCells[FirstCellId]->GetActorLocation());
	}
}

void AChessBoard::MoveFigure(AChessFigureBase* Figure, FIntPoint Address)
{
	if (Figure)
	{
		Figure->CellsAvailableToMove.Empty();
		Figure->MoveResults.Empty();

		AChessBoardCell* FigureCell = GetCellByAddress(Figure->CellAddress);
		AChessBoardCell* NextCell = GetCellByAddress(Address);
		if (FigureCell && NextCell)
		{
			//const FString Log = CellAddressToHumanFormat(Figure->CellAddress) + TEXT(" -> ") + CellAddressToHumanFormat(Address);
			//UE_LOG(LogTemp, Warning, TEXT("Team %d %s %s"), Figure->GetTeamIndex(), *Figure->FriendlyName, *Log);

			AChessFigureBase* OtherFigure = GetFigureByAddress(Address);
			if (OtherFigure)
			{
				// Move beaten figure out of board
				Figure->bIsMoved = true;
				SetFigureBeaten(OtherFigure);
			}

			// Move figure to selected cell
			Figure->bIsMoved = true;
			Figure->CellAddress = NextCell->CellAddress;
			Figure->PlayMoveFigureAnim(Figure->GetActorLocation(), NextCell->GetActorLocation());
		}
	}
}

void AChessBoard::SetFigureBeaten(AChessFigureBase* Figure)
{
	Figure->bIsBeaten = true;

	if (Figure->GetTeamIndex() == 1)
	{
		Team1ActiveFigures.RemoveSwap(Figure);
		Team1BeatenFigures.Add(Figure);

		for (int i = 0; i < CellsBeaten.Num() / 2 - 1; i++)
		{
			if (!GetFigureByAddress(CellsBeaten[i]->CellAddress))
			{
				Figure->CellAddress = CellsBeaten[i]->CellAddress;
				Figure->PlayMoveFigureAnim(Figure->GetActorLocation(), CellsBeaten[i]->GetActorLocation());
				break;
			}
		}
	}
	else
	{
		Team2ActiveFigures.RemoveSwap(Figure);
		Team2BeatenFigures.Add(Figure);

		for (int i = CellsBeaten.Num() - 1; i > CellsBeaten.Num() / 2; i--)
		{
			if (!GetFigureByAddress(CellsBeaten[i]->CellAddress))
			{
				Figure->CellAddress = CellsBeaten[i]->CellAddress;
				Figure->PlayMoveFigureAnim(Figure->GetActorLocation(), CellsBeaten[i]->GetActorLocation());
				break;
			}
		}
	}
}

AChessBoardCell* AChessBoard::GetCellByAddress(FIntPoint Address)
{
	const bool bAddressInBoardLimits =
		Address.X >= 0
		&& Address.Y >= 0
		&& Address.X < NumCells
		&& Address.Y < NumCells;

	if (bAddressInBoardLimits)
	{
		const int32 Id = Address.Y * NumCells + Address.X;
		if (Id > -1 && Id < GameCells.Num())
		{
			return GameCells[Id];
		}
	}

	return nullptr;
}

AChessFigureBase* AChessBoard::GetFigureByAddress(FIntPoint Address)
{
	for (auto& Figure : Team1ActiveFigures)
	{
		if (Figure->CellAddress == Address)
		{
			return Figure;
		}
	}

	for (auto& Figure : Team1BeatenFigures)
	{
		if (Figure->CellAddress == Address)
		{
			return Figure;
		}
	}
	
	for (auto& Figure : Team2ActiveFigures)
	{
		if (Figure->CellAddress == Address)
		{
			return Figure;
		}
	}

	for (auto& Figure : Team2BeatenFigures)
	{
		if (Figure->CellAddress == Address)
		{
			return Figure;
		}
	}
	
	return nullptr;
}

FString AChessBoard::CellAddressToHumanFormat(FIntPoint Address)
{
	FString Result;

	switch (Address.X)
	{
	case 0: Result += TEXT("A"); break;
	case 1: Result += TEXT("B"); break;
	case 2: Result += TEXT("C"); break;
	case 3: Result += TEXT("D"); break;
	case 4: Result += TEXT("E"); break;
	case 5: Result += TEXT("F"); break;
	case 6: Result += TEXT("G"); break;
	case 7: Result += TEXT("H"); break;
	default: TEXT("Error!");
	}

	Result += FString::FromInt(Address.Y + 1);
	return Result;
}

#undef LOCTEXT_NAMESPACE