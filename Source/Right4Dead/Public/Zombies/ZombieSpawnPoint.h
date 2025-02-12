// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonZombie.h"
#include "GameFramework/Actor.h"
#include "ZombieSpawnPoint.generated.h"

UCLASS()
class RIGHT4DEAD_API AZombieSpawnPoint : public AActor
{
	GENERATED_BODY()

public:
	AZombieSpawnPoint();

protected:
	virtual void BeginPlay() override;

public:
	TSubclassOf<ACommonZombie> ZombieFactory;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(CallInEditor, Category = "Debugging")
	void SpawnCommonZombie() const;
};