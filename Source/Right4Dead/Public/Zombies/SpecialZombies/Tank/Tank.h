// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieBase.h"
#include "GameFramework/Actor.h"
#include "Tank.generated.h"

UCLASS()
class RIGHT4DEAD_API ATank : public AZombieBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATank();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void InitData() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
