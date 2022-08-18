// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project_AsparagusCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

AProject_AsparagusCharacter::AProject_AsparagusCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm



	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	static ConstructorHelpers::FObjectFinder<UAnimSequence> anim(TEXT("AnimSequence'/Game/TopDownCPP/Meshes/walking_animation.walking_animation'"));
	Anim = anim.Object;

}

void AProject_AsparagusCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AProject_AsparagusCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//Check if we want to smack
	if (IsSmacking)
	{
		//If so, perform smack here and smoothly "wind up" the smack here
		SmackStrength = FMath::FInterpTo(SmackStrength, SmackStrength_Max, DeltaSeconds, SmackWindupTime);
	}
}

void AProject_AsparagusCharacter::SetupPlayerInputComponent(class UInputComponent* inputComponent)
{
	Super::SetupPlayerInputComponent(inputComponent);

	this->InputComponent->BindAxis("MoveRight", this, &AProject_AsparagusCharacter::MoveRight);
	this->InputComponent->BindAxis("MoveForward", this, &AProject_AsparagusCharacter::MoveForward);

	this->InputComponent->BindAction("SmackPlayer", IE_Pressed, this, &AProject_AsparagusCharacter::BeginSmack);
	this->InputComponent->BindAction("SmackPlayer", IE_Released, this, &AProject_AsparagusCharacter::StopSmack);

	this->InputComponent->BindAction("Jump", IE_Pressed, this, &AProject_AsparagusCharacter::Jump);
}

void AProject_AsparagusCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(FVector(500, 0, 0), GetWorld()->GetDeltaSeconds() * AxisValue, false);
}

void AProject_AsparagusCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(FVector(0, 500, 0), GetWorld()->GetDeltaSeconds() * AxisValue, false);
}

void AProject_AsparagusCharacter::BeginSmack()
{
	IsSmacking = true;

	//Check if we are a local client and not a server
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerBeginSmack();
	}
}

void AProject_AsparagusCharacter::PlayCharacterAnimation()
{
	CharacterMesh->PlayAnimation(Anim, true);
}

bool AProject_AsparagusCharacter::ServerBeginSmack_Validate()
{
	return true;
}

void AProject_AsparagusCharacter::ServerBeginSmack_Implementation()
{
	//Perform smack on server
	BeginSmack();
}

void AProject_AsparagusCharacter::StopSmack()
{
	
	SmackCharacter();
	IsSmacking = false;
	SmackStrength = 0.f;
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerStopSmack();
	}
}

bool AProject_AsparagusCharacter::ServerStopSmack_Validate()
{
	return true;
}

void AProject_AsparagusCharacter::ServerStopSmack_Implementation()
{
	//Stop smack on server
	StopSmack();
}


//Smacking logic
void AProject_AsparagusCharacter::SmackCharacter()
{
	FHitResult hResult;
	FVector Start = GetActorLocation();
	FVector End = Start + (GetActorForwardVector() * 100.f);

	FCollisionQueryParams queryParams(FName(), false, this);
	FCollisionResponseParams responseParams;
	GetWorld()->LineTraceSingleByChannel(hResult, Start, End, ECC_Camera, queryParams, responseParams);
	if (hResult.IsValidBlockingHit() && hResult.GetActor() == Cast<AProject_AsparagusCharacter>(hResult.GetActor()))
	{
		Cast<AProject_AsparagusCharacter>(hResult.GetActor())->GetCharacterMovement()->AddImpulse(FVector(((hResult.ImpactPoint - GetActorLocation()) * (SmackStrength * 10.f)) + FVector(0.f, 0.f, 800.f)));
		if (GetLocalRole() < ROLE_Authority)
		{
			ServerSmackCharacter();
		}
	}
}

bool AProject_AsparagusCharacter::ServerSmackCharacter_Validate()
{
	return true;
}

void AProject_AsparagusCharacter::ServerSmackCharacter_Implementation()
{
	SmackCharacter();
}

void AProject_AsparagusCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicate SmackStrength
	DOREPLIFETIME(AProject_AsparagusCharacter, SmackStrength);
}