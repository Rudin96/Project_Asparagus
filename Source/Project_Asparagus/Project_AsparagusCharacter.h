// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Project_AsparagusCharacter.generated.h"

UCLASS(Blueprintable)
class AProject_AsparagusCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AProject_AsparagusCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* inputComponent) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class USkeletalMeshComponent* GetCharacterMesh() const { return CharacterMesh; }

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	void BeginSmack();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerBeginSmack();

	void StopSmack();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerStopSmack();

	void SmackCharacter();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSmackCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Smacking)
		float SmackStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SmackStrength_Min;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SmackStrength_Max;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SmackWindupTime;

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* CharacterMesh;

	void PlayCharacterAnimation();

	UAnimSequence *Anim;

	bool IsSmacking;
};

