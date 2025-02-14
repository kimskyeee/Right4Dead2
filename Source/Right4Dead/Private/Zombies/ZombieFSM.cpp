#include "ZombieFSM.h"

#include "CommonZombie.h"
#include "Kismet/GameplayStatics.h"
#include "Right4Dead/Right4Dead.h"

UZombieFSM::UZombieFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UZombieFSM::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<ACommonZombie>(GetOwner());
	Movement = Owner->GetCharacterMovement();
}

void UZombieFSM::SetState(const EZombieState NewState)
{
	// 기존 상태에 따라 정리 작업
	switch (State)
	{
	case EZombieState::EZS_Idle:
		CurrentIdleTime = 0.0f;
		break;
	case EZombieState::EZS_Chase:
		CurrentChaseTime = 0.0f;
		break;
	case EZombieState::EZS_Attack:
		CurrentAttackTime = 0.0f;
		break;
	case EZombieState::EZS_Dead:
		break;
	}

	// 새로운 상태에 따른 초기화 작업
	switch (NewState)
	{
	case EZombieState::EZS_Idle:
		ChaseTarget = nullptr;
		CurrentIdleTime = 0.0f;
		break;
	case EZombieState::EZS_Chase:
		CurrentChaseTime = 0.0f;
		break;
	case EZombieState::EZS_Attack:
		CurrentAttackTime = 0.0f;
		break;
	case EZombieState::EZS_Dead:
		break;
	}
	Distance = 0.0f;
	State = NewState;
}

void UZombieFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (nullptr == Owner)
	{
		return;
	}
	
	if (ChaseTarget)
	{
		Distance = FVector::Dist(Owner->GetActorLocation(), ChaseTarget->GetActorLocation());	
	}
	
	switch (State)
	{
	case EZombieState::EZS_Idle:
		TickIdle();
		break;
	case EZombieState::EZS_Chase:
		TickChase();
		break;
	case EZombieState::EZS_Attack:
		TickAttack();
		break;
	case EZombieState::EZS_Dead:
		TickDead();
		break;
	}
}

void UZombieFSM::TickIdle()
{
	if (ChaseTarget)
	{
		SetState(EZombieState::EZS_Chase);
		return;
	}
	
	CurrentIdleTime += GetWorld()->GetDeltaSeconds();
	if (CurrentIdleTime > SearchInterval)
	{
		PRINT_CALLINFO();
		DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), Awareness, 16, FColor::Red, false, 0.5f);
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3)); // Player 타입 객체 검사
		TArray<AActor*> ActorsToIgnore;
        ActorsToIgnore.Add(Owner); // 자기 자신은 검사에서 제외
		TArray<AActor*> OverlappingActors;
		bool bHit = UKismetSystemLibrary::SphereOverlapActors(
			GetWorld(),
			Owner->GetActorLocation(),
			Awareness,
			ObjectTypes,
			UActorBase::StaticClass(),
			ActorsToIgnore,
			OverlappingActors
		);
		if (bHit)
		{
			ChaseTarget = OverlappingActors[0];
			State = EZombieState::EZS_Chase;
		}
		CurrentIdleTime = 0;
		return;
	}
}

void UZombieFSM::TickChase()
{
	if (nullptr == ChaseTarget)
	{
		SetState(EZombieState::EZS_Idle);
		return;
	}
	
	CurrentChaseTime += GetWorld()->GetDeltaSeconds();
	if (CurrentChaseTime > StopChaseTime)
	{
		ChaseTarget = nullptr;
		SetState(EZombieState::EZS_Idle);
		return;
	}

	// 공격 범위 내에 있으면 공격한다.
	if (Distance < AttackRange)
	{
		SetState(EZombieState::EZS_Attack);
		return;
	}
	else
	{
		
	}
	
	// 타겟이 인지 거리 내에 있으면 추적 지속 시간을 초기화 한다.
	if (Distance < Awareness)
	{
		CurrentChaseTime = 0.0f;
	}
}

void UZombieFSM::TickAttack()
{
	if (nullptr == ChaseTarget)
	{
		SetState(EZombieState::EZS_Idle);
		return;
	}

	// 공격 범위 안에 타겟이 없으면 추격 상태로 전환한다.
	if (Distance > AttackRange)
	{
		SetState(EZombieState::EZS_Chase);
		return;
	}
	
	CurrentAttackTime += GetWorld()->GetDeltaSeconds();
	if (CurrentAttackTime > AttackInterval)
	{
		Owner->TriggerAttack();
		CurrentAttackTime = 0.0f;
		return;
	}
}

void UZombieFSM::TickDead()
{
}