// Fill out your copyright notice in the Description page of Project Settings.


#include "UISurvivorCrosshair.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"

void UUISurvivorCrosshair::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (BottomCross)
	{
		UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(BottomCross);
		if (CanvasSlot)
		{
			CanvasSlot->SetAutoSize(true);
		}
	}
}
