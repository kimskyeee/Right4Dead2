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
void UZombieFSM::TickIdle()
{
	Super::TickIdle();
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
void UZombieFSM::TickChase()
{
	Super::TickChase();
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
void UZombieFSM::TickAttack()
{
	Super::TickAttack();
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
void UZombieFSM::TickDead()
{
	Super::TickDead();
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
	SetState(EZombieState::EZS_Dead);
}