// Fill out your copyright notice in the Description page of Project Settings.


#include "UIAttackZombie.h"
#include "Survivor.h"
#include "Kismet/GameplayStatics.h"

void UUIAttackZombie::PlayAnimationByName(ASurvivor* player)
{
	if (!player)
	{
		return;
	}

	// 애니메이션 골라서 재생
	UWidgetAnimation* CurrentAnimation =nullptr;
	//팩토리에 할당하는거 까먹지마!!!!!!!
	FString ImageName;

	switch (player->AttackCount % 4) // 4넘으면 다시 0으로 순환한다...! 굳!
	{
		case 0:
			CurrentAnimation=BD1;
			ImageName=TEXT("Blood1");
			break;
		case 1:
			CurrentAnimation=BD2;
			ImageName=TEXT("Blood2");
			break;
		case 2:
			CurrentAnimation=BD3;
			ImageName=TEXT("Blood3");
			break;
		case 3:
			CurrentAnimation=BD4;
			ImageName=TEXT("Blood4");
			break;
	}

	// 이미지를 표시해주자
	if (UWidget* ImageWidget = GetWidgetFromName(*ImageName))
	{
		ImageWidget->SetVisibility(ESlateVisibility::Visible);
	}

	// 애니메이션을 재생해주자
	PlayAnimation(CurrentAnimation,0.0f,1,EUMGSequencePlayMode::Forward, 1.0f);
	player->AttackCount++;

	// 애니메이션 종료 후 초기화
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,[this, player, ImageName]()
	{
		if (player)
		{
			player->bIsAttacked = false;
		}
		if (UWidget* ImageWidget = GetWidgetFromName(*ImageName))
		{
			ImageWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}, 5.f, false);
	
}
