#include "ZombieBase.h"

AZombieBase::AZombieBase()
{
	PrimaryActorTick.bCanEverTick = true;
	Hp = 0;
	Speed = 0;
}

void AZombieBase::BeginPlay()
{
	Super::BeginPlay();
	InitDifficulty();
}

void AZombieBase::OnDamaged(float Damage)
{
	
}

void AZombieBase::OnDie()
{
	
}