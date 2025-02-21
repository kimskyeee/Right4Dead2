#pragma once

#include "CoreMinimal.h"
#include "ZombieBaseFSM.h"
#include "Components/ActorComponent.h"
#include "ZombieFSM.generated.h"

class AAIController;
class UZombieAnimInstance;
class UCharacterMovementComponent;
class ACommonZombie;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RIGHT4DEAD_API UZombieFSM : public UZombieBaseFSM
{
	GENERATED_BODY()

public:
	UZombieFSM();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	TObjectPtr<ACommonZombie> Owner = nullptr;
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> Movement = nullptr;
	UPROPERTY()
	TObjectPtr<AAIController> ZombieAI = nullptr;
	UPROPERTY()
	TObjectPtr<UZombieAnimInstance> ZombieAnimInstance = nullptr;

	UPROPERTY(EditAnywhere, Category="Debugging")
	TObjectPtr<AActor> ChaseTarget = nullptr;
	
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
	virtual void StartIdle() override;
	virtual void TickIdle() override;
	virtual void EndIdle() override;

	/*
	 *Chase
	 */
	UPROPERTY(VisibleInstanceOnly, Category="Debugging|Chase")
	float Distance = 0.0f;
	UPROPERTY(VisibleInstanceOnly, Category="Debugging|Chase")
	float CurrentChaseTime = 0.0f;
	UPROPERTY(EditAnywhere, Category="Debugging|Chase")
	float StopChaseTime = 20.0f;
	virtual void StartChase() override;
	virtual void TickChase() override;
	virtual void EndChase() override;

	/*
	 *Attack
	 */
	UPROPERTY(EditAnywhere, Category="Debugging|Attack")
	float AttackRange = 150.0f;
	UPROPERTY(VisibleInstanceOnly, Category="Debugging|Attack")
	float CurrentAttackTime = 0.0f;
	UPROPERTY(EditAnywhere, Category="Debugging|Attack")
	float AttackInterval = 2.0f;
	virtual void StartAttack() override;
	virtual void TickAttack() override;
	virtual void EndAttack() override;

	/*
	 *Dead
	 */
	virtual void StartDead() override;
	virtual void TickDead() override;
	virtual void EndDead() override;

	void HandleShove(const FVector& FromLocation);
	void HandleDamage();
	void HandleDie();
};