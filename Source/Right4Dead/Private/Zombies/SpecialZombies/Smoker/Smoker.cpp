// Fill out your copyright notice in the Description page of Project Settings.


#include "Smoker.h"

#include "Right4DeadGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASmoker::ASmoker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Hp = 250.0f;
	Speed = 210.0f;
}

// Called when the game starts or when spawned
void ASmoker::BeginPlay()
{
	Super::BeginPlay();
}

void ASmoker::InitDifficulty()
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
void ASmoker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}