// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAnimInstance.h"

#include "ZombieBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UZombieAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Zombie = Cast<AZombieBase>(GetOwningActor());
	if (Zombie)
	{
		MovementComponent = Zombie->GetCharacterMovement();
		SkeletalMeshComponent = Zombie->GetMesh();
	}
	AnimSeed = UKismetMathLibrary::RandomIntegerInRange(MIN_uint8, MAX_uint8);
}

void UZombieAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (MovementComponent)
	{
		Velocity = MovementComponent->Velocity;
		GroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);
		ShouldMove = GroundSpeed > 3.0f && UKismetMathLibrary::NotEqual_VectorVector(MovementComponent->GetCurrentAcceleration(), FVector::ZeroVector, 0.0f);
		IsFalling = MovementComponent->IsFalling();
	}
}