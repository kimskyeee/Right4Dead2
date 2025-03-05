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

void USurvivorArmAnim::AnimNotify_SpawnCylinder()
{
	// 실린더 스윕 발생
	auto player = Cast<ASurvivor>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (player)
	{
		player->spawnShoveCylinder();
	}
}

void USurvivorArmAnim::AnimNotify_ENDReloading()
{
	auto player = Cast<ASurvivor>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (player)
	{
		player->bIsReloading = false;
	}
}

void USurvivorArmAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	//ABP소유하고 있는 pawn 얻어오기
	auto ownerPawn = TryGetPawnOwner();
	//player로 cast후 speed값설정
	Survivor = Cast<ASurvivor>(ownerPawn);
}

void USurvivorArmAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (IsValid(Survivor))
	{
		//방향, 속도
		FVector velocity = Survivor->GetVelocity();
		FVector forward = Survivor->GetActorForwardVector();
		speed = FVector::DotProduct(velocity, forward);

		FVector right = Survivor->GetActorRightVector();
		direction = FVector::DotProduct(velocity, right);
		
		//공중에 있는지 여부
		bIsInAir = Survivor->GetCharacterMovement()->IsFalling();
	}
}
