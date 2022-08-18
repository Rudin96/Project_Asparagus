// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Project_AsparagusPlayerController.generated.h"

UCLASS()
class AProject_AsparagusPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AProject_AsparagusPlayerController();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	void InitCameraSetup();

	UFUNCTION(Reliable, Server, WithValidation)
		void ServerInitCameraSetup();
};


