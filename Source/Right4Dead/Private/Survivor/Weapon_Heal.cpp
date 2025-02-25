// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Heal.h"

#include "Survivor.h"


// Sets default values
AWeapon_Heal::AWeapon_Heal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWeapon_Heal::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeapon_Heal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float HoldTime = survivor->HoldTime;
	float MaxHoldTime = survivor->MaxHoldTime;
	float CurrentHP = survivor->CurrentHP;
	float MaxHP = survivor->MaxHP;
	
	// 좌클릭을 꾹 누르고 있으면 회복 (5초동안 꾹누르기)
	if (bIsHoldingLeft)
	{
		HoldTime += GetWorld()->GetDeltaSeconds();
		// HoldTime이 MaxHoldTime을 초과하면 회복 및 카메라 전환
		if (HoldTime >= MaxHoldTime)
		{
			// 잃은 체력의 80% 회복
			CurrentHP += 0.8f * (MaxHP - CurrentHP);
			HoldTime = 0.0f;
			bIsHoldingLeft = false;

			// 카메라 전환
			survivor->SwitchCamera();

			// 응급도구 삭제
			if (survivor->HandleObjectSlot.WeaponFactory) 
			{
				survivor->UnequipWeapon();
				survivor->HandleObjectSlot = FWeaponData();
			}
			survivor->CurrentWeaponSlot.Reset();
			survivor->CurrentWeapon=nullptr;
		}
	}
}

/*void AWeapon_Heal::LeftClickEvent()
{
	Super::LeftClickEvent();

	float HoldTime = survivor->HoldTime;
	float MaxHoldTime = survivor->MaxHoldTime;
	float CurrentHP = survivor->CurrentHP;
	float MaxHP = survivor->MaxHP;

	// 좌클릭을 꾹 누르고 있으면 시작
	if (bIsHoldingLeft == true)
	{
		// 카메라 전환
		survivor->SwitchCamera();
		// 프로그레스바 진행
	}
	
	// 5초 되기전 놓으면 취소됨
	if (bIsHoldingLeft && HoldTime<MaxHoldTime)
	{
		HoldTime=0.0f;
		// 카메라 전환
		survivor->SwitchCamera();
		//프로그레스바 초기화
	}
	bIsHoldingLeft = false;
}*/

