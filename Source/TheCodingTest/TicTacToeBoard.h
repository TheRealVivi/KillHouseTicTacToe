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

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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

	UPROPERTY(ReplicatedUsing = OnRep_Slot1Color, EditAnywhere, BlueprintReadWrite, Category = "Board Pieces")
	FLinearColor Slot1Color;

	UPROPERTY(ReplicatedUsing = OnRep_Slot2Color, EditAnywhere, BlueprintReadWrite, Category = "Board Pieces")
	FLinearColor Slot2Color;

	UPROPERTY(ReplicatedUsing = OnRep_Slot3Color, EditAnywhere, BlueprintReadWrite, Category = "Board Pieces")
	FLinearColor Slot3Color;

	UPROPERTY(ReplicatedUsing = OnRep_Slot4Color, EditAnywhere, BlueprintReadWrite, Category = "Board Pieces")
	FLinearColor Slot4Color;

	UPROPERTY(ReplicatedUsing = OnRep_Slot5Color, EditAnywhere, BlueprintReadWrite, Category = "Board Pieces")
	FLinearColor Slot5Color;

	UPROPERTY(ReplicatedUsing = OnRep_Slot6Color, EditAnywhere, BlueprintReadWrite, Category = "Board Pieces")
	FLinearColor Slot6Color;

	UPROPERTY(ReplicatedUsing = OnRep_Slot7Color, EditAnywhere, BlueprintReadWrite, Category = "Board Pieces")
	FLinearColor Slot7Color;

	UPROPERTY(ReplicatedUsing = OnRep_Slot8Color, EditAnywhere, BlueprintReadWrite, Category = "Board Pieces")
	FLinearColor Slot8Color;

	UPROPERTY(ReplicatedUsing = OnRep_Slot9Color, EditAnywhere, BlueprintReadWrite, Category = "Board Pieces")
	FLinearColor Slot9Color;

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
	//void OnbSlot7ActiveUpdate();

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

	void ActivateSlot(UBoxComponent* ActiveSlotCollider, AMainPlayer* Main, uint32 SlotNumber);

	UFUNCTION(NetMulticast, Reliable)
	void SlotActivated(class AMainPlayer* Main, uint32 SlotNumber);

	UFUNCTION(NetMulticast, Reliable)
	void SetPlayerID(uint32 MainUniqueID);

	UFUNCTION()
	void OnRep_Slot1Color();

	UFUNCTION()
	void OnRep_Slot2Color();
	
	UFUNCTION()
	void OnRep_Slot3Color();
	
	UFUNCTION()
	void OnRep_Slot4Color();

	UFUNCTION()
	void OnRep_Slot5Color();

	UFUNCTION()
	void OnRep_Slot6Color();

	UFUNCTION()
	void OnRep_Slot7Color();

	UFUNCTION()
	void OnRep_Slot8Color();

	UFUNCTION()
	void OnRep_Slot9Color();
};
