// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSpawnManager.h"

#include "ZombieSpawnPoint.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AZombieSpawnManager::AZombieSpawnManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AZombieSpawnManager::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AZombieSpawnPoint::StaticClass(), Actors);
	for (auto* Actor : Actors)
	{
		SpawnPoints.Add(Cast<AZombieSpawnPoint>(Actor));
	}
}

// Called every frame
void AZombieSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

