// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieBaseFSM.h"
#include "SpecialZombieBaseFSM.generated.h"


UCLASS(Abstract)
class RIGHT4DEAD_API USpecialZombieBaseFSM : public UZombieBaseFSM
{
	GENERATED_BODY()

public:
	USpecialZombieBaseFSM();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Debugging|Attack")
	float SpecialAttackRange = 850.0f;
	UPROPERTY(EditAnywhere, Category="Debugging|Attack")
	float SpecialAttackInterval = 20.0f;
	UPROPERTY(VisibleInstanceOnly, Category="Debugging|Attack")
	float CurrentSpecialAttackTime = 20.0f;
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void TickChase() override;
};