// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/Tank.h"

#include "Right4DeadGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATank::ATank()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Hp = 3000.0f;
	Speed = 210.0f;
	PartDamageMultipliers.Legs = 1;
	PartDamageMultipliers.Stomach = 1;
	PartDamageMultipliers.Thorax = 1;
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
}

void ATank::InitDifficulty()
{
	// GameInstance 가져오기
	if (const URight4DeadGameInstance* GameInstance = Cast<URight4DeadGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		switch (GameInstance->GetDifficulty())
		{
		case EDifficulty::Easy:
			Hp = 3000.0f;
			break;
		case EDifficulty::Normal:
			Hp = 4000.0f;
			break;
		case EDifficulty::Advanced:
			Hp = 8000.0f;
			break;
		case EDifficulty::Expert:
			Hp = 8000.0f;
			break;
		}
	}
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}