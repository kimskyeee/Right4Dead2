// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/SlotComponent.h"

#include "Survivor.h"
#include "Item/ItemSpec.h"
#include "Item/WeaponItem.h"


// Sets default values for this component's properties
USlotComponent::USlotComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Slots.SetNum(SlotSize);
}


// Called when the game starts
void USlotComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USlotComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool USlotComponent::TryPickup(AItemBase* Item)
{
	if (!Item || !Item->Spec) return false;

	const bool bHasSlot = (Item->Spec->bOccupiesSlot) && (Item->Spec->PreferredSlot != ESlotType::None);
	if (!bHasSlot)
	{
		// 슬롯이 없는 퀘스트(콜라)아이템이면, 현재 무기는 잠깐 UnEquip
		if (CurrentInHands.IsValid())
		{
			// 이전 무기를 잠시 보관
			PreviousItem.Push(CurrentInHands);
			CurrentInHands->OnUnequipped();
		}
		EquipItemInHands(Item); // 손에 들자
		Item->OnPickedUp();
		return true;
	}

	const int32 idx = Index(Item->Spec->PreferredSlot);
	if (idx <= 0 || idx >= Slots.Num()) return false;

	if (Slots[idx].Item.IsValid())
	{
		// 같은 슬롯에 이미 아이템 있다면 드롭
		Drop(Slots[idx].Item.Get());
		Slots[idx].Item = nullptr;
	}

	Slots[idx].Item = Item;
	Item->OnPickedUp();
	EquipItemInHands(Item);
	return true;
}

bool USlotComponent::EquipSlot(ESlotType Slot)
{
	// 퀘스트(콜라)에서 무기로 변경시 콜라는 월드에 Drop
	if (CurrentInHands.IsValid() && CurrentInHands->Spec && CurrentInHands->Spec->PreferredSlot == ESlotType::None)
	{
		Drop(CurrentInHands.Get());
		CurrentInHands = nullptr;
	}

	const int32 idx = Index(Slot);
	if (idx <= 0 || idx >= Slots.Num()) return false;
	if (!Slots[idx].Item.IsValid()) return false;

	// 들고 있던 무기 되돌리기
	if (CurrentInHands.IsValid() && CurrentInHands != Slots[idx].Item)
	{
		ReturnCurrentToItsPlaceOrDrop();
		CurrentInHands->OnUnequipped();
	}

	EquipItemInHands(Slots[idx].Item.Get());
	return true;
}

void USlotComponent::HandleUse(EUsingType Phase, float Elapsed)
{
	if (CurrentInHands.IsValid())
	{
		CurrentInHands->HandleUse(Phase, Elapsed);
	}
}

void USlotComponent::RightClickUse()
{
	// 들고 있는 무기의 우클릭 함수 실행
	if (CurrentInHands.IsValid())
	{
		ReturnCurrentToItsPlaceOrDrop();
		CurrentInHands->RightAttack();
	}
}

void USlotComponent::EquipItemInHands(AItemBase* NewItem)
{
	if (!NewItem) return;

	TWeakObjectPtr<AItemBase> Prev = CurrentInHands;
	const bool bNewConsumable = (NewItem->Spec && NewItem->Spec->bConsumesOnUse);

	// 소모형을 들면, 복귀 대상(Prev가 비소모형일 때만) 스택에 기록
	if (bNewConsumable && Prev.IsValid() && Prev->Spec && !Prev->Spec->bConsumesOnUse)
	{
		if (PreviousItem.Num() == 0 || PreviousItem.Last() != Prev)
		{
			PreviousItem.Add(Prev);
		}
	}
	// 이전 아이템 정리
	if (Prev.IsValid())
	{
		BindConsumption(Prev.Get(), /*bBind*/false);
		Prev->OnUnequipped();
	}

	if (ASurvivor* OwnerChar = Cast<ASurvivor>(GetOwner()))
	{
		NewItem->OnEquipped(OwnerChar);
	}
	BindConsumption(NewItem, true);
}

void USlotComponent::BindConsumption(AItemBase* Item, bool bBind)
{
	if (!IsValid(Item)) return;

	// 중복 방지, 먼저 제거 후 필요 시 다시 바인드
	Item->OnConsumed.RemoveDynamic(this, &USlotComponent::OnItemConsumed);
	if (bBind)
	{
		Item->OnConsumed.AddDynamic(this, &USlotComponent::OnItemConsumed);
	}
}

void USlotComponent::OnItemConsumed(AItemBase* Used)
{
	// 슬롯에 들어있던 소비 아이템이면 슬롯에서 제거
	if (Used && Used->Spec && Used->Spec->bOccupiesSlot)
	{
		const int32 idx = Index(Used->Spec->PreferredSlot);
		if (idx > 0 && idx < Slots.Num() && Slots[idx].Item.Get() == Used)
		{
			Slots[idx].Item = nullptr;
		}
	}

	// 현재 손에 들고 있었다면 비우기
	if (CurrentInHands.Get() == Used)
	{
		BindConsumption(Used, /*bBind*/false);
		CurrentInHands = nullptr;
	}

	// 복귀
	while (PreviousItem.Num() > 0)
	{
		TWeakObjectPtr<AItemBase> Prev = PreviousItem.Pop();
		if (Prev.IsValid())
		{
			EquipItemInHands(Prev.Get());
			return;
		}
		// 무효면 계속 Pop?
	}

	// 대체 무기가 없으면 1→4 순회
	for (int32 i = 1; i <= 4; ++i)
	{
		if (Slots[i].Item.IsValid())
		{
			EquipItemInHands(Slots[i].Item.Get());
			return;
		}
	}
}

bool USlotComponent::CycleFilled(int32 Dir)
{
	if (Dir == 0) return false;
	const int32 Last = FMath::Min(4, Slots.Num()-1);

	// 시작점: 현재 손에 든 슬롯(없으면 1)
	int32 start = 1;
	if (CurrentInHands.IsValid() && CurrentInHands->Spec && CurrentInHands->Spec->bOccupiesSlot)
	{
		start = FMath::Clamp(Index(CurrentInHands->Spec->PreferredSlot), 1, Last);
	}

	int32 idx = start;
	for (int step = 0; step < Last; ++step)
	{
		// 다음 인덱스로 이동(래핑 수동 처리)
		idx += (Dir > 0 ? +1 : -1);
		if (idx < 1) idx = Last;
		if (idx > Last) idx = 1;

		if (Slots[idx].Item.IsValid())
		{
			EquipItemInHands(Slots[idx].Item.Get());
			return true;
		}
	}
	return false;
}

void USlotComponent::ReturnCurrentToItsPlaceOrDrop()
{
	if (!CurrentInHands.IsValid() || !CurrentInHands->Spec)
	{
		return;
	}

	if (CurrentInHands->Spec->bOccupiesSlot)
	{
		const int32 PrevIdx = Index(CurrentInHands->Spec->PreferredSlot);
		if (PrevIdx > 0 && PrevIdx < Slots.Num() && !Slots[PrevIdx].Item.IsValid())
		{
			Slots[PrevIdx].Item = CurrentInHands; // 제자리에 되돌리기
			return;
		}
	}
	
	// 슬롯이 없거나, 제자리가 이미 차있으면 Drop
	Drop(CurrentInHands.Get());
}

void USlotComponent::Drop(AItemBase* Item)
{
	if (!Item) return;
	
	// 손에서 분리 및 물리 활성
	Item->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Item->OnDropped();
}
