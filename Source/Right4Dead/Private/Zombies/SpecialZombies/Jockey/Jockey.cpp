// Fill out your copyright notice in the Description page of Project Settings.


#include "Jockey.h"

#include "Right4DeadGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AJockey::AJockey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// SpecialZombieFSM = CreateDefaultSubobject<UJockeyFSM>(TEXT("SpecialZombieFSM"));
	// ZombieFSM = SpecialZombieFSM;
}

// Called when the game starts or when spawned
void AJockey::BeginPlay()
{
	Super::BeginPlay();

	Hp = MaxHp = 325.0f;
	Speed = 250.0f;
}

void AJockey::InitData()
{
	// GameInstance 가져오기
	if (const URight4DeadGameInstance* GameInstance = Cast<URight4DeadGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		switch (GameInstance->GetDifficulty())
		{
		case EDifficulty::Easy:
			break;
		case EDifficulty::Normal:
			break;
		case EDifficulty::Advanced:
			break;
		case EDifficulty::Expert:
			break;
		}
	}
}

// Called every frame
void AJockey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}