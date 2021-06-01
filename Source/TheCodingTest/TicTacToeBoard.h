// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TicTacToeBoard.generated.h"

UCLASS()
class THECODINGTEST_API ATicTacToeBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATicTacToeBoard();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Structure")
	class USceneComponent* RootSceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Structure")
	class UStaticMeshComponent* VertBoardPieceMesh1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Structure")
	class UStaticMeshComponent* VertBoardPieceMesh2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Structure")
	class UStaticMeshComponent* HorizBoardPieceMesh1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Structure")
	class UStaticMeshComponent* HorizBoardPieceMesh2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	class UBoxComponent* SlotCollider1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	class UBoxComponent* SlotCollider2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	class UBoxComponent* SlotCollider3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	class UBoxComponent* SlotCollider4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	class UBoxComponent* SlotCollider5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	class UBoxComponent* SlotCollider6;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	class UBoxComponent* SlotCollider7;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	class UBoxComponent* SlotCollider8;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	class UBoxComponent* SlotCollider9;

	bool bSlot1Active;
	bool bSlot2Active;
	bool bSlot3Active;
	bool bSlot4Active;
	bool bSlot5Active;
	bool bSlot6Active;
	bool bSlot7Active;
	bool bSlot8Active;
	bool bSlot9Active;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
