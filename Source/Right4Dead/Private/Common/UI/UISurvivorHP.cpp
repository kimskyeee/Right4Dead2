// Fill out your copyright notice in the Description page of Project Settings.


#include "UISurvivorHP.h"

#include "Survivor.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UUISurvivorHP::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUISurvivorHP::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	ASurvivor* player = Cast<ASurvivor>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (player)
	{
		float percent = player->CurrentHP / player->MaxHP;
		HPProgressBar->SetPercent(percent);

		// %3.0f 사용 - 최대 3자리까지 표현하되 앞의 0은 없앰
		FString HPString = FString::Printf(TEXT("+%3.0f"), percent * 100);
		FText HPNum = FText::FromString(HPString);
		HPText->SetText(HPNum);
	}
}
