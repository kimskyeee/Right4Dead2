#include "ZombieFSM.h"

#include "CommonZombie.h"

UZombieFSM::UZombieFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UZombieFSM::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<ACommonZombie>(GetOwner());
}
void UZombieFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

#pragma region Idle
void UZombieFSM::StartIdle()
{
	Super::StartIdle();
}
void UZombieFSM::TickIdle(const float DeltaTime)
{
	Super::TickIdle(DeltaTime);
}
void UZombieFSM::EndIdle()
{
	Super::EndIdle();
}
#pragma endregion Idle

#pragma region Chase
void UZombieFSM::StartChase()
{
	Super::StartChase();
}
void UZombieFSM::TickChase(const float DeltaTime)
{
	Super::TickChase(DeltaTime);

	CurrentChaseTime += DeltaTime;
	if (CurrentChaseTime > StopChaseTime)
	{
		TriggerStopChase();
		SetState(EZombieState::EZS_Idle);
		return;
	}

	// 공격 범위 내에 있으면 공격한다.
	if (Distance < NormalAttackRange)
	{
		SetState(EZombieState::EZS_Attack);
		return;
	}
	else
	{
		TriggerStartChase(ChaseTarget);
	}
	
	// 타겟이 인지 거리 내에 있으면 추적 지속 시간을 초기화 한다.
	if (Distance < Awareness)
	{
		CurrentChaseTime = 0.0f;
	}
}
void UZombieFSM::EndChase()
{
	Super::EndChase();
}
#pragma endregion Chase

#pragma region Attack
void UZombieFSM::StartAttack()
{
	Super::StartAttack();
}
void UZombieFSM::TickAttack(const float DeltaTime)
{
	Super::TickAttack(DeltaTime);
}
void UZombieFSM::EndAttack()
{
	Super::EndAttack();
}
#pragma endregion Attack

#pragma region Dead
void UZombieFSM::StartDead()
{
	Super::StartDead();
}
void UZombieFSM::TickDead(const float DeltaTime)
{
	Super::TickDead(DeltaTime);
}
void UZombieFSM::EndDead()
{
	Super::EndDead();
}

#pragma endregion Dead

void UZombieFSM::HandleShove(const FVector& FromLocation)
{
	// 상태 관련 처리
}
void UZombieFSM::HandleDamage()
{
	// ..
}
void UZombieFSM::HandleDie()
{
	Super::HandleDie();
}

void UZombieFSM::HandlePipeBombBeep(AActor* PipeBombActor)
{
	if (ChaseTarget == PipeBombActor)
	{
		return;
	}
	if (EZombieState::EZS_Attack != State)
	{
		TriggerStopChase();
		ChaseTarget = PipeBombActor;
	}
}