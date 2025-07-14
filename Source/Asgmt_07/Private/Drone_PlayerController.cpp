#include "Drone_PlayerController.h"
#include "EnhancedInputSubsystems.h"


ADrone_PlayerController::ADrone_PlayerController()
	: InputMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	LiftAction(nullptr),
	TiltAction(nullptr)
{
}

void ADrone_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				// Player 0
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}