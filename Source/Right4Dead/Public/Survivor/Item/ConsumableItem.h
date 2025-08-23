// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "ConsumableItem.generated.h"

UCLASS()
class RIGHT4DEAD_API AConsumableItem : public AItemBase
{
	GENERATED_BODY()
	float NeedHold = 5.0f; // 5초동안 홀드

public:
	// Sets default values for this actor's properties
	AConsumableItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void ApplyItemEffect();

	virtual void OnTap(float Elapsed) override;
	virtual void OnHoldBegan() override;
	virtual void OnHoldReleased(float Elapsed) override;
};
