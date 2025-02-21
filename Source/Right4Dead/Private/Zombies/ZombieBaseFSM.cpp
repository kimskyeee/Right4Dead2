// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieBaseFSM.h"


// Sets default values for this component's properties
UZombieBaseFSM::UZombieBaseFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UZombieBaseFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UZombieBaseFSM::SetState(const EZombieState NewState)
{
	// 기존 상태에 따라 정리 작업
	switch (State)
	{
	case EZombieState::EZS_Idle:
		EndIdle();
		break;
	case EZombieState::EZS_Chase:
		EndChase();
		break;
	case EZombieState::EZS_Attack:
		EndAttack();
		break;
	case EZombieState::EZS_Dead:
		EndDead();
		break;
	}

	// 새로운 상태에 따른 초기화 작업
	switch (NewState)
	{
	case EZombieState::EZS_Idle:
		StartIdle();
		break;
	case EZombieState::EZS_Chase:
		StartChase();
		break;
	case EZombieState::EZS_Attack:
		StartAttack();
		break;
	case EZombieState::EZS_Dead:
		StartDead();
		break;
	}
	State = NewState;
}


// Called every frame
void UZombieBaseFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

