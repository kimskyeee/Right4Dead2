// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZombieSpawnManager.generated.h"

class ACommonZombie;
class AZombieSpawnPoint;
class ASurvivor;

UCLASS()
class RIGHT4DEAD_API AZombieSpawnManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AZombieSpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Category="Debugging")
	int NumOfHorde = 30;
	UPROPERTY(VisibleInstanceOnly, Category="Debugging")
	TArray<AZombieSpawnPoint*> SpawnPoints;
	UPROPERTY(VisibleInstanceOnly, Category="Debugging")
    TArray<ACommonZombie*> CommonZombies;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(CallInEditor, Category="Debugging")
	void CallHorde();
	
	UFUNCTION(CallInEditor, Category="Debugging")
	void DisableTick();
};
