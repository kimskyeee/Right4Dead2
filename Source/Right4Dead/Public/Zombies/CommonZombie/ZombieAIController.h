// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombieAIController.generated.h"

class AZombieBase;
class ACommonZombie;

UCLASS()
class RIGHT4DEAD_API AZombieAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AZombieAIController();

protected:
	
public:
	TObjectPtr<AZombieBase> Zombie = nullptr;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
};
