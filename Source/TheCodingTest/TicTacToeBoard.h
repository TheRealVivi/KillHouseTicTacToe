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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board Piece Flags")
	bool bSlot1Active;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board Piece Flags")
	bool bSlot2Active;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board Piece Flags")
	bool bSlot3Active;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board Piece Flags")
	bool bSlot4Active;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board Piece Flags")
	bool bSlot5Active;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board Piece Flags")
	bool bSlot6Active;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board Piece Flags")
	bool bSlot7Active;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board Piece Flags")
	bool bSlot8Active;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board Piece Flags")
	bool bSlot9Active;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board Piece Flags")
	bool bRow1Finished;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board Piece Flags")
	bool bRow2Finished;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board Piece Flags")
	bool bRow3Finished;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board Piece Flags")
	bool bColumn1Finished;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board Piece Flags")
	bool bColumn2Finished;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board Piece Flags")
	bool bColumn3Finished;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board Piece Flags")
	bool bDiagonal1Finished;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board Piece Flags")
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
