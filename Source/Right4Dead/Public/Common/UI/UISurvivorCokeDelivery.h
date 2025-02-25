// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UISurvivorCokeDelivery.generated.h"

/**
 * 
 */
UCLASS()
class RIGHT4DEAD_API UUISurvivorCokeDelivery : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* CokeProgressBar;
};