// Fill out your copyright notice in the Description page of Project Settings.


#include "Elevator.h"

// Sets default values
AElevator::AElevator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	StartPoint = FVector(0.f);
	MidPoint = FVector(0.f, 0.f, 430.f);
	EndPoint = FVector(0.f, 0.f, 860.f);
	InterpSpeed = 1.f;
	InterpTime = 1.f;
	bInterping = false;

	bReplicates = true;
}

// Called when the game starts or when spawned
void AElevator::BeginPlay()
{
	Super::BeginPlay();
	
	StartPoint = GetActorLocation();
	EndPoint += StartPoint;
	MidPoint += StartPoint;

	bInterping = false;

	GetWorldTimerManager().SetTimer(InterpTimer, this, &AElevator::ToggleInterping, InterpTime);

	Distance = (EndPoint - StartPoint).Size();
}

// Called every frame
void AElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (bInterping) 
	{
		FVector CurrentLocation = GetActorLocation();
		FVector Interp = FMath::VInterpTo(CurrentLocation, EndPoint, DeltaTime, InterpSpeed);
		SetActorLocation(Interp);

		float DistanceTraveled = (GetActorLocation() - StartPoint).Size();

		if (Distance - DistanceTraveled <= 1.f) 
		{
			ToggleInterping();

			GetWorldTimerManager().SetTimer(InterpTimer, this, &AElevator::ToggleInterping, InterpTime);
			SwapVectors(StartPoint, EndPoint);
		}
	}
}

void AElevator::ToggleInterping() 
{
	bInterping = !bInterping;
}

void AElevator::SwapVectors(FVector& Vector1, FVector& Vector2) 
{
	FVector Temp = Vector1;
	Vector1 = Vector2;
	Vector2 = Temp;
}