// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UISurvivorHP.generated.h"

/**
 * 
 */
UCLASS()
class RIGHT4DEAD_API UUISurvivorHP : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HPProgressBar;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HPText;

	void SetHealthBarPercent(float CurrentHealth, float MaxHealth);
	
};
