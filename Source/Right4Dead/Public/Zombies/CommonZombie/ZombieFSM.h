#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ZombieFSM.generated.h"

class AAIController;
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

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RIGHT4DEAD_API UZombieFSM : public UActorComponent
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
	EZombieState State = EZombieState::EZS_Idle;
	UPROPERTY(EditAnywhere, Category="Debugging")
	TObjectPtr<AActor> ChaseTarget = nullptr;
	
	// Idle 상태 지속 시간
	UPROPERTY(VisibleInstanceOnly, Category="Debugging|Idle")
	float CurrentIdleTime = 0.0f;
	// 타겟 탐색 간격
	UPROPERTY(EditAnywhere, Category="Debugging|Idle")
	float SearchInterval = 1.0f;
	UPROPERTY(EditAnywhere, Category="Debugging|Idle")
	bool bEnableSearch = true;

	// 추격 관련 디버깅 여부
	UPROPERTY(EditInstanceOnly, Category="Debugging|Chase")
	bool bVerboseChase = false;
	// 인식 거리
	UPROPERTY(EditAnywhere, Category="Debugging|Chase")
	float Awareness = 1500.0f;
	// 추격 대상과의 거리
	UPROPERTY(VisibleInstanceOnly, Category="Debugging|Chase")
	float Distance = 0.0f;
	// 추격 지속 시간
	UPROPERTY(VisibleInstanceOnly, Category="Debugging|Chase")
	float CurrentChaseTime = 0.0f;
	// 추격 중단 시간
	UPROPERTY(EditAnywhere, Category="Debugging|Chase")
	float StopChaseTime = 20.0f;

	// 공격 범위
	UPROPERTY(EditAnywhere, Category="Debugging|Attack")
	float AttackRange = 150.0f;
	// 공격 지속 시간
	UPROPERTY(VisibleInstanceOnly, Category="Debugging|Attack")
	float CurrentAttackTime = 0.0f;
	// 공격 간격
	UPROPERTY(EditAnywhere, Category="Debugging|Attack")
	float AttackInterval = 2.0f;

	void SetState(const EZombieState NewState);
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	void TickIdle();
	void TickChase();
	void TickAttack();
	void TickDead();

	void HandleShove(const FVector& FromLocation);
	void HandleDamage();
	void HandleDie();
};