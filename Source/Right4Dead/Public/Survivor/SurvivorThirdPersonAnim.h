// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survivor.h"
#include "Animation/AnimInstance.h"
#include "SurvivorThirdPersonAnim.generated.h"


UCLASS()
class RIGHT4DEAD_API USurvivorThirdPersonAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	EWeaponType CurrentWeapon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FSM")
	bool bIsEquippedWeapon=false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FSM")
	bool HandleObject;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FSM")
	bool CokeDelivery;
};
