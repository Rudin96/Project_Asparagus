// Fill out your copyright notice in the Description page of Project Settings.


#include "LightSwitcher_Base.h"
#include "Engine/EngineTypes.h"
#include "Components/BoxComponent.h"
#include "Components/SpotLightComponent.h"
#include "FallingActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Math/Vector.h"
#include "Project_Asparagus/Project_AsparagusCharacter.h"

// Sets default values
ALightSwitcher_Base::ALightSwitcher_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComponent"));

	Rotating_Light = CreateDefaultSubobject<USpotLightComponent>(TEXT("Rotating Light"));

	LightSpawn_Area = CreateDefaultSubobject<UBoxComponent>(TEXT("RockSpawner"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> VisualMesh(TEXT("StaticMesh'/Game/Geometry/Meshes/unreal_Lantern.unreal_Lantern'"));

	if (VisualMesh.Succeeded())
	{
		BaseMesh->SetStaticMesh(VisualMesh.Object);
	}

	RotationTime = 2.0f;

	SpawnTimer = .5f;

	Rotating_Light->SetupAttachment(BaseMesh);

	Rotating_Light->SetRelativeLocation(FVector(0.0f, 0.0f, 570.f));

	Rotating_Light->SetRelativeRotation(FRotator(-30.0f, 0.0f, 0.0f));

	Rotating_Light->SetAttenuationRadius(7000.f);

	Rotating_Light->SetIntensity(3000000.f);

	LightSpawn_Area->SetBoxExtent(FVector(1000.0f, 1000.0f, 10.0f));

	LightSpawn_Area->SetupAttachment(Rotating_Light);

	LightSpawn_Area->SetWorldLocation(FVector(860.0f, 0.0f, 530.0f));

	LightSpawn_Area->SetRelativeRotation(FRotator(30.0f, 0.0f, 0.0f));

	RootComponent = BaseMesh;

}

// Called when the game starts or when spawned
void ALightSwitcher_Base::BeginPlay()
{
	Super::BeginPlay();

	//Timer setup for rockspawn and light rotation.
	GetWorldTimerManager().SetTimer(LightTimerHandle, this, &ALightSwitcher_Base::RotateLight, RotationTime, true, 0.0f);

	GetWorldTimerManager().SetTimer(RockSpawnerTimerHandle, this, &ALightSwitcher_Base::SpawnRocks, SpawnTimer, true, 0.0f);
}

void ALightSwitcher_Base::SpawnRocks()
{
	//Set up spawn parameters here
	FActorSpawnParameters spawnParams;
	UWorld* world = GetWorld();
	spawnParams.Owner = this;
	spawnParams.bNoFail = true;
	FVector start;
	
	//Get random location in spawning box
	start = UKismetMathLibrary::RandomPointInBoundingBox(LightSpawn_Area->GetComponentLocation(), LightSpawn_Area->GetScaledBoxExtent());
	FRotator rot(0.0f, 0.0f, 0.0f);

	//Spawn the actor
	AFallingActor* spawnedActor = world->SpawnActor<AFallingActor>(fallingActor, start, rot, spawnParams);
}

void ALightSwitcher_Base::RotateLight()
{
	Rotating_Light->AddWorldRotation(FRotator(0.0f, 90.0f, 0.0f));
}

// Called every frame
void ALightSwitcher_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

