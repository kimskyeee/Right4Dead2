// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieEffectComponent.h"

#include "ZombieBase.h"


// Sets default values for this component's properties
UZombieEffectComponent::UZombieEffectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UZombieEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AZombieBase>(GetOwner());
	if (Owner)
	{
		Owner->OnBulletHit.AddDynamic(this, &UZombieEffectComponent::OnBulletHit);
	}
}

// Called every frame
void UZombieEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

