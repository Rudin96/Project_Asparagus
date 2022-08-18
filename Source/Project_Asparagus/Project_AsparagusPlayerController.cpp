// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project_AsparagusPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Project_AsparagusCharacter.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AProject_AsparagusPlayerController::AProject_AsparagusPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AProject_AsparagusPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void AProject_AsparagusPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();
}

void AProject_AsparagusPlayerController::BeginPlay()
{
	Super::BeginPlay();
	InitCameraSetup();
}

void AProject_AsparagusPlayerController::InitCameraSetup()
{
	SetViewTargetWithBlend(UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass()), 1.0f, VTBlend_Linear, 0.0f, false);
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerInitCameraSetup();
	}
}

bool AProject_AsparagusPlayerController::ServerInitCameraSetup_Validate()
{
	return true;
}

void AProject_AsparagusPlayerController::ServerInitCameraSetup_Implementation()
{
	InitCameraSetup();
}