// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UISurvivorMain.generated.h"

/**
 * 
 */
UCLASS()
class RIGHT4DEAD_API UUISurvivorMain : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UUISurvivorHP* PlayerHP;
	UPROPERTY(meta = (BindWidget))
	class UUISurvivorCrosshair* Crosshair;
};
