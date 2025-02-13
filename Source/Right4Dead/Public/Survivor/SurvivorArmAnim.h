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
	UPROPERTY(BlueprintReadOnly, Category = "References")
	ASurvivor* Survivor;
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	EWeaponType CurrentWeapon;

};
