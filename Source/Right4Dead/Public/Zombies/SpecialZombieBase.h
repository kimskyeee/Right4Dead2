// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieBase.h"
#include "SpecialZombieBase.generated.h"

class USpecialZombieBaseFSM;

UCLASS(Abstract)
class RIGHT4DEAD_API ASpecialZombieBase : public AZombieBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASpecialZombieBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	TObjectPtr<USpecialZombieBaseFSM> SpecialZombieFSM = nullptr;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
