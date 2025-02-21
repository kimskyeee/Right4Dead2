// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ZombieBaseFSM.generated.h"

class AZombieBase;

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
	UPROPERTY(VisibleInstanceOnly, Category="Debugging")
	TObjectPtr<AZombieBase> Owner = nullptr;
	
	UPROPERTY(EditAnywhere, Category="Debugging")
	EZombieState State = EZombieState::EZS_Idle;
	void SetState(const EZombieState NewState);

	UPROPERTY(EditAnywhere, Category="Debugging")
	TObjectPtr<AActor> ChaseTarget = nullptr;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	/*
	 *Idle
	 */
	UPROPERTY(VisibleInstanceOnly, Category="Debugging|Idle")
	float CurrentIdleTime = 0.0f;
	UPROPERTY(EditAnywhere, Category="Debugging|Idle")
	float SearchInterval = 1.0f;
	UPROPERTY(EditAnywhere, Category="Debugging|Idle")
	bool bEnableSearch = true;
	UPROPERTY(EditInstanceOnly, Category="Debugging|Idle")
	bool bVerboseSearch = false;
	UPROPERTY(EditAnywhere, Category="Debugging|Idle")
	float Awareness = 1500.0f;
	virtual void StartIdle();
	virtual void TickIdle();
	virtual void EndIdle();

	/*
	 *Chase
	 */
	UPROPERTY(VisibleInstanceOnly, Category="Debugging|Chase")
	float Distance = 0.0f;
	UPROPERTY(VisibleInstanceOnly, Category="Debugging|Chase")
	float CurrentChaseTime = 0.0f;
	UPROPERTY(EditAnywhere, Category="Debugging|Chase")
	float StopChaseTime = 20.0f;
	virtual void StartChase();
	virtual void TickChase();
	virtual void EndChase();
	virtual void TriggerStartChase(const TObjectPtr<AActor>& Target);
	virtual void TriggerStopChase();

	/*
	 *Attack
	 */
	UPROPERTY(EditAnywhere, Category="Debugging|Attack")
	float NormalAttackRange = 150.0f;
	UPROPERTY(EditAnywhere, Category="Debugging|Attack")
    float NormalAttackInterval = 2.0f;
	UPROPERTY(EditAnywhere, Category="Debugging|Attack")
	bool bIsSpecial = false;
	UPROPERTY(EditAnywhere, Category="Debugging|Attack")
	float SpecialAttackRange = 0.0f;
	UPROPERTY(EditAnywhere, Category="Debugging|Attack")
	float SpecialAttackInterval = 99999.0f;
	UPROPERTY(VisibleInstanceOnly, Category="Debugging|Attack")
	float CurrentAttackTime = 0.0f;
	virtual void StartAttack();
	virtual void TickAttack();
	virtual void EndAttack();
	virtual void TriggerNormalAttack();
	virtual void TriggerSpecialAttack();
	
	/*
	 *Dead
	 */
	virtual void StartDead();
	virtual void TickDead();
	virtual void EndDead();

	/*
	 * Owner 측에서 상태 관련 처리만 위임
	 */
	virtual void HandleShove(const FVector& FromLocation);
	virtual void HandleDamage();
	virtual void HandleDie();
};
