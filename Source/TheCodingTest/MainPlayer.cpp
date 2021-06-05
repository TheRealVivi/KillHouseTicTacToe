// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CameraBoomArmLength = 600.f;

	// Creating Camera Boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = CameraBoomArmLength;
	CameraBoom->bUsePawnControlRotation = true;

	// Setting default capsule component size
	GetCapsuleComponent()->SetCapsuleSize(34.13f, 91.2f);

	// Creating Camera
	FollowingCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowingCamera"));
	FollowingCamera->SetupAttachment(GetCameraBoom(), USpringArmComponent::SocketName);

	// Attach the camera to the end of the boom and let the boom adjust to match
	// the controller orientation
	FollowingCamera->bUsePawnControlRotation = false;

	PieceColor = FLinearColor(0.f, 0.f, 0.f, 0.f);

	// Don't rotate when the player rotates (Camera rotates only)
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// Default values for bools;
	bShiftKeyDown = false;
	bEDown = false;
	bLMBDown = false;
	bRMBDown = false;
	bWeaponEquipped = false;
	bReplicates = true;
	bAlwaysRelevant = true;

	// Defaults for character movement controller
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->AirControl = .2f;

	// Default stats for the player
	Name = FString("Player");
	MaxHealth = 100.f;
	Health = MaxHealth;
	Exp = 0.f;
	MaxExp = 100.f;
	PointsEarned = 0;
	Stamina = 100.f;
	MaxStamina = 150.f;
	StaminaDrainRate = 15.f;
	MinSprintStamina = 50.f;
	WalkingSpeed = 650.f;
	RunningSpeed = 950.f;
	Level = 1;
	MaxLevel = 99;
	Wins = 0;
	
	MaxHealthIncrease = 20.f;

}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// Get the point that the character spawns at (for respawning purposes)
	StartPoint = FVector(GetActorLocation());
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float DeltaStamina = StaminaDrainRate * GetWorld()->DeltaTimeSeconds;

	/*****************************************************************
	/*  SPRINTING MECHANIC
	/*   - Switch case dependent on Stamina status.
	/*   - If bShiftKeyDown is true, stamina will reduce.
	/*   - If stamina falls below certain thresholds, StaminaStatus
	/*     switches to respective states.
	/*   - When bShiftKeyDown is true, and stamina is not zero,
	/*     MovementStatus switches to sprinting.
	/*   -
	******************************************************************/
	switch (StaminaStatus)
	{
	case EStaminaStatus::ESS_Normal:
		if (bShiftKeyDown) // Sprinting
		{
			if (GetStamina() - DeltaStamina <= MinSprintStamina)
			{
				// Stamina has fell below minimum value, change state (turns stamina bar yellow)
				if (IsLocallyControlled())  // Client controlled
					SetStaminaStatusServer(EStaminaStatus::ESS_BelowMinimum);

				if (GetLocalRole() == ROLE_Authority) // Server controlled
					SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);

				SetStamina(GetStamina() - DeltaStamina);
			}
			else
			{
				SetStamina(GetStamina() - DeltaStamina);
			}
			if (IsLocallyControlled())
				SetMovementStatusServer(EMovementStatus::EMS_Sprinting);

			if (GetLocalRole() == ROLE_Authority)
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
		}
		else // Shift key up
		{
			if (GetStamina() + DeltaStamina >= MaxStamina)
			{
				SetStamina(MaxStamina);
			}
			else
			{
				SetStamina(GetStamina() + DeltaStamina);
			}

			if (IsLocallyControlled())
				SetMovementStatusServer(EMovementStatus::EMS_Normal);

			if (GetLocalRole() == ROLE_Authority)
				SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;
	case EStaminaStatus::ESS_BelowMinimum:
		if (bShiftKeyDown)  // Sprinting
		{
			if (GetStamina() - DeltaStamina <= 0.f)
			{
				// If stamina runs out, stop running and turn state to exhausted state.
				if (IsLocallyControlled())
					SetStaminaStatusServer(EStaminaStatus::ESS_Exhausted);

				if (GetLocalRole() == ROLE_Authority)
					SetStaminaStatus(EStaminaStatus::ESS_Exhausted);

				SetStamina(0.f);
				if (IsLocallyControlled())
					SetMovementStatusServer(EMovementStatus::EMS_Normal);

				if (GetLocalRole() == ROLE_Authority)
					SetMovementStatus(EMovementStatus::EMS_Normal);
			}
			else
			{
				SetStamina(GetStamina() - DeltaStamina);

				if (IsLocallyControlled())
					SetMovementStatusServer(EMovementStatus::EMS_Sprinting);

				if (GetLocalRole() == ROLE_Authority)
					SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}
		}
		else // shift key up
		{
			if (GetStamina() + DeltaStamina >= MinSprintStamina)
			{

				if (IsLocallyControlled())
					SetStaminaStatusServer(EStaminaStatus::ESS_Normal);

				if (GetLocalRole() == ROLE_Authority)
					SetStaminaStatus(EStaminaStatus::ESS_Normal);

				SetStamina(GetStamina() + DeltaStamina);
			}
			else
			{
				SetStamina(GetStamina() + DeltaStamina);
			}

			if (IsLocallyControlled())
				SetMovementStatusServer(EMovementStatus::EMS_Normal);

			if (GetLocalRole() == ROLE_Authority)
				SetMovementStatus(EMovementStatus::EMS_Normal);
		}

		break;
	case EStaminaStatus::ESS_Exhausted:
		if (bShiftKeyDown) // trying to sprint, but no stamina
		{
			SetStamina(0.f);
		}
		else // shift key up
		{
			// Start regenerating stamina, cannot sprint til stamina above minimum value.
			if (IsLocallyControlled())
				SetStaminaStatusServer(EStaminaStatus::ESS_ExhaustedRecovering);

			if (GetLocalRole() == ROLE_Authority)
				SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
			SetStamina(GetStamina() + DeltaStamina);
		}

		if (IsLocallyControlled())
			SetMovementStatusServer(EMovementStatus::EMS_Normal);
		if (GetLocalRole() == ROLE_Authority)
			SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	case EStaminaStatus::ESS_ExhaustedRecovering:
		if (GetStamina() + DeltaStamina >= MinSprintStamina) // Regenerating stamina, cannot sprint til stamina above minimum value.
		{
			if (IsLocallyControlled())
				SetStaminaStatusServer(EStaminaStatus::ESS_Normal);
			if (GetLocalRole() == ROLE_Authority)
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
		}
		else
		{
			SetStamina(GetStamina() + DeltaStamina);
		}

		if (IsLocallyControlled())
			SetMovementStatusServer(EMovementStatus::EMS_Normal);

		if (GetLocalRole() == ROLE_Authority)
			SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	default:
		;
	}



}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMainPlayer::MoveRight);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMainPlayer::Jump);
	PlayerInputComponent->BindAction(TEXT("MoveRight"), EInputEvent::IE_Released, this, &AMainPlayer::StopJumping);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMainPlayer::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMainPlayer::AddControllerPitchInput);

	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AMainPlayer::ShiftKeyDown);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AMainPlayer::ShiftKeyUp);

	PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &AMainPlayer::EKeyDown);
	PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Released, this, &AMainPlayer::EKeyUp);

	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &AMainPlayer::LMBDown);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Released, this, &AMainPlayer::LMBUp);
}

// Allows player to move forward and backward
void AMainPlayer::MoveForward(float Input) 
{
	if (Controller != nullptr && Input != 0.0f) 
	{
		// Find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Input);
	}
}

// Allows player to move left and right
void AMainPlayer::MoveRight(float Input) 
{
	if (Controller != nullptr && Input != 0.f) 
	{
		// Find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Input);
	}
}

/**
/*  If the player is a client, then trigger server RPC
/*  Otherwise, if the player is the server, set bShiftKeyDown to true
*/
void AMainPlayer::ShiftKeyDown() 
{
	if (IsLocallyControlled()) // Client
	{
		ShiftKeyDownServer(); // RPC
	}

	if (GetLocalRole() == ROLE_Authority) // Server
		bShiftKeyDown = true;
}

// Server RPC for client to press shift key
void AMainPlayer::ShiftKeyDownServer_Implementation() 
{
	bShiftKeyDown = true;
}

/**
/*  If the player is a client, then trigger server RPC
/*  Otherwise, if the player is the server, set bShiftKeyDown to false
*/
void AMainPlayer::ShiftKeyUp() 
{
	if (IsLocallyControlled()) // Client
	{
		ShiftKeyUpServer(); // RPC
	}

	if (GetLocalRole() == ROLE_Authority) // Server
		bShiftKeyDown = false;
}

// Server RPC for client to press shift key
void AMainPlayer::ShiftKeyUpServer_Implementation()
{
	bShiftKeyDown = false;
}

/**
/*  If the player is a client, then trigger server RPC
/*  Otherwise, if the player is the server, set bEDown to true and equip weapon if overlapping.
*/
void AMainPlayer::EKeyDown()
{
	if (IsLocallyControlled()) // Client
	{
		EKeyDownServer(); // RPC
	}

	if (GetLocalRole() == ROLE_Authority) // Server
		bEDown = true;
}

// Server RPC for client to press E key
void AMainPlayer::EKeyDownServer_Implementation()
{
	bEDown = true;
}

/**
/*  If the player is a client, then trigger server RPC
/*  Otherwise, if the player is the server, set bEDown to false.
*/
void AMainPlayer::EKeyUp()
{
	if (IsLocallyControlled()) // Client
	{
		EKeyUpServer(); // RPC
	}

	if (GetLocalRole() == ROLE_Authority) // Server
		bEDown = false;
}

// Server RPC for client to press E key
void AMainPlayer::EKeyUpServer_Implementation()
{
	bEDown = false;
}


/**
/*  If the player is a client, then trigger server RPC
/*  Otherwise, if the player is the server, set bLMBDown to true and attack with weapon.
*/
void AMainPlayer::LMBDown()
{
	if (IsLocallyControlled()) // Client
	{
		LMBDownServer(); // RPC
	}

	if (GetLocalRole() == ROLE_Authority) // Server
		bLMBDown = true;
}

// Server RPC for client to press E key
void AMainPlayer::LMBDownServer_Implementation()
{
	bLMBDown = true;
}


/**
/*  If the player is a client, then trigger server RPC
/*  Otherwise, if the player is the server, set bLMBDown to false.
*/
void AMainPlayer::LMBUp()
{
	if (IsLocallyControlled()) // Client
	{
		LMBUpServer(); // RPC
	}

	if (GetLocalRole() == ROLE_Authority) // Server
		bLMBDown = false;
}

// Server RPC for client to press E key
void AMainPlayer::LMBUpServer_Implementation()
{
	bLMBDown = false;
}

/**
/*  If the player is a client, then trigger server RPC
/*  Otherwise, if the player is the server, set bRMBDown to true and attack with weapon.
*/
void AMainPlayer::RMBDown()
{
	if (IsLocallyControlled()) // Client
	{
		RMBDownServer(); // RPC
	}

	if (GetLocalRole() == ROLE_Authority) // Server
		bRMBDown = true;
}

// Server RPC for client to press E key
void AMainPlayer::RMBDownServer_Implementation()
{
	bRMBDown = true;
}

/**
/*  If the player is a client, then trigger server RPC
/*  Otherwise, if the player is the server, set bRMBDown to false.
*/
void AMainPlayer::RMBUp()
{
	if (IsLocallyControlled()) // Client
	{
		RMBUpServer(); // RPC
	}

	if (GetLocalRole() == ROLE_Authority) // Server
		bRMBDown = false;
}

// Server RPC for client to press E key
void AMainPlayer::RMBUpServer_Implementation()
{
	bRMBDown = false;
}

// If player dies, set health to max, items to zero, respawn at start point.
void AMainPlayer::Die()
{
	SetHealth(GetMaxHealth());
	SendToStartPoint();
}

// Sends player to where they initially started.
void AMainPlayer::SendToStartPoint()
{
	SetActorLocation(StartPoint);
}

void AMainPlayer::ResetGameStatus()
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
}

// Increases player exp
// If new exp amount is greater than current max exp, then level up
void AMainPlayer::IncreaseExp(float ExpAmount) 
{
	float NewExpAmount = ExpAmount + GetExp();
	if (NewExpAmount >= GetMaxExp()) // More than enough exp to level up!
	{
		LevelUp(ExpAmount - (GetMaxExp() - GetExp()));
	}
	else // Just give exp
	{
		SetExp(NewExpAmount);
	}
}

/**
* Increases players level by 1, and gives remaining exp amount
* If Player has exceeded the max level, then just give exp.
*/
void AMainPlayer::LevelUp(float RemainingExpAmount) 
{
	int32 NextLevel = GetLevel() + 1;
	if (GetLevel() < GetMaxLevel()) 
	{
		SetLevel(NextLevel);
		SetExp(RemainingExpAmount);
		SetMaxExp(GetLevel() * GetMaxExp());
	}
	else 
	{
		SetExp(RemainingExpAmount);
	}
}

// if remaining health is greater than zero, then set health to remaining health
// Otherwise the player dies
void AMainPlayer::DecrementHealth(float DamageApplied)
{
	float RemainingHealth = Health - DamageApplied;
	if (RemainingHealth > 0.f)
	{
		SetHealth(RemainingHealth);
	}
	else
	{
		Die();
	}
}

// Setters and Getters

// Server RPC to set StaminaStatus
void AMainPlayer::SetStaminaStatusServer_Implementation(EStaminaStatus Status)
{
	StaminaStatus = Status;
}

void AMainPlayer::SetStaminaStatus(EStaminaStatus Status)
{
	StaminaStatus = Status;
}

// Server RPC for client to set MovementStatus
void AMainPlayer::SetMovementStatusServer_Implementation(EMovementStatus Status)
{
	// if in Sprinting state, increase max walk speed, else, maintain running speed as is.
	MovementStatus = Status;
	if (MovementStatus == EMovementStatus::EMS_Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;

	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
	}
}

// if in Sprinting state, increase max walk speed, else, maintain running speed as is.
void AMainPlayer::SetMovementStatus(EMovementStatus Status)
{
	// if in Sprinting state, increase max walk speed, else, maintain running speed as is.
	MovementStatus = Status;
	if (MovementStatus == EMovementStatus::EMS_Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;

	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
	}
}

void AMainPlayer::SetHealth(float HealthValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		Health = FMath::Clamp(HealthValue, 0.f, MaxHealth);
		OnHealthUpdate(); // Parallels server and client
	}
}

void AMainPlayer::SetStamina(float StaminaValue)
{
	if (GetLocalRole() == ROLE_Authority || IsLocallyControlled())
	{
		Stamina = FMath::Clamp(StaminaValue, 0.f, MaxStamina);
		OnStaminaUpdate(); // Parallels server and client
	}
}


void AMainPlayer::SetExp(float ExpAmount) 
{
	if (GetLocalRole() == ROLE_Authority) 
	{
		Exp = ExpAmount;
		OnExpUpdate();
	}
}

void AMainPlayer::SetMaxExp(float MaxExpValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		MaxExp = MaxExpValue;
		OnMaxExpUpdate();
	}
}


void AMainPlayer::SetLevel(int32 NextLevel) 
{
	if (GetLocalRole() == ROLE_Authority)
	{
		Level = NextLevel;
		OnLevelUpdate();
	}
}

void AMainPlayer::SetPointsEarned(int32 InPointsEarned) 
{
	if (GetLocalRole() == ROLE_Authority)
	{
		PointsEarned = InPointsEarned;
		OnPointsEarnedUpdate();
	}
}


void AMainPlayer::SetWins(int32 InWins)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		Wins = InWins;
		OnWinsUpdate();
	}
}

// *********End Setters and Getters************



void AMainPlayer::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate the following variables
	DOREPLIFETIME(AMainPlayer, Name);
	DOREPLIFETIME(AMainPlayer, Health);
	DOREPLIFETIME(AMainPlayer, Stamina);
	DOREPLIFETIME(AMainPlayer, Level);
	DOREPLIFETIME(AMainPlayer, Exp);
	DOREPLIFETIME(AMainPlayer, MaxExp);
	DOREPLIFETIME(AMainPlayer, PointsEarned);
	DOREPLIFETIME(AMainPlayer, PieceColor);
	DOREPLIFETIME(AMainPlayer, Wins);
}

void AMainPlayer::OnNameUpdate() 
{
	if (IsLocallyControlled()) 
	{

	}

	if (GetLocalRole() == ROLE_Authority) 
	{

	}
}

void AMainPlayer::OnHealthUpdate()
{
	if (IsLocallyControlled())
	{

	}

	if (GetLocalRole() == ROLE_Authority)
	{

	}
}

void AMainPlayer::OnStaminaUpdate()
{
	if (IsLocallyControlled())
	{

	}

	if (GetLocalRole() == ROLE_Authority)
	{

	}
}

void AMainPlayer::OnExpUpdate()
{
	if (IsLocallyControlled())
	{

	}

	if (GetLocalRole() == ROLE_Authority)
	{

	}
}

void AMainPlayer::OnMaxExpUpdate()
{
	if (IsLocallyControlled())
	{

	}

	if (GetLocalRole() == ROLE_Authority)
	{

	}
}

void AMainPlayer::OnLevelUpdate()
{
	if (IsLocallyControlled())
	{

	}

	if (GetLocalRole() == ROLE_Authority)
	{

	}
}

void AMainPlayer::OnPointsEarnedUpdate()
{
	if (IsLocallyControlled())
	{

	}

	if (GetLocalRole() == ROLE_Authority)
	{

	}
}

void AMainPlayer::OnPieceColorUpdate()
{
	if (IsLocallyControlled())
	{

	}

	if (GetLocalRole() == ROLE_Authority)
	{

	}
}

void AMainPlayer::OnWinsUpdate() 
{
	if (IsLocallyControlled())
	{

	}

	if (GetLocalRole() == ROLE_Authority)
	{

	}
}


void AMainPlayer::OnRep_Name()
{
	OnNameUpdate();
}

void AMainPlayer::OnRep_Health() 
{
	OnHealthUpdate();
}

void AMainPlayer::OnRep_Stamina()
{
	OnStaminaUpdate();
}

void AMainPlayer::OnRep_Exp()
{
	OnExpUpdate();
}

void AMainPlayer::OnRep_MaxExp()
{
	OnMaxExpUpdate();
}

void AMainPlayer::OnRep_Level()
{
	OnLevelUpdate();
}

void AMainPlayer::OnRep_PointsEarned()
{
	OnPointsEarnedUpdate();
}

void AMainPlayer::OnRep_PieceColor() 
{
	OnPieceColorUpdate();
}

void AMainPlayer::OnRep_Wins() 
{
	OnWinsUpdate();
}
