// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorBase.h"
#include "ZombieBase.generated.h"

UCLASS(Abstract)
class RIGHT4DEAD_API AZombieBase : public AActor, public IActorBase
{
	GENERATED_BODY()

protected:
	float Hp = 0;
	float Speed = 0;
	virtual void BeginPlay() override;
	virtual void InitDifficulty() PURE_VIRTUAL(AZombieBase::InitDifficulty, );
	
public:
	AZombieBase();
	virtual void OnDamaged(float Damage) override;
	virtual void OnDie() override;
};