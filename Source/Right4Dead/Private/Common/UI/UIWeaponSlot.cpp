// Fill out your copyright notice in the Description page of Project Settings.


#include "UIWeaponSlot.h"

#include "Components/Overlay.h"

//TArray는 동적배열
//AWeaponBase 타입의 객체를 저장하는 배열이고
//& WeaponInstances 참조를 통해 함수에서도 원본 배열을 직접 수정할 수 있다
void UUIWeaponSlot::UpdateSlot(int32 SelectedSlot, const TArray<AWeaponBase*>& WeaponInstances)
{
	// 각 슬롯의 기본 이미지 배열 (초기 UI 상태)
	TArray<UImage*> DefaultImages = { FirstBGImage, SecondBGImage, ThirdBGImage, FourthBGImage, FifthBGImage };
	
	// 모든 슬롯의 정보를 돌아보면서 UI를 업데이트 해보자
	for (int32 i=0; i<WeaponSlots.Num(); i++)
	{
		// 선택된 슬롯이면 초록색 UI를 활성화 할 것이다
		// 현재 슬롯이 선택된 슬롯인지 확인한다
		bool bIsSelected = (i ==SelectedSlot);
		// 초록색 UI를 설정한다 (선택된 슬롯이 아니면 hidden할거임)
		WeaponSlots[i].GreenIndicator->SetVisibility(bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

		// 근데 해당 슬롯에 무기가 있나?
		if (WeaponInstances.IsValidIndex(i) && WeaponInstances[i])
		{
			// 있으면 무기 데이터 가져오자
			const FWeaponData& WeaponData = WeaponInstances[i]->WeaponData;

			// 이미지 업데이트 할거임
			// 장착한 슬롯이면 EquipTexture, 아닐 경우 UnequipTexture 사용
			UTexture2D* Texture = bIsSelected ? WeaponData.WeaponEquipSlotUI 
											  : WeaponData.WeaponUnEquipSlotUI;

			// 슬롯의 무기 이미지를 업데이트 (Brush를 Texture로 변경)
			WeaponSlots[i].WeaponImage->SetBrushFromTexture(Texture);
		}

		else
		{
			if (UTexture2D* DefaultTexture = Cast<UTexture2D>(DefaultImages[i]->Brush.GetResourceObject()))
			{
				WeaponSlots[i].WeaponImage->SetBrushFromTexture(DefaultTexture);
			}
		}		
	}
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

