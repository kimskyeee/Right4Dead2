// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIWeaponSlot.generated.h"

/**
 * 
 */
UCLASS()
class RIGHT4DEAD_API UUIWeaponSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateSlot();

	// Image
	UPROPERTY(meta = (BindWidget))
	class UImage* FirstSlot;
	UPROPERTY(meta = (BindWidget))
	class UOverlay* GUN;
	UPROPERTY(meta = (BindWidget))
	class UImage* SecondSlot;
	UPROPERTY(meta = (BindWidget))
	class UImage* ThirdSlot;
	UPROPERTY(meta = (BindWidget))
	class UImage* FourthSlot;
	UPROPERTY(meta = (BindWidget))
	class UImage* FifthSlot;

	// Equip Slot
	UPROPERTY(meta = (BindWidget))
	class UOverlay* First_ON;
	UPROPERTY(meta = (BindWidget))
	class UOverlay* Second_ON;
	UPROPERTY(meta = (BindWidget))
	class UOverlay* Third_ON;

	// Ammo Count
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentAmmo;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MaxAmmo;

	// Animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* UZI_Anim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* Axe_Anim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* Pipe_Anim;
};
