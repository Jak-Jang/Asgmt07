#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Drone_PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class ASGMT_07_API ADrone_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADrone_PlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LiftAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* TiltAction;

	virtual void BeginPlay() override;
};
