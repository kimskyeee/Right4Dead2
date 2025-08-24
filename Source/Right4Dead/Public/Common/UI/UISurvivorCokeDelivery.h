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
	
protected:
	float  DurationSec   = 0.f;
	double StartTimeSec  = 0.0;
	bool   bActive       = false;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar = nullptr;
	UPROPERTY(meta=(BindWidget))
	class UImage* Img_CurrentState = nullptr;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Text_CurrentState = nullptr;
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void SetSimpleVisual(UTexture2D* Icon, const FText& Label);
	
public:
	UFUNCTION(BlueprintCallable)
	void StartProgress(float InDurationSec, double InStartTimeSec);

	UFUNCTION(BlueprintCallable)
	void StopProgress();
};