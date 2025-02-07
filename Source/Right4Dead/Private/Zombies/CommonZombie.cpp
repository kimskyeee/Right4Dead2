// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/CommonZombie.h"

#include "Right4DeadGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACommonZombie::ACommonZombie()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Hp = 50.0f;
	Speed = 250.0f;
}

// Called when the game starts or when spawned
void ACommonZombie::BeginPlay()
{
	Super::BeginPlay();
}

void ACommonZombie::InitDifficulty()
{
	// GameInstance 가져오기
	if (const URight4DeadGameInstance* GameInstance = Cast<URight4DeadGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		switch (GameInstance->GetDifficulty())
		{
		case EDifficulty::Easy:
			HitDamageRatio = 2; // 200%
			AttackDamage = 1;
			break;
		case EDifficulty::Normal:
			HitDamageRatio = 1; // 100%
			AttackDamage = 2;
			break;
		case EDifficulty::Advanced:
			HitDamageRatio = 0.75f; // 75%
			AttackDamage = 5;
			break;
		case EDifficulty::Expert:
			HitDamageRatio = 0.5f; // 50%
			AttackDamage = 20;
			break;
		}
	}
}

// Called every frame
void ACommonZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACommonZombie::OnDamaged(float Damage)
{
	Damage *= HitDamageRatio;
}

void ACommonZombie::OnDie()
{
}