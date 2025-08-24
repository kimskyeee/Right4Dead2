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
	UE_LOG(LogTemp, Warning, TEXT("[TryPickup] Item=%s OwnerIsMe=%d InHands?=%d"),
	   *GetNameSafe(Item),
	   Item && Item->GetOwner()==GetOwner(),
	   CurrentInHands.Get()==Item);
	
	// 무기 줍기 함수
	if (!Item || !Item->Spec) return false;
	ASurvivor* OwnerChar = Cast<ASurvivor>(GetOwner());

	if (Item->GetOwner() == OwnerChar) return false;
	if (CurrentInHands.IsValid() && CurrentInHands.Get() == Item) return false;
	
	const bool bHasSlot = (Item->Spec->bOccupiesSlot) && (Item->Spec->PreferredSlot != ESlotType::None);
	if (!bHasSlot)
	{
		// 비슬롯 무기
		Item->OnPickedUp(OwnerChar); 
		EquipItemInHands(Item);
		return true;
	}

	// 슬롯 있는 무기면 슬롯 검사
	const int32 idx = Index(Item->Spec->PreferredSlot);
	if (idx <= 0 || idx >= Slots.Num()) return false;

	if (Slots[idx].Item.IsValid())
	{
		// 같은 슬롯에 이미 아이템 있다면 기존에 있던 아이템은 드롭
		Drop(Slots[idx].Item.Get(), true);
	}

	// 다시 슬롯 채우기
	Slots[idx].Item = Item;
	Item->OnPickedUp(OwnerChar);
	
	EquipItemInHands(Item);
	return true;
}

bool USlotComponent::EquipSlot(ESlotType Slot)
{
	// 슬롯 선택
	const int32 idx = Index(Slot);
	if (idx < 0 || idx >= Slots.Num()) return false;
	if (!Slots[idx].Item.IsValid()) return false;
	
	// 비슬롯 퀘스트 -> 무기로 변경시 콜라는 월드에 Drop
	if (CurrentInHands.IsValid() && CurrentInHands->Spec && !CurrentInHands->Spec->bOccupiesSlot)
	{
		Drop(CurrentInHands.Get(), false);
	}
	
	// 현재 슬롯과 다르면, 현재 아이템 보관 또는 드롭
	if (CurrentInHands.IsValid() && CurrentInHands != Slots[idx].Item)
	{
		ReturnCurrentToItsPlaceOrDrop();
	}
	else if (CurrentInHands == Slots[idx].Item) return false;

	EquipItemInHands(Slots[idx].Item.Get());
	return true;
}

void USlotComponent::EquipItemInHands(AItemBase* NewItem)
{
	// 무기를 손에 장착하는 함수
	if (!NewItem || !NewItem->Spec) return;
	if (CurrentInHands.Get() == NewItem)return;

	if (bIsEquipping) return;
	TGuardValue<bool> Guard(bIsEquipping, true);
	
	ASurvivor* OwnerChar = Cast<ASurvivor>(GetOwner());
	if (!OwnerChar) return;
	USceneComponent* Parent = (OwnerChar->Arms ? static_cast<USceneComponent*>(OwnerChar->Arms)
										   : static_cast<USceneComponent*>(OwnerChar->GetMesh()));
	const FName Socket = NewItem->GetAttachSocketName();

	TWeakObjectPtr<AItemBase> Prev = CurrentInHands;
	UItemSpec* PrevSpec = (Prev.IsValid() ? Prev->Spec : nullptr);

	const bool bNewIsSlot = NewItem->Spec->bOccupiesSlot; // 새로운 무기 슬롯형인가요?
	const bool bNewConsumable = NewItem->Spec->bConsumesOnUse; // 새로운 무기 소비형 인가요?
	const bool bPrevIsNonSlot = (Prev.IsValid() && PrevSpec && !PrevSpec->bOccupiesSlot); // 이전무기가 비슬롯인가요?
	
	if (bPrevIsNonSlot && bNewIsSlot)
	{
		Drop(Prev.Get(), false);
		Prev = nullptr;
	}

	// 소모형/비슬롯을 들 때만 이전무기를 스택에 보관
	if ((!bNewIsSlot || bNewConsumable) && Prev.IsValid())
	{
		if (PreviousItem.Num() == 0 || PreviousItem.Last() != Prev)
		{
			PreviousItem.Add(Prev);
		}
	}
	
	// 이전 아이템 정리
	if (Prev.IsValid())
	{
		BindConsumption(Prev.Get(), false);
		Prev->OnUnequipped();
	}

	CurrentInHands = NewItem;
	NewItem->OnEquipped(OwnerChar, Parent, Socket);
	BindConsumption(NewItem, true);

	NotifyInHandsChanged(NewItem);
	if (bNewIsSlot)
	{
		NotifySlotChanged(NewItem->Spec->PreferredSlot, NewItem);
	}
}

void USlotComponent::BindConsumption(AItemBase* Item, bool bBind)
{
	// 소비 아이템 바인딩
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
	// 소비 아이템 소모
	if (!Used || !Used->Spec) return;
	
	// 슬롯에 들어있던 소비 아이템 슬롯에서 제거
	if (Used->Spec->bOccupiesSlot)
	{
		const int32 idx = Index(Used->Spec->PreferredSlot);
		if (idx > 0 && idx < Slots.Num() && Slots[idx].Item.Get() == Used)
		{
			Slots[idx].Item = nullptr;
			RemoveItemFromSlot(Used);
			NotifySlotChanged(Used->Spec->PreferredSlot, nullptr);
		}
	}

	// 현재 손에 들고 있었다면 비우기
	if (CurrentInHands.Get() == Used)
	{
		BindConsumption(Used, false);
		CurrentInHands = nullptr;
		NotifyInHandsChanged(nullptr);
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
	// Slot 순회 함수 (마우스 휠, Q 전용)
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
	// 현재 손에 들고 있는 아이템 정리
	if (!CurrentInHands.IsValid() || !CurrentInHands->Spec)	return;

	AItemBase* Item = CurrentInHands.Get();

	if (Item->Spec->bOccupiesSlot)
	{
		const int32 PrevIdx = Index(Item->Spec->PreferredSlot);
		if (PrevIdx > 0 && PrevIdx < Slots.Num())
		{
			const bool bSlotEmpty    = !Slots[PrevIdx].Item.IsValid();
			const bool bSlotHasSelf  = (Slots[PrevIdx].Item.Get() == Item);

			UE_LOG(LogTemp, Warning, TEXT("[ReturnOrDrop] item=%s slot=%d slotPtr=%s self?=%d empty?=%d"),
				*GetNameSafe(Item),
				PrevIdx,
				*GetNameSafe(Slots[PrevIdx].Item.Get()),
				Slots[PrevIdx].Item.Get() == Item,
				!Slots[PrevIdx].Item.IsValid());

			if (bSlotEmpty || bSlotHasSelf)
			{
				// 손에서만 내려놓고 슬롯에는 남겨둠(또는 비어있으면 채움)
				BindConsumption(Item, false);
				CurrentInHands = nullptr;
				if (bSlotEmpty)
				{
					Slots[PrevIdx].Item = Item;
				}
				Item->OnUnequipped();

				NotifyInHandsChanged(nullptr);
				// 슬롯 내용은 동일하지만 UI 갱신이 필요하면 유지
				NotifySlotChanged(Item->Spec->PreferredSlot, Item);
				return;
			}
		}
	}

	// 여기로 오면 진짜 본래 자리에 둘 수 없는 경우 → Drop
	Drop(Item, true);
}

void USlotComponent::Drop(AItemBase* Item, bool bAllowFallBack)
{
	// 버리기
	if (!Item) return;

	// 손에서 드롭하는 경우 미리 fallback 후보 확보
	AItemBase* Fallback = nullptr;
	if (bAllowFallBack && CurrentInHands.Get() == Item)
	{
		Fallback = GetFallBackItem(Item);
	}

	RemoveItemFromSlot(Item);
	// 손에서 분리 및 물리 활성
	Item->OnDropped();

	// 즉시 대체 장착(빈 손 방지)
	if (Fallback)
	{
		EquipItemInHands(Fallback);
	}
}

void USlotComponent::RemoveItemFromSlot(AItemBase* Item)
{
	if (!IsValid(Item)) return;

	// 슬롯에서 제거
	if (Item->Spec && Item->Spec->bOccupiesSlot)
	{
		const int32 idx = Index(Item->Spec->PreferredSlot);
		if (idx > 0 && idx < Slots.Num() && Slots[idx].Item.Get() == Item)
		{
			Slots[idx].Item = nullptr;
			NotifySlotChanged(Item->Spec->PreferredSlot, nullptr); // UI/게임플레이 알림
		}
	}

	// 손 비우기
	if (CurrentInHands.Get() == Item)
	{
		BindConsumption(Item, false);
		CurrentInHands = nullptr;
		NotifyInHandsChanged(nullptr);
	}

	// 이전 무기 스택에서 제거
	for (int32 i = PreviousItem.Num() - 1; i >= 0; --i)
	{
		if (!PreviousItem[i].IsValid() || PreviousItem[i].Get() == Item)
		{
			PreviousItem.RemoveAt(i);
		}
	}
}

AItemBase* USlotComponent::GetFallBackItem(const AItemBase* Old) const
{
	// 소모한 이후 대체 장착 후보 찾기
	// Previous 순회
	for (int32 i = PreviousItem.Num() - 1; i >= 0; --i)
	{
		if (PreviousItem[i].IsValid() && PreviousItem[i].Get() != Old)
		{
			return PreviousItem[i].Get();
		}
	}

	// 그 다음으로 슬롯 순회
	const int32 Last = FMath::Min(4, Slots.Num() - 1);
	for (int32 idx = 1; idx <= Last; ++idx)
	{
		if (Slots[idx].Item.IsValid() && Slots[idx].Item.Get() != Old)
		{
			return Slots[idx].Item.Get();
		}
	}
	return nullptr; // 못찾음
}

void USlotComponent::HandleUse(EUsingType Phase, float Elapsed)
{
	if (CurrentInHands.IsValid())
	{
		CurrentInHands->HandleUse(Phase, Elapsed);
		UE_LOG(LogTemp, Warning, TEXT("CurrentInHands.IsValid()"));
	}
}

void USlotComponent::RightClickUse()
{
	// 들고 있는 무기의 우클릭 함수 실행
	if (CurrentInHands.IsValid())
	{
		CurrentInHands->RightAttack();
	}
}

EItemType USlotComponent::GetActiveItemType()
{
	if (!CurrentInHands.IsValid()) return EItemType::None;
	
	return (CurrentInHands->Spec->ItemType);
}

bool USlotComponent::GetSlotIcon(ESlotType Slot, UTexture2D*& OutIcon) const
{
	const int32 idx = Index(Slot);
	if (idx <= 0 || idx >= Slots.Num())
	{
		OutIcon = nullptr;
		return false;
	}
	if (!Slots[idx].Item.IsValid())
	{
		OutIcon = nullptr;
		return false;
	}

	OutIcon = Slots[idx].Item->GetUIIcon();
	return (OutIcon != nullptr);
}


void USlotComponent::NotifyInHandsChanged(AItemBase* Item)
{
	// 게임플레이/애님 알림
	OnInHandsItemChanged.Broadcast(Item);

	// UI 알림
	ESlotType Slot = ESlotType::None;
	if (Item && Item->Spec && Item->Spec->bOccupiesSlot)
	{
		Slot = Item->Spec->PreferredSlot;
	}
	UTexture2D* Icon = Item ? Item->GetUIIcon() : nullptr;
	OnUIInHandsChanged.Broadcast(Slot, Icon);
}

void USlotComponent::NotifySlotChanged(ESlotType Slot, AItemBase* Item)
{
	// 게임플레이/애님 알림
	OnSlotItemChanged.Broadcast(Slot, Item);

	// UI 알림
	UTexture2D* Icon = nullptr;
	if (Item)
	{
		Icon = Item->GetUnEquipUIIcon() ? Item->GetUnEquipUIIcon() : Item->GetUIIcon();
	}
	OnUISlotItemChanged.Broadcast(Slot, Icon);
}
