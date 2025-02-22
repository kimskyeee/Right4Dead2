#include "SpecialZombieBaseFSM.h"

USpecialZombieBaseFSM::USpecialZombieBaseFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
	bEnableSearch = false;
}

void USpecialZombieBaseFSM::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	if (CurrentSpecialAttackTime < SpecialAttackInterval)
	{
		CurrentSpecialAttackTime += DeltaTime;
	}
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USpecialZombieBaseFSM::TickChase()
{
	Super::TickChase();
}
