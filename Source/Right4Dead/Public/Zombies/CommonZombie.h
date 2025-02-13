// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieAIController.h"
#include "ZombieBase.h"
#include "CommonZombie.generated.h"

UCLASS()
class RIGHT4DEAD_API ACommonZombie : public AZombieBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ACommonZombie();

protected:
	float HitDamageRatio = 2;
	float AttackDamage = 1;
	bool bClimbing = false;
	FTransform ClimbDestination = FTransform::Identity;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void InitDifficulty() override;

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> Target = nullptr;
	UPROPERTY()
	TObjectPtr<AZombieAIController> AIController = nullptr;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void StartClimbing(const FTransform& Destination);
	void EndClimbing();
	AActor* GetChasingTarget();
};