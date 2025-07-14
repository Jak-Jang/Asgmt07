#include "Pawn_Drone.h"
#include "Drone_PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"

// Sets default values
APawn_Drone::APawn_Drone()
	: MoveSpeed(600.0f),
	RotateSpeed(100.0f),
	LiftSpeed(600.f),
	GravitySpeed(-980.0f),
	bInGround(true),
	Gravity(0.0f),
	CurrentRotation(FRotator::ZeroRotator)
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = Capsule;
	Capsule->SetSimulatePhysics(false);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	Arrow->SetupAttachment(RootComponent);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMesh->SetupAttachment(RootComponent);
	SkeletalMesh->SetSimulatePhysics(false);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
	SpringArm->bUsePawnControlRotation = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;
}

void APawn_Drone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 라인트레이스로 bInGround 판단
	FVector StartLocation = GetActorLocation();
	// 캡슐 아래 10cm
	FVector EndLocation = 
		StartLocation + (FVector::DownVector * (Capsule->GetScaledCapsuleHalfHeight() + 10.0f));
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	// 자기 자신 무시
	QueryParams.AddIgnoredActor(this);
	bool bHitGround =
		GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			ECC_Visibility,
			QueryParams);
	if (bHitGround && FVector::DotProduct(HitResult.Normal, FVector::UpVector) > 0.7f)
		bInGround = true;
	else bInGround = false;

	// Drone Movement
	if (!DroneMovement.IsNearlyZero())
	{
		if (bInGround) 
		{ 
			AddActorLocalOffset(FVector(
				DroneMovement.X * MoveSpeed * DeltaTime, 
				DroneMovement.Y * MoveSpeed * DeltaTime, 
				0.0f), 
				true); 
		}
		else 
		{ 
			AddActorLocalOffset(FVector(
				DroneMovement.X * MoveSpeed * DeltaTime, 
				DroneMovement.Y * MoveSpeed * DeltaTime, 
				0.0f) * 0.5f, 
				true); 
		}
	}

	// Drone Look
	if (!DroneSight.IsNearlyZero())
	{
		AddActorWorldRotation(FRotator(0.0f, DroneSight.X * RotateSpeed * DeltaTime, 0.0f));
		SpringArm->AddLocalRotation(FRotator(DroneSight.Y * RotateSpeed * DeltaTime, 0.0f, 0.0f));
	}

	// Drone Tilt
	if (!FMath::IsNearlyZero(DroneTilt))
	{
		AddActorLocalRotation(FRotator(DroneTilt * RotateSpeed * DeltaTime, 0.0f, 0.0f));
	}

	// Drone Lift
	if (bDroneLift)
	{ 
		AddActorLocalOffset(FVector(0.0f, 0.0f, LiftSpeed * DeltaTime), true);
		Gravity = 0.0f;
	}

	// Gravity
	if (!bDroneLift && !bInGround)
	{
		Gravity += GravitySpeed * DeltaTime;
		AddActorWorldOffset(FVector(0.0f, 0.0f, Gravity * DeltaTime), true);
	}
	else { Gravity = 0.0f; }
}

void APawn_Drone::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ADrone_PlayerController* PlayerController
			= Cast<ADrone_PlayerController>(GetController()))
		{
			// Move Action
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered, 
					this, 
					&APawn_Drone::DroneMove);
			}
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Completed, 
					this, 
					&APawn_Drone::DroneStopMove);
			}
			// Look Action
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&APawn_Drone::DroneLook);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Completed,
					this,
					&APawn_Drone::DroneStopLook);
			}
			// Tilt Action
			if (PlayerController->TiltAction)
			{
				EnhancedInput->BindAction(
					PlayerController->TiltAction,
					ETriggerEvent::Triggered,
					this,
					&APawn_Drone::DroneTilting);
			}
			if (PlayerController->TiltAction)
			{
				EnhancedInput->BindAction(
					PlayerController->TiltAction,
					ETriggerEvent::Completed,
					this,
					&APawn_Drone::DroneStopTilting);
			}
			// Lift Action
			if (PlayerController->LiftAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LiftAction,
					ETriggerEvent::Triggered,
					this,
					&APawn_Drone::DroneLift);
			}
			if (PlayerController->LiftAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LiftAction,
					ETriggerEvent::Completed,
					this,
					&APawn_Drone::DroneStopLift);
			}
		}
	}
}

void APawn_Drone::DroneMove(const FInputActionValue& Value)
{
	if (!Controller) return;
	DroneMovement = Value.Get<FVector2D>();
}
void APawn_Drone::DroneStopMove(const FInputActionValue& Value)
{
	if (!Controller) return;
	DroneMovement = FVector2D::ZeroVector;
}
void APawn_Drone::DroneLook(const FInputActionValue& Value)
{
	if (!Controller) return;
	DroneSight = Value.Get<FVector2D>();
}
void APawn_Drone::DroneStopLook(const FInputActionValue& Value)
{
	if (!Controller) return;
	DroneSight = FVector2D::ZeroVector;
}
void APawn_Drone::DroneTilting(const FInputActionValue& Value)
{
	if (!Controller) return;
	DroneTilt = Value.Get<float>();
}
void APawn_Drone::DroneStopTilting(const FInputActionValue& Value)
{
	if (!Controller) return;
	DroneTilt = 0.0f;
}
void APawn_Drone::DroneLift(const FInputActionValue& Value)
{
	if (!Controller) return;
	bDroneLift = Value.Get<bool>();
}
void APawn_Drone::DroneStopLift(const FInputActionValue& Value)
{
	if (!Controller) return;
	bDroneLift = Value.Get<bool>();
}