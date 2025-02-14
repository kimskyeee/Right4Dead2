#include "ZombieFSM.h"

#include "CommonZombie.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
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
	// ZombieAIController는 Owner에서 주입
	// ZombieAI = Cast<AZombieAIController>(Owner->GetController());
	// Verbose 세팅은 인스턴스에서만 껏다 켯다 할 수 있도록 함
	bVerboseChase = false;
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
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3)); // Player 타입 객체 검사
		TArray<AActor*> ActorsToIgnore;
        ActorsToIgnore.Add(Owner); // 자기 자신은 검사에서 제외
		TArray<FHitResult> OutHits;
		const bool bHit = UKismetSystemLibrary::SphereTraceMulti(
			GetWorld(),
			Owner->GetActorLocation(),
			Owner->GetActorLocation(),
			Awareness,
			UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4),
			false,
			ActorsToIgnore,
			(bVerboseChase) ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
			OutHits,
			true
		);
		if (bHit && OutHits.Num() > 0)
		{
			ChaseTarget = OutHits[0].GetActor();
			State = EZombieState::EZS_Chase;
			ZombieAI->MoveToActor(ChaseTarget);
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
		ZombieAI->StopMovement();
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
		if (ZombieAI->GetMoveStatus() == EPathFollowingStatus::Type::Idle)
		{
			ZombieAI->MoveToActor(ChaseTarget);
		}
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