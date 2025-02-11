// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "ZombieClimbTriggerBox.generated.h"

class UBoxComponent;

UCLASS()
class RIGHT4DEAD_API AZombieClimbTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AZombieClimbTriggerBox();

protected:
	UFUNCTION()
	void OnClimbEndTriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                      const FHitResult& SweepResult);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> ClimbEndTriggerBox = nullptr;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
