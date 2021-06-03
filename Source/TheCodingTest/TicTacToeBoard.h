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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board Pieces")
	TSubclassOf<class ATicTacToeBoardPiece> BoardPieceToSpawn1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board Pieces")
	TSubclassOf<class ATicTacToeBoardPiece> BoardPieceToSpawn2;

	uint32 Player1ID;
	uint32 Player2ID;

	bool bPlayer1IDAssigned;
	bool bPlayer2IDAssigned;

	bool bSlot1Active;
	bool bSlot2Active;
	bool bSlot3Active;
	bool bSlot4Active;
	bool bSlot5Active;
	bool bSlot6Active;
	bool bSlot7Active;
	bool bSlot8Active;
	bool bSlot9Active;

	bool bRow1Finished;
	bool bRow2Finished;
	bool bRow3Finished;
	bool bColumn1Finished;
	bool bColumn2Finished;
	bool bColumn3Finished;
	bool bDiagonal1Finished;
	bool bDiagonal2Finished;

	bool bTieGame;
	bool bGameActive;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void CheckBoard(class AMainPlayer* Main);
	void AwardPlayer(class AMainPlayer* Main);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void ResetGame();

	UFUNCTION()
	void SpawnPiece(UBoxComponent* ActiveSlotCollider, const FVector& Location, uint32 ActivePlayerID);

	UFUNCTION()
	FVector GetSpawnPoint(UBoxComponent* ActiveSlotCollider);
};
