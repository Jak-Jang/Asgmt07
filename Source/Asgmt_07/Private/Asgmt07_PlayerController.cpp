#include "Asgmt07_PlayerController.h"
#include "EnhancedInputSubsystems.h"


AAsgmt07_PlayerController::AAsgmt07_PlayerController() 
	:	InputMappingContext(nullptr),
		MoveAction(nullptr),
		LookAction(nullptr)
{
}

void AAsgmt07_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				// 우선 순위 0
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}