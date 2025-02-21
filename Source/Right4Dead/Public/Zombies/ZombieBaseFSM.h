// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ZombieBaseFSM.generated.h"

UENUM(BlueprintType)
enum class EZombieState : uint8
{
	EZS_Idle,
	EZS_Chase,
	EZS_Attack,
	EZS_Dead
};

UCLASS(Abstract)
class RIGHT4DEAD_API UZombieBaseFSM : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UZombieBaseFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Category="Debugging")
	EZombieState State = EZombieState::EZS_Idle;
	void SetState(const EZombieState NewState);
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void StartIdle() PURE_VIRTUAL(UZombieBaseFSM::StartIdle, );
	virtual void TickIdle() PURE_VIRTUAL(UZombieBaseFSM::TickIdle, );
	virtual void EndIdle() PURE_VIRTUAL(UZombieBaseFSM::EndIdle, );

	virtual void StartChase() PURE_VIRTUAL(UZombieBaseFSM::StartChase, );
	virtual void TickChase() PURE_VIRTUAL(UZombieBaseFSM::TickChase, );
	virtual void EndChase() PURE_VIRTUAL(UZombieBaseFSM::EndChase, );
	
	virtual void StartAttack() PURE_VIRTUAL(UZombieBaseFSM::StartAttack, );
	virtual void TickAttack() PURE_VIRTUAL(UZombieBaseFSM::TickAttack, );
	virtual void EndAttack() PURE_VIRTUAL(UZombieBaseFSM::EndAttack, );
	
	virtual void StartDead() PURE_VIRTUAL(UZombieBaseFSM::StartDead, );
	virtual void TickDead() PURE_VIRTUAL(UZombieBaseFSM::TickDead, );
	virtual void EndDead() PURE_VIRTUAL(UZombieBaseFSM::EndDead, );
};
