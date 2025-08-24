// Fill out your copyright notice in the Description page of Project Settings.


#include "UISurvivorCokeDelivery.h"
#include "Survivor.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UUISurvivorCokeDelivery::NativeConstruct()
{
	Super::NativeConstruct();

	if (ProgressBar) ProgressBar->SetPercent(0.f);
	SetVisibility(ESlateVisibility::Hidden);
	bActive = false;
}

void UUISurvivorCokeDelivery::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bActive || !ProgressBar) return;

	const double Now = FPlatformTime::Seconds();
	const float Elapsed = static_cast<float>(Now - StartTimeSec);
	const float Alpha = FMath::Clamp(Elapsed / DurationSec, 0.f, 1.f);

	ProgressBar->SetPercent(Alpha);

	if (Alpha >= 1.f)
	{
		// 완료 시 자동 종료(표시를 계속 유지하고 싶으면 주석 처리)
		StopProgress();
	}
}

void UUISurvivorCokeDelivery::SetSimpleVisual(UTexture2D* Icon, const FText& Label)
{
	if (!Img_CurrentState || !Text_CurrentState) return;
	
	Img_CurrentState->SetBrushFromTexture(Icon, true);
	Text_CurrentState->SetText(Label);
}

void UUISurvivorCokeDelivery::StartProgress(float InDurationSec, double InStartTimeSec)
{
	DurationSec = FMath::Max(0.01f, InDurationSec);
	StartTimeSec = InStartTimeSec;
	bActive = true;

	if (!ProgressBar) return;
	ProgressBar->SetPercent(0.f);
	SetVisibility(ESlateVisibility::HitTestInvisible); // 클릭 막고 보이기
}

void UUISurvivorCokeDelivery::StopProgress()
{
	bActive = false;
	if (!ProgressBar) return;
	
	ProgressBar->SetPercent(0.f);
	SetVisibility(ESlateVisibility::Hidden);
}
