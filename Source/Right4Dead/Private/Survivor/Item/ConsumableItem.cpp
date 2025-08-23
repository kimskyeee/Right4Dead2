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

void AConsumableItem::ApplyItemEffect()
{
}

void AConsumableItem::OnTap(float Elapsed)
{
	// 동작하지 말자
}

void AConsumableItem::OnHoldBegan()
{
	Super::OnHoldBegan();

	// 진행도 UI 시작
	
	if (!Char) return;
	Char->SwitchCamera(true);
}

void AConsumableItem::OnHoldReleased(float Elapsed)
{
	Super::OnHoldReleased(Elapsed);
	
	if (!Char) return;

	if (Elapsed >= NeedHold)
	{
		Char->SwitchCamera(false);
		
		ApplyItemEffect();
		OnConsumed.Broadcast(this);
		Destroy();
	}
}

