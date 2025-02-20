// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UITakeDamage.generated.h"

/**
 * 
 */
UCLASS()
class RIGHT4DEAD_API UUITakeDamage : public UUserWidget
{
	GENERATED_BODY()
public:
	
	void PlayAnimationByName();

	UPROPERTY(meta = (BindWidget))
	class UImage* DamageScreen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* TakeDamage;
};
