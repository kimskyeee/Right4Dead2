// Fill out your copyright notice in the Description page of Project Settings.


#include "UIWeaponSlot.h"

#include "Survivor.h"
#include "Kismet/GameplayStatics.h"

void UUIWeaponSlot::UpdateSlot()
{
	auto player = Cast<ASurvivor>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));

	// 장착 무기별 슬롯 업데이트 하기
	// 1. 장착한 무기가 없으면 -> 활성화된 이미지가 없음 (전부 회색)
	if (player->CurrentWeaponSlot->WeaponName == EWeaponType::None)
	{
		if (UWidget* FirstOverlayWidget = GetWidgetFromName(TEXT("First_ON")))
		{
			FirstOverlayWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		if (UWidget* SecondaryOverlayWidget = GetWidgetFromName(TEXT("Second_ON")))
		{
			SecondaryOverlayWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		if (UWidget* ThirdOverlayWidget = GetWidgetFromName(TEXT("Third_ON")))
		{
			ThirdOverlayWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	// 2. Primary 무기면 -> 1번 슬롯만 초록색 이미지 보이게
	if (player->CurrentWeaponSlot->WeaponName == EWeaponType::Primary)
	{
		if (UWidget* FirstOverlayWidget = GetWidgetFromName(TEXT("First_ON")))
		{
			FirstOverlayWidget->SetVisibility(ESlateVisibility::Visible);
		}
		if (UWidget* SecondaryOverlayWidget = GetWidgetFromName(TEXT("Second_ON")))
		{
			SecondaryOverlayWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		if (UWidget* ThirdOverlayWidget = GetWidgetFromName(TEXT("Third_ON")))
		{
			ThirdOverlayWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		if (UWidget* FirstOverlayWidget = GetWidgetFromName(TEXT("GUN")))
		{
			FirstOverlayWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	// 3. Secondary 무기면 -> 2번 슬롯만 초록색 이미지
	if (player->CurrentWeaponSlot->WeaponName == EWeaponType::Secondary)
	{
		if (UWidget* FirstOverlayWidget = GetWidgetFromName(TEXT("First_ON")))
		{
			FirstOverlayWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		if (UWidget* SecondaryOverlayWidget = GetWidgetFromName(TEXT("Second_ON")))
		{
			SecondaryOverlayWidget->SetVisibility(ESlateVisibility::Visible);
		}
		if (UWidget* ThirdOverlayWidget = GetWidgetFromName(TEXT("Third_ON")))
		{
			ThirdOverlayWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		if (UWidget* FirstOverlayWidget = GetWidgetFromName(TEXT("GUN")))
		{
			FirstOverlayWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
	// 4. Melee 무기면 -> 3번 슬롯만 초록색 이미지
	if (player->CurrentWeaponSlot->WeaponName == EWeaponType::Melee)
	{
		if (UWidget* FirstOverlayWidget = GetWidgetFromName(TEXT("First_ON")))
		{
			FirstOverlayWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		if (UWidget* SecondaryOverlayWidget = GetWidgetFromName(TEXT("Second_ON")))
		{
			SecondaryOverlayWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		if (UWidget* ThirdOverlayWidget = GetWidgetFromName(TEXT("Third_ON")))
		{
			ThirdOverlayWidget->SetVisibility(ESlateVisibility::Visible);
		}
		if (UWidget* FirstOverlayWidget = GetWidgetFromName(TEXT("GUN")))
		{
			FirstOverlayWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}


	/*// 애니메이션 재생	
	if (UWidget* FirstSlotBG = GetWidgetFromName(TEXT("FirstSlot")))
	{
		FirstSlotBG->SetVisibility(ESlateVisibility::Hidden);
	}
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [player]()
	{
		if (player)
		{
			player->bIsDamaged = false;
		}
	}, 3.5f, false); */
}
