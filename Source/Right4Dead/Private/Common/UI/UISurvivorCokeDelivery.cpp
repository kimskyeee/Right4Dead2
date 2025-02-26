// Fill out your copyright notice in the Description page of Project Settings.


#include "UISurvivorCokeDelivery.h"
#include "Survivor.h"
#include "Components/ProgressBar.h"

void UUISurvivorCokeDelivery::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	ASurvivor* player = Cast<ASurvivor>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (player)
	{
		float percent = player->HoldTime / player->MaxHoldTime;
		CokeProgressBar->SetPercent(percent);
	}
}
