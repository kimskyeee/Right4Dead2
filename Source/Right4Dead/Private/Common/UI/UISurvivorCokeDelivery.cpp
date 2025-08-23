// Fill out your copyright notice in the Description page of Project Settings.


#include "UISurvivorCokeDelivery.h"
#include "Survivor.h"
#include "Components/ProgressBar.h"

void UUISurvivorCokeDelivery::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	ASurvivor* Player = Cast<ASurvivor>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Player)
	{
		// float Percent = Player->HoldTime / Player->MaxHoldTime;
		// CokeProgressBar->SetPercent(Percent);
	}
}
