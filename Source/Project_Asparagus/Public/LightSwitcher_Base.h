// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightSwitcher_Base.generated.h"

UCLASS()
class PROJECT_ASPARAGUS_API ALightSwitcher_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightSwitcher_Base();

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		class UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		class USpotLightComponent* Rotating_Light;
	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		class UBoxComponent* LightSpawn_Area;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RotationTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SpawnTimer;

	UPROPERTY(EditDefaultsOnly, Category = Falling)
		TSubclassOf<class AFallingActor> fallingActor;

	void RotateLight();

	void SpawnRocks();

	FTimerHandle LightTimerHandle;
	FTimerHandle RockSpawnerTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
