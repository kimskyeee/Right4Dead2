// Fill out your copyright notice in the Description page of Project Settings.


#include "Spitter.h"

#include "Right4DeadGameInstance.h"
#include "SpitterFSM.h"
#include "Kismet/GameplayStatics.h"

ASpitter::ASpitter()
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
	
	SpecialZombieFSM = CreateDefaultSubobject<USpitterFSM>(TEXT("SpecialZombieFSM"));
	ZombieFSM = SpecialZombieFSM;
}

void ASpitter::BeginPlay()
{
	Super::BeginPlay();
}

void ASpitter::InitData()
{
	// TODO: Spitter 데이터
	Hp = 100.0f;
    Speed = 210.0f;
	
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

void ASpitter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}