// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivorArmAnim.h"

#include "GameFramework/CharacterMovementComponent.h"

void USurvivorArmAnim::AnimNotify_AxeCameraShake()
{
	//카메라 쉐이크 재생?
	auto player = Cast<ASurvivor>(GetWorld()->GetFirstPlayerController()->GetPawn());
	auto pc = GetWorld()->GetFirstPlayerController();
	if (pc)
	{
		pc->PlayerCameraManager->StartCameraShake(player->SweepCameraShake);
	}
}

void USurvivorArmAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//ABP소유하고 있는 pawn 얻어오기
	auto ownerPawn = TryGetPawnOwner();
	//TPS player로 cast후 speed값설정
	auto player = Cast<ASurvivor>(ownerPawn);
	if (player)
	{
		//방향, 속도
		FVector velocity = player->GetVelocity();
		FVector forward = player->GetActorForwardVector();
		speed = FVector::DotProduct(velocity, forward);

		FVector right = player->GetActorRightVector();
		direction = FVector::DotProduct(velocity, right);
		
		//공중에 있는지 여부
		bIsInAir = player->GetCharacterMovement()->IsFalling();
	}
}
