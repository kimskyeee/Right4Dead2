// Fill out your copyright notice in the Description page of Project Settings.


#include "UIWeaponSlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Item/FireWeapon.h"
#include "Item/SlotComponent.h"

void UUIWeaponSlot::NativeConstruct()
{
	Super::NativeConstruct();

	Slots.SetNum(5);

	InitUISlot();

	// 델리게이트 연결
	BindAll();
}

void UUIWeaponSlot::NativeDestruct()
{
	UnbindAll();
	Super::NativeDestruct();
}

void UUIWeaponSlot::InitUISlot()
{
	Slots[1].BGWhite = S1_BGWhite;
	Slots[1].BGGreen = S1_BGGreen;
	Slots[1].P_IconNormal = S1_IconNormal;
	Slots[1].P_IconEquip = S1_IconEquip;
	Slots[1].TextCur = S1_TextCurrent;
	Slots[1].TextMax = S1_TextMax;
	Slots[1].DefaultNormalIcon = S1_DefaultNormal;

	Slots[2].BGWhite = S2_BGWhite;
	Slots[2].BGGreen = S2_BGGreen;
	Slots[2].IconNormal = S2_IconNormal;
	Slots[2].DefaultNormalIcon = S2_DefaultNormal;

	Slots[3].BGWhite = S3_BGWhite;
	Slots[3].BGGreen = S3_BGGreen;
	Slots[3].IconNormal = S3_IconNormal;
	Slots[3].DefaultNormalIcon = S3_DefaultNormal;

	Slots[4].BGWhite = S4_BGWhite;
	Slots[4].BGGreen = S4_BGGreen;
	Slots[4].IconNormal = S4_IconNormal;
	Slots[4].DefaultNormalIcon = S4_DefaultNormal;

	// 초기 비활성
	for (int i=1; i<=4; ++i)
	{
		ApplyActive(i, false);
	}
}

void UUIWeaponSlot::HandleInHandsChanged(ESlotType ActiveSlot, UTexture2D* ActiveIcon)
{
	// 전체 비활성화
	for (int i=1; i<=4; ++i)
	{
		ApplyActive(i, false);
	}

	// 활성 적용
	const int idx = Index(ActiveSlot);
	ActiveIdx = idx;
	if (idx >= 1 && idx <= 4)
	{
		SetActiveIcon(idx, ActiveIcon);
		ApplyActive(idx, true);

		if (idx == 1)
		{
			BindPrimaryAmmo();
		}
		else
		{
			UnbindPrimaryAmmo();
		}
	}
}

void UUIWeaponSlot::HandleSlotIconChanged(ESlotType NewSlot, UTexture2D* NewIcon)
{
	const int idx = Index(NewSlot);
	if (idx < 1 || idx > 4) return;

	// 슬롯 변화
	if (idx == 1)
	{
		// 프라이머리 캐시 갱신
		Slots[1].CachedUnequipIcon = NewIcon;

		// 현재 비활성 상태라면 즉시 갱신
		if (Slots[1].BGGreen && Slots[1].BGGreen->GetVisibility() != ESlateVisibility::Visible)
		{
			ApplyActive(1, false);
		}
	}
	else
	{
		// 2~4: 비활성 상태라면 항상 기본 아이콘 복귀
		const bool bIsActive = (ActiveIdx == idx);
		if (!bIsActive)
		{
			if (Slots[idx].IconNormal && Slots[idx].DefaultNormalIcon)
			{
				Slots[idx].IconNormal->SetBrushFromTexture(Slots[idx].DefaultNormalIcon);
			}
		}
	}
}

void UUIWeaponSlot::HandleSlotItemChangedPtr(ESlotType SlotType, AItemBase* Item)
{
	const int idx = Index(SlotType);
	if (idx != 1) return;

	// 무기 데이터에서 아이콘 선택: UnEquipUIIcon 우선, 없으면 UIIcon
	UTexture2D* Unequip = nullptr;
	if (!Item) return;
	Unequip = Item->UnEquipUIIcon ? Item->UnEquipUIIcon : Item->UIIcon;

	Slots[1].CachedUnequipIcon = Unequip;

	// 프라이머리가 비활성인 경우 즉시 갱신
	const bool bActive = (Slots[1].BGGreen && Slots[1].BGGreen->GetVisibility() == ESlateVisibility::Visible);
	if (!bActive && Slots[1].P_IconNormal)
	{
		UTexture2D* Tex = Unequip ? Unequip : Slots[1].DefaultNormalIcon;
		if (!Tex) return;
		Slots[1].P_IconNormal->SetBrushFromTexture(Tex);
	}
}

void UUIWeaponSlot::ApplyActive(int32 Idx, bool bActive)
{
	if (!Slots.IsValidIndex(Idx)) return;
    FSlotView& SlotView = Slots[Idx];

    if (SlotView.BGGreen)
    {
        SlotView.BGGreen->SetVisibility(bActive ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }

    if (Idx == 1)
    {
        // 1번: Normal/Equip
        if (bActive)
        {
            if (SlotView.P_IconEquip) SlotView.P_IconEquip->SetVisibility(ESlateVisibility::Visible);
            if (SlotView.P_IconNormal) SlotView.P_IconNormal->SetVisibility(ESlateVisibility::Hidden);
            if (SlotView.TextCur) SlotView.TextCur->SetVisibility(ESlateVisibility::Hidden);
            if (SlotView.TextMax) SlotView.TextMax->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            // 비활성: Unequip 캐시가 있으면 그걸, 없으면 Normal
        	if (SlotView.P_IconNormal)
        	{
        		UTexture2D* Tex = SlotView.CachedUnequipIcon ? SlotView.CachedUnequipIcon : SlotView.DefaultNormalIcon;
        		if (Tex) SlotView.P_IconNormal->SetBrushFromTexture(Tex);
        		SlotView.P_IconNormal->SetVisibility(ESlateVisibility::Visible);
        	}
        	if (SlotView.P_IconEquip) SlotView.P_IconEquip->SetVisibility(ESlateVisibility::Hidden);

        	// 텍스트 개별 Off
        	if (SlotView.TextCur) SlotView.TextCur->SetVisibility(ESlateVisibility::Collapsed);
        	if (SlotView.TextMax) SlotView.TextMax ->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
    else
    {
    	if (SlotView.IconNormal)
    	{
    		if (!bActive && SlotView.DefaultNormalIcon)
    		{
    			SlotView.IconNormal->SetBrushFromTexture(SlotView.DefaultNormalIcon);
    		}
    		SlotView.IconNormal->SetVisibility(ESlateVisibility::Visible);
    	}
    }
}

void UUIWeaponSlot::SetActiveIcon(int32 Idx, UTexture2D* Icon)
{
	if (!Slots.IsValidIndex(Idx) || !Icon) return;
	FSlotView& SlotView = Slots[Idx];

	if (Idx == 1)
	{
		if (!SlotView.P_IconEquip) return;
		SlotView.P_IconEquip->SetBrushFromTexture(Icon);
	}
	else
	{
		if (!SlotView.IconNormal) return;
		SlotView.IconNormal->SetBrushFromTexture(Icon); // 활성 시에는 무기 아이콘으로 교체
	}
}

void UUIWeaponSlot::SetInactiveVisual(int32 Idx)
{
	// 외부에서 비활성화할 일이 있으면 호출
	ApplyActive(Idx, false);
}

void UUIWeaponSlot::BindAll()
{
	if (!SlotComp) return;

	// 중복 바인딩 방지
	SlotComp->OnUIInHandsChanged.RemoveDynamic(this, &UUIWeaponSlot::HandleInHandsChanged);
	SlotComp->OnUISlotItemChanged.RemoveDynamic(this, &UUIWeaponSlot::HandleSlotIconChanged);
	SlotComp->OnSlotItemChanged.RemoveDynamic(this, &UUIWeaponSlot::HandleSlotItemChangedPtr);
	
	SlotComp->OnUIInHandsChanged.AddDynamic(this, &UUIWeaponSlot::HandleInHandsChanged);
	SlotComp->OnUISlotItemChanged.AddDynamic(this, &UUIWeaponSlot::HandleSlotIconChanged);
	SlotComp->OnSlotItemChanged.AddDynamic(this, &UUIWeaponSlot::HandleSlotItemChangedPtr);
}

void UUIWeaponSlot::UnbindAll()
{
	UnbindPrimaryAmmo();

	if (!SlotComp) return;
	SlotComp->OnUIInHandsChanged.RemoveDynamic(this, &UUIWeaponSlot::HandleInHandsChanged);
	SlotComp->OnUISlotItemChanged.RemoveDynamic(this, &UUIWeaponSlot::HandleSlotIconChanged);
	SlotComp->OnSlotItemChanged.RemoveDynamic(this, &UUIWeaponSlot::HandleSlotItemChangedPtr);
}

void UUIWeaponSlot::BindPrimaryAmmo()
{
	if (!SlotComp) return;
	if (AItemBase* Item = SlotComp->GetInHandsRaw())
	{
		if (auto* Fire = Cast<AFireWeapon>(Item))
		{
			// 같은 무기에 바인드 되어 있으면 스킵
			if (BoundFire.Get() == Fire) return;

			// 다른 무기 바인딩을 해제
			UnbindPrimaryAmmo();

			BoundFire = Fire;
			Fire->OnAmmoChanged.AddDynamic(this, &UUIWeaponSlot::OnAmmoChanged);

			OnAmmoChanged(Fire->GetCurrentAmmo(), Fire->GetMaxAmmoAmount());
		}
	}
}

void UUIWeaponSlot::UnbindPrimaryAmmo()
{
	if (AItemBase* Item = SlotComp->GetInHandsRaw())
	{
		if (auto* Fire = Cast<AFireWeapon>(Item))
		{
			Fire->OnAmmoChanged.RemoveDynamic(this, &UUIWeaponSlot::OnAmmoChanged);
		}
	}
	
	BoundFire = nullptr;
}

void UUIWeaponSlot::OnAmmoChanged(int32 Cur, int32 Max)
{
	FSlotView& SlotView = Slots[1];
	if (SlotView.TextCur) SlotView.TextCur->SetText(FText::AsNumber(Cur));
	if (SlotView.TextMax)  SlotView.TextMax ->SetText(FText::AsNumber(Max));
}

int32 UUIWeaponSlot::Index(ESlotType SlotType) const
{
	switch (SlotType)
	{
	case
		ESlotType::PrimarySlot:
		return 1;
	case
		ESlotType::SecondarySlot:
		return 2;
	case
		ESlotType::ThrownSlot:
		return 3;
	case
		ESlotType::HandleSlot:
		return 4;
	default: return 0;
	}
}
