// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSpawnPoint.h"

#include "Survivor.h"
#include "ZombieAIController.h"
#include "ZombieAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Right4Dead/Right4Dead.h"


// Sets default values
AZombieSpawnPoint::AZombieSpawnPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<ACommonZombie> ZombieClass(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Zombies/BP_CommonZombie.BP_CommonZombie_C'"));
	if (ZombieClass.Succeeded())
	{
		ZombieFactory = ZombieClass.Class;
	}
}

// Called when the game starts or when spawned
void AZombieSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZombieSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AZombieSpawnPoint::SpawnCommonZombie() const
{
	PRINT_CALLINFO();
	if (UWorld* World = GetWorld())
	{
		// 시뮬레이트 또는 게임 실행 중에만
		if (World->WorldType == EWorldType::PIE || World->WorldType == EWorldType::Game)
		{
			FActorSpawnParameters SpawnParams;
			if (auto* SpawnedZombie = World->SpawnActor<ACommonZombie>(ZombieFactory, GetActorLocation(), GetActorRotation()))
			{
				SpawnedZombie->Target = SpawnedZombie;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Spawn Zombie Failed"));
		}
	}
}

