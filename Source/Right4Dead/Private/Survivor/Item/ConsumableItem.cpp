// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ConsumableItem.h"

#include "Survivor.h"
#include "Item/ItemInfo.h"


// Sets default values
AConsumableItem::AConsumableItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AConsumableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AConsumableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AConsumableItem::HandleUse(EUsingType Phase, float ElapsedHold)
{
	if (!Char) return;

	if (Phase == EUsingType::Ongoing)
	{
		Char->SwitchCamera(true);
	}
	if (Phase == EUsingType::Completed && ElapsedHold >= NeedHold)
	{
		Char->SwitchCamera(false);
		
		ApplyItemEffect();
		OnConsumed.Broadcast(this);
		Destroy();
	}
}

void AConsumableItem::ApplyItemEffect()
{
}
