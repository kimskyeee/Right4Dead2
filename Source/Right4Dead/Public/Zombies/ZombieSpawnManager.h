// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZombieSpawnManager.generated.h"

class ACommonZombieAIController;
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
	int MaxZombieCount = 120;
	UPROPERTY(EditAnywhere, Category="Debugging")
	int NumOfHorde = 40;
	UPROPERTY(VisibleInstanceOnly, Category="Debugging")
	TArray<AZombieSpawnPoint*> SpawnPoints;

	TSubclassOf<ACommonZombie> ZombieFactory;

	UPROPERTY(VisibleInstanceOnly, Category="Debugging")
	TObjectPtr<AActor> InitTarget = nullptr;
	
	UPROPERTY(VisibleInstanceOnly, Category="Debugging")
	TSet<ACommonZombie*> ActiveZombies;
	
	UPROPERTY()
	TObjectPtr<USoundWave> HordeComingSound = nullptr;

	void EnqueueZombie(ACommonZombie* Zombie);
	ACommonZombie* DequeueZombie();
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(CallInEditor, BlueprintCallable, Category="Debugging")
	void CallHorde();

private:
	UPROPERTY(VisibleInstanceOnly, Category="Debugging")
	int PoolCount = 0;
	TQueue<ACommonZombie*, EQueueMode::SingleThreaded> ZombiePool;
};
