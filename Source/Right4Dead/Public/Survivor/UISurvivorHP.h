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
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HPProgressBar;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HPText;
	
};
