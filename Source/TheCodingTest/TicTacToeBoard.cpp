// Fill out your copyright notice in the Description page of Project Settings.


#include "TicTacToeBoard.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "MainPlayer.h"
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
		UE_LOG(LogTemp, Warning, TEXT("Overlap Begin!"))

		if (OverlappedComponent->GetUniqueID() == SlotCollider1->GetUniqueID()) 
		{
			UE_LOG(LogTemp, Warning, TEXT("SLOT1 COLLIDED WITH"))
			SpawnPiece(SlotCollider1);
		}

		if (OverlappedComponent->GetUniqueID() == SlotCollider2->GetUniqueID())
		{
			UE_LOG(LogTemp, Warning, TEXT("SLOT2 COLLIDED WITH"))
		}

		if (OverlappedComponent->GetUniqueID() == SlotCollider3->GetUniqueID())
		{
			UE_LOG(LogTemp, Warning, TEXT("SLOT3 COLLIDED WITH"))
		}

		if (OverlappedComponent->GetUniqueID() == SlotCollider4->GetUniqueID())
		{
			UE_LOG(LogTemp, Warning, TEXT("SLOT4 COLLIDED WITH"))
		}

		if (OverlappedComponent->GetUniqueID() == SlotCollider5->GetUniqueID())
		{
			UE_LOG(LogTemp, Warning, TEXT("SLOT5 COLLIDED WITH"))
		}

		if (OverlappedComponent->GetUniqueID() == SlotCollider6->GetUniqueID())
		{
			UE_LOG(LogTemp, Warning, TEXT("SLOT6 COLLIDED WITH"))
		}

		if (OverlappedComponent->GetUniqueID() == SlotCollider7->GetUniqueID())
		{
			UE_LOG(LogTemp, Warning, TEXT("SLOT7 COLLIDED WITH"))
		}

		if (OverlappedComponent->GetUniqueID() == SlotCollider8->GetUniqueID())
		{
			UE_LOG(LogTemp, Warning, TEXT("SLOT8 COLLIDED WITH"))
		}

		if (OverlappedComponent->GetUniqueID() == SlotCollider9->GetUniqueID())
		{
			UE_LOG(LogTemp, Warning, TEXT("SLOT9 COLLIDED WITH"))
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

void ATicTacToeBoard::ResetGame() 
{
	if (GetLocalRole() == ROLE_Authority) 
	{
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
}

// TODO: MODIFY SO GAME DOES NOT CRASH
void ATicTacToeBoard::SpawnPiece(UBoxComponent* ActiveSlotCollider)
{
	if (GetLocalRole() == ROLE_Authority) 
	{
		UE_LOG(LogTemp, Warning, TEXT("SLOT1 ATTEMPTING TO SPAWN"))
		if (BoardPieceToSpawn) 
		{
			UWorld* World = GetWorld();
			FActorSpawnParameters SpawnParams;

			if (World) 
			{
				//GAME CRASHING BUG. DO NOT KNOW WHY YET.
				//ATicTacToeBoardPiece* BoardPieceSpawned = World->SpawnActor<ATicTacToeBoardPiece>(BoardPieceToSpawn, GetSpawnPoint(ActiveSlotCollider), FRotator(0.f), SpawnParams);
				UE_LOG(LogTemp, Warning, TEXT("SLOT1 SPAWNED A PIECE!"))
			}
		}
	}
}

// TODO: MODIFY SO GAME DOES NOT CRASH
FVector ATicTacToeBoard::GetSpawnPoint(UBoxComponent* ActiveSlotCollider) 
{
	FVector Extent = ActiveSlotCollider->GetScaledBoxExtent();
	FVector Origin = ActiveSlotCollider->GetComponentLocation();

	FVector Point = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);

	return Point;
}