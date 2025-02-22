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

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACommonZombie::StaticClass(), Actors);
	for (auto* Actor : Actors)
	{
		CommonZombies.Add(Cast<ACommonZombie>(Actor));
	}
}

// Called every frame
void AZombieSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AZombieSpawnManager::CallHorde()
{
	int Rem = NumOfHorde;
	while (Rem >= 0)
	{
		for (const auto* SpawnPoint : SpawnPoints)
		{
			if (--Rem < 0) break;
			SpawnPoint->SpawnCommonZombie();
		}
	}

	// UGameplayStatics::PlaySound2D();
}