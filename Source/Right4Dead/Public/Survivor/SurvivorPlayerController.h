// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SurvivorPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RIGHT4DEAD_API ASurvivorPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<class UUISurvivorCokeDelivery> CokeDeliveryUIClass;
	UPROPERTY()
	class UUISurvivorCokeDelivery* CokeDeliveryUI = nullptr;

	UPROPERTY()
	class USlotComponent* SlotComp = nullptr;
	
	TWeakObjectPtr<class AConsumableItem> BoundConsumable;
	
	UFUNCTION(BlueprintCallable)
	void HandleInHandsChanged(class AItemBase* NewItem);
	void BindConsumable(class AConsumableItem* Item);

	void UnbindConsumable();
	void OnHoldStarted(AConsumableItem* Item, float Duration, double StartTime);
	void OnHoldCanceled(AConsumableItem* Item);
	void OnHoldCompleted(AConsumableItem* Item);
};
