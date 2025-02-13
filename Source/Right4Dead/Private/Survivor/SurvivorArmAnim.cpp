// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivorArmAnim.h"

#include "GameFramework/CharacterMovementComponent.h"

void USurvivorArmAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//ABP소유하고 있는 pawn 얻어오기
	auto ownerPawn = TryGetPawnOwner();
	//TPS player로 cast후 speed값설정
	auto player = Cast<ASurvivor>(ownerPawn);
	if (player)
	{
		//공중에 있는지 여부
		bIsInAir = player->GetCharacterMovement()->IsFalling();
	}
}
