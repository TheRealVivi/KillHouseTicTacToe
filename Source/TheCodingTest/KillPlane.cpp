// Fill out your copyright notice in the Description page of Project Settings.


#include "KillPlane.h"
#include "MainPlayer.h"
#include "Components/BoxComponent.h"

// Sets default values
AKillPlane::AKillPlane()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionVolume = CreateDefaultSubobject<UBoxComponent>("CollisionVolume");
	RootComponent = CollisionVolume;
}

// Called when the game starts or when spawned
void AKillPlane::BeginPlay()
{
	Super::BeginPlay();


	if (GetLocalRole() == ROLE_Authority)
	{
		CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AKillPlane::OnOverlapBegin);
	}
}

// Called every frame
void AKillPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Upon collision with the kill plane, kill the character and destroy everything else that touches it.
void AKillPlane::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (GetLocalRole() == ROLE_Authority)
	{
		if (OtherActor)
		{
			AMainPlayer* Main = Cast<AMainPlayer>(OtherActor);
			if (Main)
			{
				Main->Die();
			}
			else
			{
				OtherActor->Destroy();
			}
		}
	}
}