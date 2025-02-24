// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UISurvivorMedKit.generated.h"

/**
 * 
 */
UCLASS()
class RIGHT4DEAD_API UUISurvivorMedKit : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* MedKitProgressBar;
};
