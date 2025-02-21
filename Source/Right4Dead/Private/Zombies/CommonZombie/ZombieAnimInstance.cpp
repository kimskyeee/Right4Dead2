// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAnimInstance.h"

#include "CommonZombie.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Right4Dead/Right4Dead.h"

void UZombieAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	PRINT_CALLINFO();
	Zombie = Cast<ACommonZombie>(GetOwningActor());
	AnimSeed = UKismetMathLibrary::RandomIntegerInRange(MIN_uint8, MAX_uint8);
}

void UZombieAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Zombie)
	{
		ZombieState = Zombie->GetState();
		
		if (MovementComponent)
		{
			Velocity = MovementComponent->Velocity;
			GroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);
			ShouldMove = GroundSpeed > 3.0f && UKismetMathLibrary::NotEqual_VectorVector(MovementComponent->GetCurrentAcceleration(), FVector::ZeroVector, 0.0f);
			IsFalling = MovementComponent->IsFalling();
			IsClimbing = Zombie->bClimbing;
		}
	}
}