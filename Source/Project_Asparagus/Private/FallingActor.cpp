// Fill out your copyright notice in the Description page of Project Settings.


#include "FallingActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "LightSwitcher_Base.h"
#include "Math/Vector.h"
#include "Components/PrimitiveComponent.h"
#include "DestructibleActor.h"
#include "DestructibleComponent.h"

// Sets default values
AFallingActor::AFallingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> RockVisualAsset(TEXT("StaticMesh'/Game/StarterContent/Props/SM_Rock.SM_Rock'"));

	RootComponent = BaseMesh;

	if (RockVisualAsset.Succeeded())
	{
		BaseMesh->SetStaticMesh(RockVisualAsset.Object);
		BaseMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		BaseMesh->SetCollisionProfileName(TEXT("BlockAll"));

		BaseMesh->SetNotifyRigidBodyCollision(true);

		BaseMesh->SetSimulatePhysics(true);
	}
}

// Called when the game starts or when spawned
void AFallingActor::BeginPlay()
{
	Super::BeginPlay();
}

void AFallingActor::NotifyHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(HitComponent, OtherActor, OtherComponent, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	//Check if it hit a destructible actor
	if (Cast<UDestructibleComponent>(OtherComponent))
	{
		//If so, apply some damage and break it
		Cast<UDestructibleComponent>(OtherComponent)->ApplyDamage(300.0f, HitLocation, HitLocation * 500.0f, 300.0f);
	}
}

// Called every frame
void AFallingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

