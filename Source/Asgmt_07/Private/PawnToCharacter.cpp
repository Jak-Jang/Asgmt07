#include "PawnToCharacter.h"
#include "Asgmt07_PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"

APawnToCharacter::APawnToCharacter() : MoveSpeed(600.0f), LookSpeed(100.0f)
{
	bUseControllerRotationYaw = false;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = Capsule;
	Capsule->SetSimulatePhysics(false);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	Arrow->SetupAttachment(Capsule);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMesh->SetupAttachment(Capsule);
	SkeletalMesh->SetSimulatePhysics(false);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(Capsule);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bUsePawnControlRotation = false;
	// SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
}

void APawnToCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!MoveInput.IsNearlyZero())
	{
		AddActorLocalOffset(
			FVector(MoveInput.X * MoveSpeed * DeltaTime, MoveInput.Y * MoveSpeed * DeltaTime, 0.0f),
			true);
	}

	if (!LookInput.IsNearlyZero())
	{
		AddActorLocalRotation(FRotator(0.0f, LookInput.X * LookSpeed * DeltaTime, 0.0f));

		SpringArm->AddLocalRotation(FRotator(LookInput.Y * LookSpeed * DeltaTime, 0.0f, 0.0f));
	}
}

void APawnToCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AAsgmt07_PlayerController* PlayerController
			= Cast<AAsgmt07_PlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction, 
					ETriggerEvent::Triggered, 
					this, 
					&APawnToCharacter::Move);
			}

			if(PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction, 
					ETriggerEvent::Completed, 
					this, 
					&APawnToCharacter::StopMove);
			}

			if(PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction, 
					ETriggerEvent::Triggered, 
					this, 
					&APawnToCharacter::Look);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Completed,
					this,
					&APawnToCharacter::StopLook);
			}
		}
	}
}

void APawnToCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;
	MoveInput = value.Get<FVector2D>();
}
void APawnToCharacter::StopMove(const FInputActionValue& value)
{
	if (!Controller) return;
	MoveInput = FVector2D::ZeroVector;
}
void APawnToCharacter::Look(const FInputActionValue& value)
{
	if (!Controller) return;
	LookInput = value.Get<FVector2D>();
}
void APawnToCharacter::StopLook(const FInputActionValue& value)
{
	if (!Controller) return;
	LookInput = FVector2D::ZeroVector;
}