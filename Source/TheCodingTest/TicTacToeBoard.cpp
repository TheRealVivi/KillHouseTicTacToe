// Fill out your copyright notice in the Description page of Project Settings.


#include "TicTacToeBoard.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "MainPlayer.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "TicTacToeBoardPiece.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATicTacToeBoard::ATicTacToeBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;

	VertBoardPieceMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VertBoardPiece1"));
	VertBoardPieceMesh1->SetupAttachment(GetRootComponent());

	VertBoardPieceMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VertBoardPiece2"));
	VertBoardPieceMesh2->SetupAttachment(GetRootComponent());

	HorizBoardPieceMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HorizBoardPiece1"));
	HorizBoardPieceMesh1->SetupAttachment(GetRootComponent());

	HorizBoardPieceMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HorizBoardPiece2"));
	HorizBoardPieceMesh2->SetupAttachment(GetRootComponent());

	SlotCollider1 = CreateDefaultSubobject<UBoxComponent>(TEXT("SlotCollider1"));
	SlotCollider1->SetupAttachment(GetRootComponent());

	SlotCollider2 = CreateDefaultSubobject<UBoxComponent>(TEXT("SlotCollider2"));
	SlotCollider2->SetupAttachment(GetRootComponent());

	SlotCollider3 = CreateDefaultSubobject<UBoxComponent>(TEXT("SlotCollider3"));
	SlotCollider3->SetupAttachment(GetRootComponent());

	SlotCollider4 = CreateDefaultSubobject<UBoxComponent>(TEXT("SlotCollider4"));
	SlotCollider4->SetupAttachment(GetRootComponent());

	SlotCollider5 = CreateDefaultSubobject<UBoxComponent>(TEXT("SlotCollider5"));
	SlotCollider5->SetupAttachment(GetRootComponent());

	SlotCollider6 = CreateDefaultSubobject<UBoxComponent>(TEXT("SlotCollider6"));
	SlotCollider6->SetupAttachment(GetRootComponent());

	SlotCollider7 = CreateDefaultSubobject<UBoxComponent>(TEXT("SlotCollider7"));
	SlotCollider7->SetupAttachment(GetRootComponent());

	SlotCollider8 = CreateDefaultSubobject<UBoxComponent>(TEXT("SlotCollider8"));
	SlotCollider8->SetupAttachment(GetRootComponent());

	SlotCollider9 = CreateDefaultSubobject<UBoxComponent>(TEXT("SlotCollider9"));
	SlotCollider9->SetupAttachment(GetRootComponent());

	Player1ID = 0;
	Player2ID = 0;

	bPlayer1IDAssigned = false;
	bPlayer2IDAssigned = false;

	bReplicates = true;
	bSlot1Active = false;
	bSlot2Active = false;
	bSlot3Active = false;
	bSlot4Active = false;
	bSlot5Active = false;
	bSlot6Active = false;
	bSlot7Active = false;
	bSlot8Active = false;
	bSlot9Active = false;

	bRow1Finished = false;
	bRow2Finished = false;
	bRow3Finished = false;
	bColumn1Finished = false;
	bColumn2Finished = false;
	bColumn3Finished = false;
	bDiagonal1Finished = false;
	bDiagonal2Finished = false;
	bTieGame = false;

	bGameActive = false;
}

// Called when the game starts or when spawned
void ATicTacToeBoard::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority) 
	{
		SlotCollider1->OnComponentBeginOverlap.AddDynamic(this, &ATicTacToeBoard::OnOverlapBegin);
		SlotCollider1->OnComponentEndOverlap.AddDynamic(this, &ATicTacToeBoard::OnOverlapEnd);

		SlotCollider2->OnComponentBeginOverlap.AddDynamic(this, &ATicTacToeBoard::OnOverlapBegin);
		SlotCollider2->OnComponentEndOverlap.AddDynamic(this, &ATicTacToeBoard::OnOverlapEnd);

		SlotCollider3->OnComponentBeginOverlap.AddDynamic(this, &ATicTacToeBoard::OnOverlapBegin);
		SlotCollider3->OnComponentEndOverlap.AddDynamic(this, &ATicTacToeBoard::OnOverlapEnd);

		SlotCollider4->OnComponentBeginOverlap.AddDynamic(this, &ATicTacToeBoard::OnOverlapBegin);
		SlotCollider4->OnComponentEndOverlap.AddDynamic(this, &ATicTacToeBoard::OnOverlapEnd);

		SlotCollider5->OnComponentBeginOverlap.AddDynamic(this, &ATicTacToeBoard::OnOverlapBegin);
		SlotCollider5->OnComponentEndOverlap.AddDynamic(this, &ATicTacToeBoard::OnOverlapEnd);

		SlotCollider6->OnComponentBeginOverlap.AddDynamic(this, &ATicTacToeBoard::OnOverlapBegin);
		SlotCollider6->OnComponentEndOverlap.AddDynamic(this, &ATicTacToeBoard::OnOverlapEnd);

		SlotCollider7->OnComponentBeginOverlap.AddDynamic(this, &ATicTacToeBoard::OnOverlapBegin);
		SlotCollider7->OnComponentEndOverlap.AddDynamic(this, &ATicTacToeBoard::OnOverlapEnd);

		SlotCollider8->OnComponentBeginOverlap.AddDynamic(this, &ATicTacToeBoard::OnOverlapBegin);
		SlotCollider8->OnComponentEndOverlap.AddDynamic(this, &ATicTacToeBoard::OnOverlapEnd);

		SlotCollider9->OnComponentBeginOverlap.AddDynamic(this, &ATicTacToeBoard::OnOverlapBegin);
		SlotCollider9->OnComponentEndOverlap.AddDynamic(this, &ATicTacToeBoard::OnOverlapEnd);
	}
}

// Called every frame
void ATicTacToeBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATicTacToeBoard::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if (GetLocalRole() == ROLE_Authority) 
	{
		AMainPlayer* Main = Cast<AMainPlayer>(OtherActor);

		if (Main) 
		{
			// TEMPORARY, NEED TO MAKE AN OBJECT THAT FEEDS THIS OBJECT THE IDS, LIKE A BUTTON.

			if (Main->GetUniqueID() != Player1ID && !bPlayer1IDAssigned)
			{
				UE_LOG(LogTemp, Warning, TEXT("Player 1 ID has been assigned!"))
					Player1ID = Main->GetUniqueID();
				bPlayer1IDAssigned = true;

				Main->ResetGameStatus();
			}
			else if (Main->GetUniqueID() != Player2ID && !bPlayer2IDAssigned && Player1ID != Main->GetUniqueID())
			{
				UE_LOG(LogTemp, Warning, TEXT("Player 2 ID has been assigned!"))
					Player2ID = Main->GetUniqueID();
				bPlayer2IDAssigned = true;

				Main->ResetGameStatus();
			}

			//if(!bPlayer1IDAssigned || !bPlayer2IDAssigned)
			//	SetPlayerID(Main->GetUniqueID());

			
			// TEMPORARY, NEED TO MAKE AN OBJECT THAT FEEDS THIS OBJECT THE IDS, LIKE A BUTTON.




			UE_LOG(LogTemp, Warning, TEXT("Overlap Begin!"))

			if (OverlappedComponent->GetUniqueID() == SlotCollider1->GetUniqueID() && !bSlot1Active)
			{
				UE_LOG(LogTemp, Warning, TEXT("SLOT1 COLLIDED WITH"))
				ActivateSlot(SlotCollider1, Main, 1);
			}

			if (OverlappedComponent->GetUniqueID() == SlotCollider2->GetUniqueID() && !bSlot2Active)
			{
				UE_LOG(LogTemp, Warning, TEXT("SLOT2 COLLIDED WITH"))
				ActivateSlot(SlotCollider2, Main, 2);
			}

			if (OverlappedComponent->GetUniqueID() == SlotCollider3->GetUniqueID() && !bSlot3Active)
			{
				UE_LOG(LogTemp, Warning, TEXT("SLOT3 COLLIDED WITH"))
				ActivateSlot(SlotCollider3, Main, 3);
			}

			if (OverlappedComponent->GetUniqueID() == SlotCollider4->GetUniqueID() && !bSlot4Active)
			{
				UE_LOG(LogTemp, Warning, TEXT("SLOT4 COLLIDED WITH"))
				ActivateSlot(SlotCollider4, Main, 4);
			}

			if (OverlappedComponent->GetUniqueID() == SlotCollider5->GetUniqueID() && !bSlot5Active)
			{
				UE_LOG(LogTemp, Warning, TEXT("SLOT5 COLLIDED WITH"))
				ActivateSlot(SlotCollider5, Main, 5);
			}

			if (OverlappedComponent->GetUniqueID() == SlotCollider6->GetUniqueID() && !bSlot6Active)
			{
				UE_LOG(LogTemp, Warning, TEXT("SLOT6 COLLIDED WITH"))
				ActivateSlot(SlotCollider1, Main, 6);
			}

			if (OverlappedComponent->GetUniqueID() == SlotCollider7->GetUniqueID() && !bSlot7Active)
			{
				// Can probably shove this repeated code into one function with signature (UBoxComponent, AMainPlayer, bool, bool)
				UE_LOG(LogTemp, Warning, TEXT("SLOT7 COLLIDED WITH"))
				ActivateSlot(SlotCollider7, Main, 7);
			}

			if (OverlappedComponent->GetUniqueID() == SlotCollider8->GetUniqueID() && !bSlot8Active)
			{
				UE_LOG(LogTemp, Warning, TEXT("SLOT8 COLLIDED WITH"))
				ActivateSlot(SlotCollider8, Main, 8);
			}

			if (OverlappedComponent->GetUniqueID() == SlotCollider9->GetUniqueID() && !bSlot9Active)
			{
				UE_LOG(LogTemp, Warning, TEXT("SLOT9 COLLIDED WITH"))
				ActivateSlot(SlotCollider9, Main, 9);
			}

			CheckBoard(Main);
		}
	}
}

void ATicTacToeBoard::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{
	if (GetLocalRole() == ROLE_Authority) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap End!"))
	}
}

void ATicTacToeBoard::CheckBoard(AMainPlayer* Main) 
{
	if (Main) 
	{
		if (Main->bSlot1Active && Main->bSlot2Active && Main->bSlot3Active && !Main->bRow1Finished)
		{
			Main->bRow1Finished = true;
		}
		else if (Main->bSlot4Active && Main->bSlot5Active && Main->bSlot6Active && !Main->bRow2Finished)
		{
			Main->bRow2Finished = true;
		}
		else if (Main->bSlot7Active && Main->bSlot8Active && Main->bSlot9Active && !Main->bRow3Finished)
		{
			Main->bRow3Finished = true;
		}
		else if (Main->bSlot1Active && Main->bSlot4Active && Main->bSlot7Active && !Main->bColumn1Finished)
		{
			Main->bColumn1Finished = true;
		}
		else if (Main->bSlot2Active && Main->bSlot5Active && Main->bSlot8Active && !Main->bColumn2Finished)
		{
			Main->bColumn2Finished = true;
		}
		else if (Main->bSlot3Active && Main->bSlot6Active && Main->bSlot9Active && !Main->bColumn3Finished)
		{
			Main->bColumn3Finished = true;
		}
		else if (Main->bSlot1Active && Main->bSlot5Active && Main->bSlot9Active && !Main->bDiagonal1Finished)
		{
			Main->bDiagonal1Finished = true;
		}
		else if (Main->bSlot3Active && Main->bSlot5Active && Main->bSlot7Active && !Main->bDiagonal2Finished)
		{
			Main->bDiagonal2Finished = true;
		}

		if (Main->bRow1Finished || Main->bRow2Finished || Main->bRow3Finished || Main->bColumn1Finished || Main->bColumn2Finished || Main->bColumn3Finished
			|| Main->bDiagonal1Finished || Main->bDiagonal2Finished) 
		{
			AwardPlayer(Main);
			ResetGame();

		}
	}
}

void ATicTacToeBoard::AwardPlayer(AMainPlayer* Main) 
{
	if (Main) 
	{
		UE_LOG(LogTemp, Warning, TEXT("AWARDING POINTS"))
		Main->PointsEarned += 5;
		Main->Wins++;
	}
}

void ATicTacToeBoard::ResetGame_Implementation()
{
	DestroyBoardPieces();
		bSlot1Active = false;
		bSlot2Active = false;
		bSlot3Active = false;
		bSlot4Active = false;
		bSlot5Active = false;
		bSlot6Active = false;
		bSlot7Active = false;
		bSlot8Active = false;
		bSlot9Active = false;

		bRow1Finished = false;
		bRow2Finished = false;
		bRow3Finished = false;
		bColumn1Finished = false;
		bColumn2Finished = false;
		bColumn3Finished = false;
		bDiagonal1Finished = false;
		bDiagonal2Finished = false;
		bTieGame = false;

		bGameActive = false;
		Player1ID = 0;
		Player2ID = 0;
		bPlayer1IDAssigned = false;
		bPlayer2IDAssigned = false;
}


void ATicTacToeBoard::SpawnPiece(UBoxComponent* ActiveSlotCollider, const FVector& Location, uint32 ActivePlayerID)
{
	if (GetLocalRole() == ROLE_Authority) 
	{
		UE_LOG(LogTemp, Warning, TEXT("SLOT1 ATTEMPTING TO SPAWN"))
		if (BoardPieceToSpawn1 && BoardPieceToSpawn2) 
		{
			UWorld* World = GetWorld();
			FActorSpawnParameters SpawnParams;

			if (World) 
			{
				if(ActivePlayerID == Player1ID)
					ATicTacToeBoardPiece* BoardPieceSpawned = World->SpawnActor<ATicTacToeBoardPiece>(BoardPieceToSpawn1, Location, FRotator(0.f), SpawnParams);
				else if(ActivePlayerID == Player2ID)
					ATicTacToeBoardPiece* BoardPieceSpawned = World->SpawnActor<ATicTacToeBoardPiece>(BoardPieceToSpawn2, Location, FRotator(0.f), SpawnParams);
				UE_LOG(LogTemp, Warning, TEXT("SLOT1 SPAWNED A PIECE!"))
			}
		}
	}
}


FVector ATicTacToeBoard::GetSpawnPoint(UBoxComponent* ActiveSlotCollider) 
{
	FVector Extent = ActiveSlotCollider->GetScaledBoxExtent();
	FVector Origin = ActiveSlotCollider->GetComponentLocation();

	FVector Point = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);

	return Point;
}

void ATicTacToeBoard::ActivateSlot(UBoxComponent* ActiveSlotCollider, AMainPlayer* Main, uint32 SlotNumber) 
{
	UE_LOG(LogTemp, Warning, TEXT("SLOT7 COLLIDED WITH"))
	SpawnPiece(ActiveSlotCollider, GetSpawnPoint(ActiveSlotCollider), Main->GetUniqueID());
	SlotActivated(Main, SlotNumber);
}

void ATicTacToeBoard::SlotActivated_Implementation(AMainPlayer* Main, uint32 SlotNumber)
{
	if (Main->GetUniqueID() == Player1ID) 
	{
		Main->PieceColor = FLinearColor(0.f, 0.f, 1.f, 1.f);
	}
	else if (Main->GetUniqueID() == Player2ID)
	{
		Main->PieceColor = FLinearColor(1.f, 0.f, 0.f, 1.f);
	}

	if (SlotNumber == 1)
	{
		bSlot1Active = true;
		Main->bSlot1Active = true;
		Slot1Color = Main->PieceColor;
	}
	else if (SlotNumber == 2)
	{
		bSlot2Active = true;
		Main->bSlot2Active = true;
		Slot2Color = Main->PieceColor;
	}
	else if (SlotNumber == 3)
	{
		bSlot3Active = true;
		Main->bSlot3Active = true;
		Slot3Color = Main->PieceColor;
	}
	else if (SlotNumber == 4)
	{
		bSlot4Active = true;
		Main->bSlot4Active = true;
		Slot4Color = Main->PieceColor;
	}
	else if (SlotNumber == 5)
	{
		bSlot5Active = true;
		Main->bSlot5Active = true;
		Slot5Color = Main->PieceColor;
	}
	else if (SlotNumber == 6)
	{
		bSlot6Active = true;
		Main->bSlot6Active = true;
		Slot6Color = Main->PieceColor;
	}
	else if (SlotNumber == 7)
	{
		bSlot7Active = true;
		Main->bSlot7Active = true;
		Slot7Color = Main->PieceColor;
	}
	else if (SlotNumber == 8)
	{
		bSlot8Active = true;
		Main->bSlot8Active = true;
		Slot8Color = Main->PieceColor;
	}
	else if (SlotNumber == 9)
	{
		bSlot9Active = true;
		Main->bSlot9Active = true;
		Slot9Color = Main->PieceColor;
	}
}

void ATicTacToeBoard::SetPlayerID_Implementation(uint32 MainPlayerID) 
{
	if (MainPlayerID != Player1ID && !bPlayer1IDAssigned)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player 1 ID has been assigned!"))
		Player1ID = MainPlayerID;
		bPlayer1IDAssigned = true;
	}
	else if (MainPlayerID != Player2ID && !bPlayer2IDAssigned && Player1ID != MainPlayerID)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player 2 ID has been assigned!"))
		Player2ID = MainPlayerID;
		bPlayer2IDAssigned = true;
	}
}

void ATicTacToeBoard::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate the following variables
	DOREPLIFETIME(ATicTacToeBoard, Slot1Color);
	DOREPLIFETIME(ATicTacToeBoard, Slot2Color);
	DOREPLIFETIME(ATicTacToeBoard, Slot3Color);
	DOREPLIFETIME(ATicTacToeBoard, Slot4Color);
	DOREPLIFETIME(ATicTacToeBoard, Slot5Color);
	DOREPLIFETIME(ATicTacToeBoard, Slot6Color);
	DOREPLIFETIME(ATicTacToeBoard, Slot7Color);
	DOREPLIFETIME(ATicTacToeBoard, Slot8Color);
}

void ATicTacToeBoard::OnRep_Slot1Color()
{

}

void ATicTacToeBoard::OnRep_Slot2Color()
{

}

void ATicTacToeBoard::OnRep_Slot3Color()
{

}

void ATicTacToeBoard::OnRep_Slot4Color()
{

}

void ATicTacToeBoard::OnRep_Slot5Color()
{

}

void ATicTacToeBoard::OnRep_Slot6Color()
{

}

void ATicTacToeBoard::OnRep_Slot7Color() 
{

}

void ATicTacToeBoard::OnRep_Slot8Color()
{

}

void ATicTacToeBoard::OnRep_Slot9Color()
{

}