// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/Spitter.h"

#include "Right4DeadGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpitter::ASpitter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Hp = 100.0f;
	Speed = 210.0f;
}

// Called when the game starts or when spawned
void ASpitter::BeginPlay()
{
	Super::BeginPlay();
}

void ASpitter::InitDifficulty()
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
void ASpitter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}