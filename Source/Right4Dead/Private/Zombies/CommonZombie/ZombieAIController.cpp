// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAIController.h"

#include "CommonZombie.h"
#include "Navigation/PathFollowingComponent.h"

// Sets default values
AZombieAIController::AZombieAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AZombieAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	Zombie = Cast<ACommonZombie>(InPawn);
}

void AZombieAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	if (Result.IsFailure())
	{
		// UE_LOG(LogTemp, Warning, TEXT("AZombieAIController::OnMoveCompleted::Fail"));
	}

	if (Result.IsInterrupted())
	{
		// UE_LOG(LogTemp, Warning, TEXT("AZombieAIController::OnMoveCompleted::Interrupted"));
	}

	if (Result.IsSuccess())
	{
		// UE_LOG(LogTemp, Warning, TEXT("AZombieAIController::OnMoveCompleted::Success"));
	}
}

