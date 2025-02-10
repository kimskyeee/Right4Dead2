// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/CommonZombie.h"

#include "AIController.h"
#include "Right4DeadGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Right4Dead/Right4Dead.h"

// Sets default values
ACommonZombie::ACommonZombie()
{
	PRINT_CALLINFO();
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Hp = 50.0f;
	Speed = 250.0f;
	AIControllerClass = AAIController::StaticClass();
}

// Called when the game starts or when spawned
void ACommonZombie::BeginPlay()
{
	Super::BeginPlay();

	if (Target)
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController)
		{
			AIController->MoveToActor(Target, 30);
		}
	}
}

void ACommonZombie::InitDifficulty()
{
	// GameInstance 가져오기
	if (const URight4DeadGameInstance* GameInstance = Cast<URight4DeadGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		switch (GameInstance->GetDifficulty())
		{
		case EDifficulty::Easy:
			TakeDamageMultiplier = 2; // 200%
			AttackDamage = 1;
			break;
		case EDifficulty::Normal:
			TakeDamageMultiplier = 1; // 100%
			AttackDamage = 2;
			break;
		case EDifficulty::Advanced:
			TakeDamageMultiplier = 0.75f; // 75%
			AttackDamage = 5;
			break;
		case EDifficulty::Expert:
			TakeDamageMultiplier = 0.5f; // 50%
			AttackDamage = 20;
			break;
		}
	}
}

// Called every frame
void ACommonZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bClimbing)
	{
		FVector P0 = GetActorLocation();
		
		if (bFirst && FVector::Dist(P0, FD) < 5.0f)
		{
			GetCharacterMovement()->StopMovementImmediately();
			bFirst = false;
			bSecond = true;
		}

		if (bSecond && FVector::Dist(P0, SD) < 5.0f)
		{
			GetCharacterMovement()->StopMovementImmediately();
			bClimbing = false;
			bSecond = false;
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			AAIController* AIController = Cast<AAIController>(GetController());
			if (AIController)
			{
				AIController->MoveToActor(Target, 30);
			}
		}
		
		if (bFirst)
		{
			FVector MoveDirection = (FD - P0).GetSafeNormal();
			FVector P = P0 + MoveDirection * Speed * DeltaTime;
			SetActorLocation(P);
		}
		if (bSecond)
		{
			FVector MoveDirection = (SD - P0).GetSafeNormal();
			FVector P = P0 + MoveDirection * Speed * DeltaTime;
			SetActorLocation(P);
		}
	}
}

void ACommonZombie::StartClimbing(FVector FirstDest, FVector SecondDest)
{
	FD = FirstDest;
	SD = SecondDest;
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->StopMovement();
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	}
	bClimbing = true;
	bFirst = true;
}