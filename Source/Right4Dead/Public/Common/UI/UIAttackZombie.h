// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIAttackZombie.generated.h"

/**
 * 
 */
UCLASS()
class RIGHT4DEAD_API UUIAttackZombie : public UUserWidget
{
	GENERATED_BODY()

public:
	void PlayAnimationByName(class ASurvivor* player);
	
	// Image
	UPROPERTY(meta = (BindWidget))
	class UImage* Blood1;
	UPROPERTY(meta = (BindWidget))
	class UImage* Blood2;
	UPROPERTY(meta = (BindWidget))
	class UImage* Blood3;
	UPROPERTY(meta = (BindWidget))
	class UImage* Blood4;
	
	// Animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* BD1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* BD2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* BD3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* BD4;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayBloodSpraySound();
};
