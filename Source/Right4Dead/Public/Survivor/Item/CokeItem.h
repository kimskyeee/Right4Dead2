// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConsumableItem.h"
#include "CokeItem.generated.h"

UCLASS()
class RIGHT4DEAD_API ACokeItem : public AConsumableItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACokeItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ApplyItemEffect() override;
	void CheckDeliveryPos();
};
