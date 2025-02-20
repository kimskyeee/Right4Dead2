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
	//Notify Event
	UFUNCTION()
	void AnimNotify_AxeCameraShake();
	UFUNCTION()
	void AnimNotify_SpawnCylinder();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	EWeaponType CurrentWeapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FSM")
	bool bIsEquippedWeapon=false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FSM")
	bool PrimaryWeapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FSM")
	bool SecondaryWeapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FSM")
	bool MeleeWeapon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FSM")
	float speed = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FSM")
	float direction = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FSM")
	bool bIsInAir=false;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
