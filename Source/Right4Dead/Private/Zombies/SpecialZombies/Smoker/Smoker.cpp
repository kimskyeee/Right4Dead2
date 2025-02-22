// Fill out your copyright notice in the Description page of Project Settings.


#include "Smoker.h"

#include "Right4DeadGameInstance.h"
#include "SmokerFSM.h"
#include "Kismet/GameplayStatics.h"

ASmoker::ASmoker()
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
	
	SpecialZombieFSM = CreateDefaultSubobject<USmokerFSM>(TEXT("SpecialZombieFSM"));
	ZombieFSM = SpecialZombieFSM;
}

void ASmoker::BeginPlay()
{
	Super::BeginPlay();
}

void ASmoker::InitData()
{
	// TODO: Smoker 데이터
	Hp = 250.0f;
	Speed = 210.0f;
	SpecialZombieFSM->SpecialAttackRange = 3000.0f;
	
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

void ASmoker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}