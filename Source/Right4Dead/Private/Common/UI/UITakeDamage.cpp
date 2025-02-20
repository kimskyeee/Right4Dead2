// Fill out your copyright notice in the Description page of Project Settings.


#include "UITakeDamage.h"

#include "Survivor.h"
#include "Kismet/GameplayStatics.h"

void UUITakeDamage::PlayAnimationByName()
{
	auto player = Cast<ASurvivor>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	if (!TakeDamage)
	{
		return;
	}

	if (UWidget* ImageWidget = GetWidgetFromName(TEXT("DamageScreen")))
	{
		ImageWidget->SetVisibility(ESlateVisibility::Visible);
	}
	PlayAnimation(TakeDamage, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);

	// 애니메이션 종료 후 bIsDamaged 초기화
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [player]()
	{
		if (player)
		{
			player->bIsDamaged = false;
		}
	}, 0.5f, false); // 0.5초 후 초기화

	// 애니메이션 종료 후 Visibility를 Hidden으로 변경
	FTimerHandle TimerHandle2;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		if (UWidget* ImageWidget = GetWidgetFromName(TEXT("TakeDamageImage")))
		{
			ImageWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}, 0.5f, false); // 애니메이션 길이만큼 대기 후 숨김
}
