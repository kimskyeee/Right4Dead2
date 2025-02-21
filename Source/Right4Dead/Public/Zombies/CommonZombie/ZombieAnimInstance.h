// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieFSM.h"
#include "Animation/AnimInstance.h"
#include "ZombieAnimInstance.generated.h"

class UCommonZombieFSM;
enum class EZombieState : uint8;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class RIGHT4DEAD_API UZombieAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, Category = "References")
	TObjectPtr<ACommonZombie> Zombie = nullptr;
	UPROPERTY(BlueprintReadOnly)
	uint8 AnimSeed = 0;
	UPROPERTY(BlueprintReadOnly)
	EZombieState ZombieState = EZombieState::EZS_Idle;
	UPROPERTY(BlueprintReadOnly, Category = "Essential Movement Data")
	bool ShouldMove = false;
	UPROPERTY(BlueprintReadOnly, Category = "Essential Movement Data")
	bool IsFalling = false;
	UPROPERTY(BlueprintReadOnly, Category = "Essential Movement Data")
	bool IsClimbing = false;
	UPROPERTY(BlueprintReadOnly, Category = "Essential Movement Data")
	float GroundSpeed = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Essential Movement Data")
	FVector Velocity = FVector::ZeroVector;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAttack();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayKnockBack(const double Theta);
};
