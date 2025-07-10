#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Asgmt07_PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class ASGMT_07_API AAsgmt07_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAsgmt07_PlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;

	virtual void BeginPlay() override;
};