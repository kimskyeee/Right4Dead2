// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "ZombieClimbTriggerBox.generated.h"

UCLASS()
class RIGHT4DEAD_API AZombieClimbTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AZombieClimbTriggerBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
