// Fill out your copyright notice in the Description page of Project Settings.


#include "UIWeaponSlot.h"

#include "Survivor.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

//TArray는 동적배열
//AWeaponBase 타입의 객체를 저장하는 배열이고
//& Inventory 참조를 통해 함수에서도 원본 배열을 직접 수정할 수 있다
/*void UUIWeaponSlot::UpdateSlot(AWeaponBase* EquippedWeapon, const TMap<EWeaponType, AWeaponBase*>& Inventory)
{
	// 각 슬롯의 기본 이미지 배열 (초기 UI 상태)
	TArray<UImage*> DefaultImages = { FirstBGImage, SecondBGImage, ThirdBGImage, FourthBGImage, FifthBGImage };

	// TODO: 일단 이전 방식과 연동을 위해...
	const int32 SlotIndex = static_cast<int32>(EquippedWeapon->SlotType) - 1;
	if (SlotIndex < 0)
	{
		return;
	}

	for (int i = 0; i < WeaponSlots.Num(); i++)
	{
		// 방금 손에 든 무기의 슬롯이 맞는가?
		const bool bIsSelected = (i == SlotIndex);

		// 초록색 UI를 설정한다 (현재 활성화된 슬롯이라면 초록 인디케이터를 표시, 아니면 숨김)
		const ESlateVisibility NewVisibility = bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
		WeaponSlots[i].GreenIndicator->SetVisibility(NewVisibility);

		UTexture2D* Texture = nullptr;
		// 손에 들지 않은 i번째 슬롯에 무기가 있는가? 
		const EWeaponType SlotType = static_cast<EWeaponType>(i + 1);
		if (auto Weapon = Inventory[SlotType])
		{
			// 슬롯에 무기가 있다면...
			// 현재 장착하고 있는 무기라면 장착 이미지를
			// 그렇지 않다면 비장착 이미지를
			Texture = bIsSelected ? Weapon->WeaponData.WeaponEquipSlotUI : Weapon->WeaponData.WeaponUnEquipSlotUI;
		}
		else
		{
			// 슬롯에 무기가 없다면 기본 이미지로 설정해라
			Texture = Cast<UTexture2D>(DefaultImages[i]->GetBrush().GetResourceObject());
		}
		// 최종적으로 i번째 슬롯의 무기 이미지를 업데이트 (Brush를 Texture로 변경)
		WeaponSlots[i].WeaponImage->SetBrushFromTexture(Texture);
	}
}*/

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

void UUIWeaponSlot::NativeConstruct()
{
	Super::NativeConstruct();

	// 슬롯 정보 배열로 저장하자
	WeaponSlots = {
		{First_ON, FirstBGImage},
		{Second_ON, SecondBGImage},
		{Third_ON, ThirdBGImage},
		{Fourth_ON, FourthBGImage},
		{Fifth_ON, FifthBGImage}
	};
}

void UUIWeaponSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	SetAmmoText();
}

