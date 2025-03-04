// Fill out your copyright notice in the Description page of Project Settings.


#include "Boomer.h"

#include "Right4DeadGameInstance.h"
#include "ZombieAudioComponent.h"
#include "BoomerFSM.h"
#include "Kismet/GameplayStatics.h"

ABoomer::ABoomer()
{
	PrimaryActorTick.bCanEverTick = true;

	// TODO: 모델 및 애님블루프린트 변경
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshObj(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/ThirdPerson/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));
	if (SkeletalMeshObj.Succeeded())
	{
		GetMesh()->SetSkeletalMeshAsset(SkeletalMeshObj.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -89));
		GetMesh()->SetRelativeRotation(FRotator(0, 270, 0));
		ConstructorHelpers::FClassFinder<UAnimInstance> AnimBlueprintClass(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/Zombies/CommonZombie/ABP_CommonZombie.ABP_CommonZombie_C'"));
		if (AnimBlueprintClass.Succeeded())
		{
			GetMesh()->SetAnimInstanceClass(AnimBlueprintClass.Class);
		}
	}

	SpecialZombieFSM = CreateDefaultSubobject<UBoomerFSM>(TEXT("SpecialZombieFSM"));
	ZombieFSM = SpecialZombieFSM;
	
	static ConstructorHelpers::FClassFinder<UZombieAudioComponent> ZombieAudioComponentClass(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Zombies/Boomer/BP_BoomerAudioComponent.BP_BoomerAudioComponent_C'"));
	if (ZombieAudioComponentClass.Succeeded())
	{
		ZombieAudioComponentFactory = ZombieAudioComponentClass.Class;
	}
}

void ABoomer::BeginPlay()
{
	Super::BeginPlay();
}

void ABoomer::InitData()
{
	Hp = MaxHp = 50.0f;
	Speed = 175.0f;
	ZombieFSM->NormalAttackInterval = 1.0f;
	SpecialZombieFSM->SpecialAttackRange = 350.0f;
	SpecialZombieFSM->SpecialAttackInterval = 30.0f;
	// TODO: 담즙 지속 시간 (5초, 지속 중에 또 맞으면 1.5초 추가)
	// TODO: 죽을 때 담즙 폭파 반경 (2.5 ~ 4m)
	
	if (const URight4DeadGameInstance* GameInstance = Cast<URight4DeadGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		switch (GameInstance->GetDifficulty())
		{
		case EDifficulty::Easy:
			NormalAttackDamage = 1;
			break;
		case EDifficulty::Normal:
			NormalAttackDamage = 2;
			break;
		case EDifficulty::Advanced:
			NormalAttackDamage = 5;
			break;
		case EDifficulty::Expert:
			NormalAttackDamage = 20;
			break;
		}
	}
}

void ABoomer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}