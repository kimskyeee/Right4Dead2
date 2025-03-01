// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAudioComponent.h"

#include "ZombieBase.h"
#include "ZombieFSM.h"


// Sets default values for this component's properties
UZombieAudioComponent::UZombieAudioComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UZombieAudioComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<AZombieBase>(GetOwner());
	if (Owner)
	{
		ZombieFsm = Cast<UZombieFSM>(Owner->ZombieFSM);
		if (ZombieFsm)
		{
			ZombieFsm->OnChangedState.AddDynamic(this, &UZombieAudioComponent::OnChangedState);
			ZombieFsm->OnNormalAttacked.AddDynamic(this, &UZombieAudioComponent::OnNormalAttacked);
		}
	}
}
