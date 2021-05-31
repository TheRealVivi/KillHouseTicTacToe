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
	
	MaxHealthIncrease = 20.f;

}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

void AMainPlayer::Die() 
{
	Health = MaxHealth;
}

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