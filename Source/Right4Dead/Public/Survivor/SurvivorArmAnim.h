// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survivor.h"
#include "Animation/AnimInstance.h"
#include "SurvivorArmAnim.generated.h"

/**
 * 
 */
UCLASS()
class RIGHT4DEAD_API USurvivorArmAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	EWeaponType CurrentWeapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FSM")
	bool bIsEquippedWeapon=false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FSM")
	bool bIsInAir=false;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
