// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainPlayer.generated.h"

UCLASS()
class THECODINGTEST_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayer();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowingCamera;

	// Stats
	UPROPERTY(ReplicatedUsing = OnRep_Name, EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	FString Name;

	UPROPERTY(ReplicatedUsing = OnRep_Health, EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stats")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stat Boosts")
	float MaxHealthIncrease;

	UPROPERTY(ReplicatedUsing = OnRep_Stamina, EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Stamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stats")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float StaminaDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MinSprintStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RunningSpeed;

	UPROPERTY(ReplicatedUsing = OnRep_PointsEarned, EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	int32 PointsEarned;

	UPROPERTY(ReplicatedUsing = OnRep_Exp, EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Exp;

	UPROPERTY(ReplicatedUsing = OnRep_MaxExp, EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float MaxExp;

	UPROPERTY(ReplicatedUsing = OnRep_Level, EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
	int32 MaxLevel;

	UPROPERTY(EditAnywhere, Category = Camera)
	float CameraBoomArmLength; // length of camera boom

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Start Point")
	FVector StartPoint; // Where player spawns.

	UPROPERTY(ReplicatedUsing = OnRep_PieceColor, EditAnywhere, BlueprintReadWrite, Category = "Board Pieces")
	FLinearColor PieceColor;

	bool bShiftKeyDown; // is shift key down
	bool bEDown; // is E key down
	bool bLMBDown; // is lmb down
	bool bRMBDown; // is rmb down
	bool bWeaponEquipped; // Does player have a weapon equipped

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

	void OnHealthUpdate();
	void OnStaminaUpdate();
	void OnExpUpdate();
	void OnMaxExpUpdate();
	void OnLevelUpdate();
	void OnNameUpdate();
	void OnPointsEarnedUpdate();
	void OnPieceColorUpdate();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	* For player input controller
	*/
	void MoveForward(float Input);
	void MoveRight(float Input);

	void ShiftKeyDown();
	void ShiftKeyUp();
	UFUNCTION(Server, Reliable) // Server RPC to trigger SHIFT key being pressed
	void ShiftKeyDownServer();
	UFUNCTION(Server, Reliable) // Server RPC to trigger SHIFT key being released
	void ShiftKeyUpServer();

	void EKeyDown();
	void EKeyUp();
	UFUNCTION(Server, Reliable) // Server RPC to trigger E key being pressed
	void EKeyDownServer();
	UFUNCTION(Server, Reliable) // Server RPC to trigger E key being released
	void EKeyUpServer();

	void LMBDown();
	void LMBUp();
	UFUNCTION(Server, Reliable) // Server RPC to trigger LMB key being pressed
	void LMBDownServer();
	UFUNCTION(Server, Reliable) // Server RPC to trigger LMB key being released
	void LMBUpServer();

	void RMBDown();
	void RMBUp();
	UFUNCTION(Server, Reliable) // Server RPC to trigger RMB key being pressed
	void RMBDownServer();
	UFUNCTION(Server, Reliable) // Server RPC to trigger RMB key being released
	void RMBUpServer();

	void Die();
	void RespawnPlayer();


	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowingCamera() const { return FollowingCamera; }

	UFUNCTION()
	void OnRep_Name();

	UFUNCTION()
	void OnRep_Health();

	UFUNCTION()
	void OnRep_Stamina();

	UFUNCTION()
	void OnRep_Exp();

	UFUNCTION()
	void OnRep_MaxExp();

	UFUNCTION()
	void OnRep_Level();

	UFUNCTION()
	void OnRep_PointsEarned();

	UFUNCTION()
	void OnRep_PieceColor();
};
