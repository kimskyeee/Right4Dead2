// Fill out your copyright notice in the Description page of Project Settings.


#include "UISurvivorCrosshair.h"

#include "DataTableEditorUtils.h"
#include "VectorTypes.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"

void UUISurvivorCrosshair::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	float PositionX, PositionY;

	if (BottomCross)
	{
		UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(BottomCross);
		if (CanvasSlot)
		{
			CanvasSlot->SetSize(FVector2D(CrosshairThickness, CrosshairLength));
			PositionX = 0 - ( CrosshairThickness / 2);
			PositionY = CrosshairSpread;
			CanvasSlot->SetPosition(FVector2d(PositionX,PositionY));
		}
	}

	if (TopCross)
	{
		UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(TopCross);
		if (CanvasSlot)
		{
			CanvasSlot->SetSize(FVector2D(CrosshairThickness, CrosshairLength));
			PositionX = 0 - ( CrosshairThickness / 2);
			PositionY = (0-CrosshairLength) -CrosshairSpread;
			CanvasSlot->SetPosition(FVector2d(PositionX,PositionY));
		}
	}

	if (LeftCross)
	{
		UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(LeftCross);
		if (CanvasSlot)
		{
			CanvasSlot->SetSize(FVector2D(CrosshairLength, CrosshairThickness));
			PositionX = (0 - CrosshairLength) - CrosshairSpread;
			PositionY =  0 - (CrosshairThickness / 2);
			CanvasSlot->SetPosition(FVector2d(PositionX,PositionY));
		}
	}
	
	if (RightCross)
	{
		UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(RightCross);
		if (CanvasSlot)
		{
			CanvasSlot->SetSize(FVector2D(CrosshairLength, CrosshairThickness));
			PositionX = (CrosshairLength+CrosshairSpread)-CrosshairLength;
			PositionY =  0 - (CrosshairThickness / 2);
			CanvasSlot->SetPosition(FVector2d(PositionX,PositionY));
		}
	}
}
