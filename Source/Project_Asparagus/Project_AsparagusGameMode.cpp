// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project_AsparagusGameMode.h"
#include "Project_AsparagusPlayerController.h"
#include "Project_AsparagusCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProject_AsparagusGameMode::AProject_AsparagusGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AProject_AsparagusPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}