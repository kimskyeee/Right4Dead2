// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ConsumableItem.h"

#include "Survivor.h"


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

void AConsumableItem::CompleteConsume()
{
	Char->SwitchCamera(false);

	ApplyItemEffect(); // 효과 적용
	
	OnHoldCompleted.Broadcast(this);
	OnConsumed.Broadcast(this);
	bHoldActive = false;
	bInUseGuard = false;
	
	Destroy();
}

void AConsumableItem::CancelConsume()
{
	// 시간 초기화 + 1인칭 복귀 + 아이템 다시 보이기
	Char->SwitchCamera(false);

	bCompleted  = false;
	bInUseGuard = false;
}

void AConsumableItem::OnTap(float Elapsed)
{
}

void AConsumableItem::OnHoldBegan()
{
	if (!Char) return;
	if (bInUseGuard) return;
	bInUseGuard  = true;

	const double Now = FPlatformTime::Seconds();
	OnHoldStarted.Broadcast(this, NeedHold, Now);
	
	bHoldActive = true;
	bCompleted = false;
	
	Char->SwitchCamera(true);
}

void AConsumableItem::OnHoldTick(float Elapsed)
{
	// 동작하지 말자
	if (!bHoldActive || bCompleted) return;

	if (Elapsed >= NeedHold)
	{
		bCompleted = true;
		CompleteConsume();
	}
}

void AConsumableItem::OnHoldReleased(float Elapsed)
{
	// 홀드를 끝냈을 때 분기
	if (!bHoldActive) return;

	if (!bCompleted)
	{
		CancelConsume(); // 5초 미만 취소 루트
		OnHoldCanceled.Broadcast(this);
	}
	// 5초 이상이면 CompleteConsume에서 이미 처리 완료
	bHoldActive = false;
	bInUseGuard = false;
}

