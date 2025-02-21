#pragma once

#include "CoreMinimal.h"
#include "ZombieAIController.h"
#include "Components/ActorComponent.h"
#include "ZombieFSM.generated.h"

class AZombieBase;
class UZombieAnimInstance;
class UCharacterMovementComponent;
class ACommonZombie;

UENUM(BlueprintType)
enum class EZombieState : uint8
{
	EZS_Idle,
	EZS_Chase,
	EZS_Attack,
	EZS_Dead
};

UCLASS(Abstract)
class RIGHT4DEAD_API UZombieFSM : public UActorComponent
{
	GENERATED_BODY()

public:
	UZombieFSM();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;
	void SetState(const EZombieState NewState);

	/*
	 * Owner References
	*/
	UPROPERTY()
	TObjectPtr<AZombieBase> Owner = nullptr;
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> Movement = nullptr;
	UPROPERTY()
	TObjectPtr<AAIController> AIController = nullptr;
	UPROPERTY()
	TObjectPtr<UAnimInstance> AnimInstance = nullptr;

	/*
	 * Data
	*/
	// 인식 거리
	UPROPERTY(EditAnywhere, Category="Debugging|Data")
	float Awareness = 1500.0f;

	/*
	 * Variables
	*/
	UPROPERTY(EditAnywhere, Category="Debugging")
	EZombieState State = EZombieState::EZS_Idle;
	UPROPERTY(EditAnywhere, Category="Debugging")
	TObjectPtr<AActor> ChaseTarget = nullptr;
	// 추격 대상과의 거리
    UPROPERTY(VisibleInstanceOnly, Category="Debugging")
    float Distance = 0.0f;

	/*
	 * State : Idle
	*/
	UPROPERTY(EditInstanceOnly, Category="Debugging|Idle")
	bool bVerboseSearch = false;
	UPROPERTY(EditAnywhere, Category="Debugging|Idle")
	bool bEnableSearch = true;
	UPROPERTY(VisibleInstanceOnly, Category="Debugging|Idle")
	float CurrentIdleTime = 0.0f;
	UPROPERTY(EditAnywhere, Category="Debugging|Idle")
	float SearchInterval = 1.0f;
	virtual void StartIdle();
	virtual void TickIdle();
	virtual void EndIdle();

	/*
	 * State : Chase
	*/
	UPROPERTY(VisibleInstanceOnly, Category="Debugging|Chase")
	float CurrentChaseTime = 0.0f;
	UPROPERTY(EditAnywhere, Category="Debugging|Chase")
	float StopChaseTime = 20.0f;
	virtual void StartChase();
	virtual void TickChase();
	virtual void EndChase();

	/*
	 * State : Attack
	*/
	UPROPERTY(EditAnywhere, Category="Debugging|Attack")
	float AttackRange = 150.0f;
	UPROPERTY(VisibleInstanceOnly, Category="Debugging|Attack")
	float CurrentAttackTime = 0.0f;
	UPROPERTY(EditAnywhere, Category="Debugging|Attack")
	float AttackInterval = 2.0f;
	virtual void StartAttack();
	virtual void TickAttack();
	virtual void EndAttack();

	/*
	 * State : Dead
	*/
	UPROPERTY(VisibleInstanceOnly, Category="Debugging|Dead")
	float CurrentDeadTime = 0.0f;
	virtual void StartDead();
	virtual void TickDead();
	virtual void EndDead();

public:
	void Init();
	AActor* GetChaseTarget() const;
	EZombieState GetState() const;
	void SetAIController(AAIController* InAIController);
	virtual void HandleShove(const FVector& FromLocation);
	virtual void HandleDamage();
	virtual void HandleDie();
	void SetChaseTarget(class AActor* Actor);
};