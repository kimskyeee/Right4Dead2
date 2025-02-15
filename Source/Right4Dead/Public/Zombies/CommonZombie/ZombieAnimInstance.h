// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ZombieAnimInstance.generated.h"

class UCharacterMovementComponent;
class AZombieBase;
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
	TObjectPtr<AZombieBase> Zombie = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "References")
	TObjectPtr<UCharacterMovementComponent> MovementComponent = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "References")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent = nullptr;
	UPROPERTY(BlueprintReadOnly)
	uint8 AnimSeed = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Essential Movement Data")
	bool ShouldMove = false;
	UPROPERTY(BlueprintReadOnly, Category = "Essential Movement Data")
	bool IsFalling = false;
	UPROPERTY(BlueprintReadOnly, Category = "Essential Movement Data")
	float GroundSpeed = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Essential Movement Data")
	FVector Velocity = FVector::ZeroVector;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAttack();
};
