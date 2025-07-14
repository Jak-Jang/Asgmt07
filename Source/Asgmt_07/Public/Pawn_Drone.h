#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Pawn_Drone.generated.h"

class UCapsuleComponent;
class UArrowComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class ASGMT_07_API APawn_Drone : public APawn
{
	GENERATED_BODY()

public:
	APawn_Drone();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* Capsule;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* Arrow;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed|Drone")
	float MoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed|Drone")
	float RotateSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed|Drone")
	float LiftSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed|Gravity")
	float GravitySpeed;


	bool bInGround;
	float Gravity;
	FRotator CurrentRotation;

	FVector2D DroneMovement;
	FVector2D DroneSight;
	float DroneTilt;
	bool bDroneLift;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void DroneMove(const FInputActionValue& Value);
	void DroneStopMove(const FInputActionValue& Value);
	void DroneLook(const FInputActionValue& Value);
	void DroneStopLook(const FInputActionValue& Value);
	void DroneTilting(const FInputActionValue& Value);
	void DroneStopTilting(const FInputActionValue& Value);
	void DroneLift(const FInputActionValue& Value);
	void DroneStopLift(const FInputActionValue& Value);

};
