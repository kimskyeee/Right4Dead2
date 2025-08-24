// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivorPlayerController.h"

#include "UISurvivorCokeDelivery.h"
#include "Blueprint/UserWidget.h"
#include "Item/ConsumableItem.h"
#include "Item/SlotComponent.h"

void ASurvivorPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (CokeDeliveryUIClass)
	{
		CokeDeliveryUI = CreateWidget<UUISurvivorCokeDelivery>(GetWorld(), CokeDeliveryUIClass);
		if (CokeDeliveryUI)
		{
			CokeDeliveryUI->SetVisibility(ESlateVisibility::Hidden);
			CokeDeliveryUI->AddToViewport(10);
		}
	}
}

void ASurvivorPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 캐릭터에서 슬롯 컴포넌트 찾기
	SlotComp = InPawn ? InPawn->FindComponentByClass<USlotComponent>() : nullptr;
	if (SlotComp)
	{
		SlotComp->OnInHandsItemChanged.AddDynamic(this, &ASurvivorPlayerController::HandleInHandsChanged);
	}
}

void ASurvivorPlayerController::HandleInHandsChanged(class AItemBase* NewItem)
{
	UnbindConsumable();
	if (CokeDeliveryUI)
	{
		CokeDeliveryUI->StopProgress();
		CokeDeliveryUI->SetVisibility(ESlateVisibility::Hidden);
	}
	
	if (AConsumableItem* ConsumableItem = Cast<AConsumableItem>(NewItem))
	{
		BindConsumable(ConsumableItem);
		UTexture2D* Icon=nullptr;
		FText Label;
		ConsumableItem->GetSimpleUI(Icon, Label);
		CokeDeliveryUI->SetSimpleVisual(Icon, Label);
	}
}

void ASurvivorPlayerController::BindConsumable(class AConsumableItem* Item)
{
	if (!Item) return;
	
	Item->OnHoldStarted.AddUObject(this, &ASurvivorPlayerController::OnHoldStarted);
	Item->OnHoldCanceled.AddUObject(this, &ASurvivorPlayerController::OnHoldCanceled);
	Item->OnHoldCompleted.AddUObject(this, &ASurvivorPlayerController::OnHoldCompleted);

	BoundConsumable = Item;
}

void ASurvivorPlayerController::UnbindConsumable()
{
	if (AConsumableItem* Item = BoundConsumable.Get())
	{
		Item->OnHoldStarted.RemoveAll(this);
		Item->OnHoldCanceled.RemoveAll(this);
		Item->OnHoldCompleted.RemoveAll(this);
	}
	BoundConsumable = nullptr;
}

void ASurvivorPlayerController::OnHoldStarted(AConsumableItem* Item, float Duration, double StartTime)
{
	if (CokeDeliveryUI)
	{
		CokeDeliveryUI->StartProgress(Duration, StartTime);
	}
}

void ASurvivorPlayerController::OnHoldCanceled(AConsumableItem* Item)
{
	if (CokeDeliveryUI)
	{
		CokeDeliveryUI->StopProgress();
	}
}

void ASurvivorPlayerController::OnHoldCompleted(AConsumableItem* Item)
{
	if (CokeDeliveryUI)
	{
		// 완료하면 되긴하지만 일단
		CokeDeliveryUI->StopProgress();
	}
}
