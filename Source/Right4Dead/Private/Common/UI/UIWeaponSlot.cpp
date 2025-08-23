// Fill out your copyright notice in the Description page of Project Settings.


#include "UIWeaponSlot.h"

#include "NNETypes.h"
#include "Survivor.h"
#include "Components/Overlay.h"
#include "Item/SlotComponent.h"


void UUIWeaponSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	APlayerController* PC = GetOwningPlayer();
	APawn* Pawn = PC->GetPawn();
	
	if (!Pawn) return;

	/*if (USlotComponent* Slot = Pawn->FindComponentByClass<USlotComponent>())
	{
		Slot->OnSlotItemChanged.AddDynamic(this, &UUIWeaponSlot::HandleSlotItemChanged);
		Slot->OnInHandsChanged.AddDynamic(this, &UUIWeaponSlot::HandleInHandsChanged);

		// 초기 동기화: 현재 슬롯 아이콘 채우기
		UTexture2D* Icon = nullptr;
		if (Slot->GetSlotIcon(ESlotType::PrimarySlot, Icon)) HandleSlotItemChanged(ESlotType::PrimarySlot, Icon);
		if (Slot->GetSlotIcon(ESlotType::SecondarySlot, Icon)) HandleSlotItemChanged(ESlotType::SecondarySlot, Icon);
		if (Slot->GetSlotIcon(ESlotType::ThrownSlot, Icon)) HandleSlotItemChanged(ESlotType::ThrownSlot, Icon);
		if (Slot->GetSlotIcon(ESlotType::HandleSlot, Icon)) HandleSlotItemChanged(ESlotType::HandleSlot, Icon);
	}*/
}

void UUIWeaponSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	SetAmmoText();
}

void UUIWeaponSlot::SetAmmoText()
{
	/*auto player = Cast<ASurvivor>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	if (player && player->CurrentWeapon)
	{
		FText CurrentAmmoCount = FText::AsNumber(player->CurrentWeapon->WeaponData.CurrentAmmo);
		FText MaxAmmoAmount = FText::AsNumber(player->CurrentWeapon->WeaponData.MaxAmmoAmount);

		// null체크를 합시다
		if (CurrentAmmo && MaxAmmo)
		{
			CurrentAmmo->SetText(CurrentAmmoCount);
			MaxAmmo->SetText(MaxAmmoAmount);
		}
	}*/
}

