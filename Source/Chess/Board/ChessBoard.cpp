// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ChessBoard.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "ChessBoardCell.h"
#include "Chess/Pieces/PieceBishop.h"
#include "Chess/Pieces/PieceKing.h"
#include "Chess/Pieces/PieceKnight.h"
#include "Chess/Pieces/PiecePawn.h"
#include "Chess/Pieces/PieceQueen.h"
#include "Chess/Pieces/PieceRook.h"

AChessBoard::AChessBoard()
{
	// Create dummy root scene component
	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	RootComponent = DefaultRoot;

	// Set defaults
	NumCells = 8;
	CellSpacing = 200.f;

	// Map initial locations for Team1 Pieces, Team2 will be mirrored
	StartingPositions.Add(TEXT("1Ra1"));
	StartingPositions.Add(TEXT("1Nb1"));
	StartingPositions.Add(TEXT("1Bc1"));
	StartingPositions.Add(TEXT("1Qd1"));
	StartingPositions.Add(TEXT("1Ke1"));
	StartingPositions.Add(TEXT("1Bf1"));
	StartingPositions.Add(TEXT("1Ng1"));
	StartingPositions.Add(TEXT("1Rh1"));
	StartingPositions.Add(TEXT("1a2"));
	StartingPositions.Add(TEXT("1b2"));
	StartingPositions.Add(TEXT("1c2"));
	StartingPositions.Add(TEXT("1d2"));
	StartingPositions.Add(TEXT("1e2"));
	StartingPositions.Add(TEXT("1f2"));
	StartingPositions.Add(TEXT("1g2"));
	StartingPositions.Add(TEXT("1h2"));
	StartingPositions.Add(TEXT("2a7"));
	StartingPositions.Add(TEXT("2b7"));
	StartingPositions.Add(TEXT("2c7"));
	StartingPositions.Add(TEXT("2d7"));
	StartingPositions.Add(TEXT("2e7"));
	StartingPositions.Add(TEXT("2f7"));
	StartingPositions.Add(TEXT("2g7"));
	StartingPositions.Add(TEXT("2h7"));
	StartingPositions.Add(TEXT("2Ra8"));
	StartingPositions.Add(TEXT("2Nb8"));
	StartingPositions.Add(TEXT("2Bc8"));
	StartingPositions.Add(TEXT("2Qd8"));
	StartingPositions.Add(TEXT("2Ke8"));
	StartingPositions.Add(TEXT("2Bf8"));
	StartingPositions.Add(TEXT("2Ng8"));
	StartingPositions.Add(TEXT("2Rh8"));

	//InitialPiecesLocations.Add(0, APieceRook::StaticClass());
	//InitialPiecesLocations.Add(1, APieceKnight::StaticClass());
	//InitialPiecesLocations.Add(2, APieceBishop::StaticClass());
	//InitialPiecesLocations.Add(3, APieceQueen::StaticClass());
	//InitialPiecesLocations.Add(4, APieceKing::StaticClass());
	//InitialPiecesLocations.Add(5, APieceBishop::StaticClass());
	//InitialPiecesLocations.Add(6, APieceKnight::StaticClass());
	//InitialPiecesLocations.Add(7, APieceRook::StaticClass());
	//InitialPiecesLocations.Add(8, APiecePawn::StaticClass());
	//InitialPiecesLocations.Add(9, APiecePawn::StaticClass());
	//InitialPiecesLocations.Add(10, APiecePawn::StaticClass());
	//InitialPiecesLocations.Add(11, APiecePawn::StaticClass());
	//InitialPiecesLocations.Add(12, APiecePawn::StaticClass());
	//InitialPiecesLocations.Add(13, APiecePawn::StaticClass());
	//InitialPiecesLocations.Add(14, APiecePawn::StaticClass());
	//InitialPiecesLocations.Add(15, APiecePawn::StaticClass());
}

void AChessBoard::PrepareGameBoard()
{
	CreateGameCells();
	CreateNonGameCells();
	CreatePieces();
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
	// Non game cells for beaten Pieces
	const int32 NumCellsNG = 4;
	const int32 NumCellsTotal = NumCellsNG * NumCellsNG;
	const float Size = NumCellsNG * CellSpacing;

	// Team1
	FVector InitialOffset = FVector(-CellSpacing * 2.f, CellSpacing * 6.5f, 0.f) + GetActorLocation();
	for (int32 CellIndex = 0; CellIndex < NumCellsTotal; CellIndex++)
	{
		const int32 CellIndexX = CellIndex % NumCellsNG;
		const int32 CellIndexY = CellIndex / NumCellsNG;
		const float YOffset = CellIndexX * CellSpacing - Size / 2 + CellSpacing / 2;
		const float XOffset = CellIndexY * CellSpacing - Size / 2 + CellSpacing / 2;
		const FVector CellLocation = FVector(XOffset, YOffset, 0.f) + InitialOffset;
		AChessBoardCell* NewCell = GetWorld()->SpawnActor<AChessBoardCell>(CellLocation, FRotator::ZeroRotator);
		NewCell->SetCellSize(CellSpacing);
		NewCell->CellAddress = FIntPoint(CellIndexX + 100, CellIndexY + 100);
		NewCell->bIsNonGameCell = true;
		CellsBeaten1.Add(NewCell);
	}

	// Team2
	InitialOffset = FVector(CellSpacing * 2.f, CellSpacing * 6.5f, 0.f) + GetActorLocation();
	for (int32 CellIndex = 0; CellIndex < NumCellsTotal; CellIndex++)
	{
		const int32 CellIndexX = CellIndex % NumCellsNG;
		const int32 CellIndexY = CellIndex / NumCellsNG;
		const float YOffset = CellIndexX * CellSpacing - Size / 2 + CellSpacing / 2;
		const float XOffset = CellIndexY * CellSpacing - Size / 2 + CellSpacing / 2;
		const FVector CellLocation = FVector(XOffset, YOffset, 0.f) + InitialOffset;
		AChessBoardCell* NewCell = GetWorld()->SpawnActor<AChessBoardCell>(CellLocation, FRotator::ZeroRotator);
		NewCell->SetCellSize(CellSpacing);
		NewCell->CellAddress = FIntPoint(CellIndexX + 100, CellIndexY + 100);
		NewCell->bIsNonGameCell = true;
		CellsBeaten2.Add(NewCell);
	}
}

void AChessBoard::CreatePieces()
{
	if (GameCells.Num() > 0)
	{
		for (auto& SP : StartingPositions)
		{
			const FPieceInfo PieceInfo = HumanFormatToPieceInfo(SP);
			AChessBoardCell* Cell = GetCellByAddress(PieceInfo.CellAddress);
			if (PieceInfo.TeamId > 0 && PieceInfo.PieceClass && Cell)
			{
				APieceBase* Piece;
				FActorSpawnParameters SpawnInfo;
				SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				//Team1
				if (PieceInfo.TeamId == 1)
				{
					Piece = GetWorld()->SpawnActor<APieceBase>(PieceInfo.PieceClass, Cell->GetActorTransform(), SpawnInfo);
					Piece->SetTeam(PieceInfo.TeamId);
					Piece->GameBoard = this;
					Piece->CellAddress = Cell->CellAddress;
					Team1ActivePieces.Add(Piece);

					// Store reference to king piece
					if (Piece->GetClass() == APieceKing::StaticClass())
					{
						King1 = Piece;
					}
				}

				//Team2
				if (PieceInfo.TeamId == 1)
				{
					Piece = GetWorld()->SpawnActor<APieceBase>(PieceInfo.PieceClass, Cell->GetActorTransform(), SpawnInfo);
					Piece->SetTeam(PieceInfo.TeamId);
					Piece->GameBoard = this;
					Piece->CellAddress = Cell->CellAddress;
					Team2ActivePieces.Add(Piece);

					// Store reference to king piece
					if (Piece->GetClass() == APieceKing::StaticClass())
					{
						King2 = Piece;
					}
				}
			}
		}

		//for (auto& Entry : InitialPiecesLocations)
		//{
		//	if (Entry.Key <= GameCells.Num())
		//	{
		//		APieceBase* Piece;
		//		FActorSpawnParameters SpawnInfo;
		//		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//		//Team1
		//		int32 Index = Entry.Key;
		//		Piece = GetWorld()->SpawnActor<APieceBase>(Entry.Value, GameCells[Index]->GetActorTransform(), SpawnInfo);
		//		Piece->SetTeam(1);
		//		Piece->GameBoard = this;
		//		Piece->CellAddress = GameCells[Index]->CellAddress;
		//		Team1ActivePieces.Add(Piece);
		//		if (Piece->GetClass() == APieceKing::StaticClass())
		//		{
		//			King1 = Piece;
		//		}

		//		//Team2
		//		Index = GameCells.Num() - 1 - Entry.Key;
		//		Piece = GetWorld()->SpawnActor<APieceBase>(Entry.Value, GameCells[Index]->GetActorTransform(), SpawnInfo);
		//		Piece->SetTeam(2);
		//		Piece->GameBoard = this;
		//		Piece->CellAddress = GameCells[Index]->CellAddress;
		//		Team2ActivePieces.Add(Piece);
		//		if (Piece->GetClass() == APieceKing::StaticClass())
		//		{
		//			King2 = Piece;
		//		}
		//	}
		//}
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

	// Destroy team 1 non-game cells
	for (auto& Cell : CellsBeaten1)
	{
		Cell->Highlight(false);
		Cell->Select(false);
		Cell->Destroy();
	}
	CellsBeaten1.Empty();

	// Destroy team 2 non-game cells
	for (auto& Cell : CellsBeaten2)
	{
		Cell->Highlight(false);
		Cell->Select(false);
		Cell->Destroy();
	}
	CellsBeaten2.Empty();

	// Destroy team 1 active Pieces
	King1 = nullptr;
	for (auto& Piece : Team1ActivePieces)
	{
		Piece->Destroy();
	}
	Team1ActivePieces.Empty();

	// Destroy team 2 active Pieces
	King2 = nullptr;
	for (auto& Piece : Team2ActivePieces)
	{
		Piece->Destroy();
	}
	Team2ActivePieces.Empty();

	// Destroy team 1 beaten Pieces
	for (auto& Piece : Team1BeatenPieces)
	{
		Piece->Destroy();
	}
	Team1BeatenPieces.Empty();

	// Destroy team 2 beaten Pieces
	for (auto& Piece : Team2BeatenPieces)
	{
		Piece->Destroy();
	}
	Team2BeatenPieces.Empty();
}

void AChessBoard::SwapPiecesOnCells(AChessBoardCell* FirstCell, AChessBoardCell* SecondCell)
{
	//APieceBase* F1 = GetPieceByAddress(GameCells[FirstCellId]->CellAddress);
	//APieceBase* F2 = GetPieceByAddress(GameCells[SecondCellId]->CellAddress);

	//if (F1 && F2)
	//{
	//	F1->CellAddress = GameCells[SecondCellId]->CellAddress;
	//	F1->SetActorLocation(GameCells[SecondCellId]->GetActorLocation());
	//	F2->CellAddress = GameCells[FirstCellId]->CellAddress;
	//	F2->SetActorLocation(GameCells[FirstCellId]->GetActorLocation());
	//}
}

void AChessBoard::MovePiece(APieceBase* Piece, FIntPoint Address)
{
	if (Piece)
	{
		Piece->CellsAvailableToMove.Empty();
		Piece->MoveInfos.Empty();

		AChessBoardCell* PieceCell = GetCellByAddress(Piece->CellAddress);
		AChessBoardCell* NextCell = GetCellByAddress(Address);
		if (PieceCell && NextCell)
		{
			//const FString Log = CellAddressToHumanFormat(Piece->CellAddress) + TEXT(" -> ") + CellAddressToHumanFormat(Address);
			//UE_LOG(LogTemp, Warning, TEXT("Team %d %s %s"), Piece->GetTeamIndex(), *Piece->FriendlyName, *Log);

			APieceBase* OtherPiece = GetPieceByAddress(Address);
			if (OtherPiece)
			{
				// Move beaten Piece out of board
				Piece->bIsMoved = true;
				SetPieceBeaten(OtherPiece);
			}

			// Move Piece to selected cell
			Piece->bIsMoved = true;
			Piece->CellAddress = NextCell->CellAddress;
			Piece->PlayMovePieceAnim(Piece->GetActorLocation(), NextCell->GetActorLocation());
		}
	}
}

void AChessBoard::SetPieceBeaten(APieceBase* Piece)
{
	Piece->bIsBeaten = true;

	if (Piece->GetTeamIndex() == 1)
	{
		Team1ActivePieces.RemoveSwap(Piece);
		Team1BeatenPieces.Add(Piece);

		for (int i = 0; i < CellsBeaten1.Num() / 2 - 1; i++)
		{
			if (!GetPieceByAddress(CellsBeaten1[i]->CellAddress))
			{
				Piece->CellAddress = CellsBeaten1[i]->CellAddress;
				Piece->PlayMovePieceAnim(Piece->GetActorLocation(), CellsBeaten1[i]->GetActorLocation());
				break;
			}
		}
	}
	else
	{
		Team2ActivePieces.RemoveSwap(Piece);
		Team2BeatenPieces.Add(Piece);

		for (int i = CellsBeaten2.Num() - 1; i > CellsBeaten2.Num() / 2; i--)
		{
			if (!GetPieceByAddress(CellsBeaten2[i]->CellAddress))
			{
				Piece->CellAddress = CellsBeaten2[i]->CellAddress;
				Piece->PlayMovePieceAnim(Piece->GetActorLocation(), CellsBeaten2[i]->GetActorLocation());
				break;
			}
		}
	}
}

TArray<FMoveInfo> AChessBoard::CalculatePiecesMoves()
{
	TArray<FMoveInfo> Result;

	for (auto& Piece : Team1ActivePieces)
	{
		Piece->CalculateMovesResults();
		Result.Append(Piece->MoveInfos);
	}

	for (auto& Piece : Team2ActivePieces)
	{
		Piece->CalculateMovesResults();
		Result.Append(Piece->MoveInfos);
	}

	return Result;
}

void AChessBoard::EvaluateGame()
{
	TArray<FMoveInfo> AllMoves = CalculatePiecesMoves();
	TArray<FMoveInfo> Team1Moves;
	TArray<FMoveInfo> Team2Moves;

	for (auto& M : AllMoves)
	{
		if (M.Piece->GetTeamIndex() == 1)
		{
			Team1Moves.Add(M);
		}

		if (M.Piece->GetTeamIndex() == 2)
		{
			Team2Moves.Add(M);
		}
	}


	//Stalemate
	if (Team1Moves.Num() == 0)
	{
		OnGameFinished.Broadcast(1, 1);
		return;
	}

	if (Team2Moves.Num() == 0)
	{
		OnGameFinished.Broadcast(2, 1);
		return;
	}
	//Stalemate


	//Checkmate 1
	if (King1->bIsBeaten)
	{
		OnGameFinished.Broadcast(2, 0);
		return;
	}

	bool bKing1HasNoMoves = false;
	bool bKing1IsUnderAttack = false;
	King1->CalculateMovesResults();
	if (King1->MoveInfos.Num() == 0)
	{
		bKing1HasNoMoves = true;
	}

	AChessBoardCell* King1Cell = GetCellByAddress(King1->CellAddress);
	for (auto& M : Team2Moves)
	{
		if (GetCellByAddress(M.CellAddress) == King1Cell)
		{
			bKing1IsUnderAttack = true;
			break;
		}
	}

	const bool bKing1Checkmate = bKing1HasNoMoves && bKing1IsUnderAttack;
	if (bKing1Checkmate)
	{
		OnGameFinished.Broadcast(1, 0);
		return;
	}
	//Checkmate 1


	//Checkmate 2
	if (King2->bIsBeaten)
	{
		OnGameFinished.Broadcast(1, 0);
		return;
	}

	bool bKing2HasNoMoves = false;
	bool bKing2IsUnderAttack = false;
	King2->CalculateMovesResults();
	if (King2->MoveInfos.Num() == 0)
	{
		bKing2HasNoMoves = true;
	}

	AChessBoardCell* King2Cell = GetCellByAddress(King2->CellAddress);
	for (auto& M : Team1Moves)
	{
		if (GetCellByAddress(M.CellAddress) == King2Cell)
		{
			bKing2IsUnderAttack = true;
			break;
		}
	}

	const bool bKing2Checkmate = bKing2HasNoMoves && bKing2IsUnderAttack;
	if (bKing2Checkmate)
	{
		OnGameFinished.Broadcast(1, 0);
		return;
	}

	OnGameContinue.Broadcast();
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

APieceBase* AChessBoard::GetPieceByAddress(FIntPoint Address)
{
	for (auto& Piece : Team1ActivePieces)
	{
		if (Piece->CellAddress == Address)
		{
			return Piece;
		}
	}

	for (auto& Piece : Team1BeatenPieces)
	{
		if (Piece->CellAddress == Address)
		{
			return Piece;
		}
	}

	for (auto& Piece : Team2ActivePieces)
	{
		if (Piece->CellAddress == Address)
		{
			return Piece;
		}
	}

	for (auto& Piece : Team2BeatenPieces)
	{
		if (Piece->CellAddress == Address)
		{
			return Piece;
		}
	}

	return nullptr;
}

FString AChessBoard::CellAddressToHumanFormat(FIntPoint& Address)
{
	FString Result;

	switch (Address.X)
	{
	case 0: Result += TEXT("a"); break;
	case 1: Result += TEXT("b"); break;
	case 2: Result += TEXT("c"); break;
	case 3: Result += TEXT("d"); break;
	case 4: Result += TEXT("e"); break;
	case 5: Result += TEXT("f"); break;
	case 6: Result += TEXT("g"); break;
	case 7: Result += TEXT("h"); break;
	default: Result = TEXT("Not found!"); return Result;
	}

	Result += FString::FromInt(Address.Y + 1);
	return Result;
}

FIntPoint AChessBoard::HumanFormatToCellAddress(FString& Info)
{
	if (Info.Len() == 2)
	{
		const FString InfoLower = Info.ToLower();
		int32 AddressX;
		switch (InfoLower[0])
		{
		case 'a': AddressX = 0; break;
		case 'b': AddressX = 1; break;
		case 'c': AddressX = 2; break;
		case 'd': AddressX = 3; break;
		case 'e': AddressX = 4; break;
		case 'f': AddressX = 5; break;
		case 'g': AddressX = 6; break;
		case 'h': AddressX = 7; break;
		default: AddressX = -1;
		}

		const int32 AddressY = InfoLower[1] - 1;
		return FIntPoint(AddressX, AddressY);
	}
	else
	{
		return FIntPoint(-1, -1);
	}
}

FPieceInfo AChessBoard::HumanFormatToPieceInfo(FString& Info)
{
	const FString InfoLower = Info.ToLower();

	// TeamId
	int32 TeamId = 0;
	TeamId = InfoLower[0] == 1 ? 1 : TeamId;
	TeamId = InfoLower[0] == 2 ? 2 : TeamId;

	TCHAR PieceChar = '0';
	if (Info.Len() == 3)
	{
		// Pawn address
		PieceChar = 'p';
	}
	else if (Info.Len() == 4)
	{
		// Non-pawn address
		PieceChar = InfoLower[1];
	}

	// Piece class
	TSubclassOf<APieceBase> PieceClass;
	switch (PieceChar)
	{
	case 'k': PieceClass = APieceKing::StaticClass(); break;
	case 'q': PieceClass = APieceQueen::StaticClass(); break;
	case 'r': PieceClass = APieceRook::StaticClass(); break;
	case 'b': PieceClass = APieceBishop::StaticClass(); break;
	case 'n': PieceClass = APieceKnight::StaticClass(); break;
	case 'p': PieceClass = APiecePawn::StaticClass(); break;
	default: PieceClass = NULL;
	}

	FString Address;
	for (int32 i = InfoLower.Len() - 1; i > InfoLower.Len() - 3; i--)
	{
		Address.AppendChar(InfoLower[i]);
	}

	const FIntPoint CellAddress = HumanFormatToCellAddress(Address);

	return FPieceInfo(TeamId, PieceClass, CellAddress);
}