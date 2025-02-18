#include "ZombieFSM.h"

#include "CommonZombie.h"
#include "Survivor.h"
#include "ZombieAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	
	// ZombieAIController와 ZombieAnimInstance는 Owner에서 주입
	// ZombieAI = Cast<AZombieAIController>(Owner->GetController());
	// ZombieAnimInstance = (Owner->ZombieAnimInstance);
	
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
		CurrentAttackTime = AttackInterval;
		break;
	case EZombieState::EZS_Dead:
		ChaseTarget = nullptr;
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
		TArray<AActor*> ActorsToIgnore;
        ActorsToIgnore.Add(Owner); // 자기 자신은 검사에서 제외
		TArray<FHitResult> OutHits;
		const bool bHit = UKismetSystemLibrary::SphereTraceMulti(
			GetWorld(),
			Owner->GetActorLocation(),
			Owner->GetActorLocation(),
			Awareness,
			UEngineTypes::ConvertToTraceType(ECC_Pawn),
			false,
			ActorsToIgnore,
			(bVerboseChase) ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
			OutHits,
			true
		);
		if (bHit && OutHits.Num() > 0)
		{
			for (auto HitResult : OutHits)
			{
				// TODO: Collision Channel이 정리되면 PipeBomb, 담즙 등에도 반응하도록 수정
				if (ASurvivor* Survivor = Cast<ASurvivor>(HitResult.GetActor()))
				{
					ChaseTarget = Survivor;
				}
			}
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
		if (ZombieAI)
		{
			if (ZombieAI->GetMoveStatus() == EPathFollowingStatus::Type::Idle)
			{
				ZombieAI->MoveToActor(ChaseTarget);
			}
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

void UZombieFSM::HandleShove(const FVector& FromLocation)
{
	if (ZombieAnimInstance)
	{
		const FVector Location = Owner->GetActorLocation();
		const FVector ForwardVector = Owner->GetActorForwardVector();
		// 상대 액터의 Z축 좌표는 무시한다.
		const FVector FromLocationWithoutZ = FVector(FromLocation.X, FromLocation.Y, Location.Z);
		// 상대 액터가 어느 방향(상대 기준)으로 밀쳤는지 방향 벡터를 구한다.
		const FVector DirVector = (FromLocationWithoutZ - Location).GetSafeNormal();
		// 내적으로 위아래를 구분하고
		double Theta = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ForwardVector, DirVector)));
		// 외적으로 좌우를 구분
		const FVector CrossProduct = FVector::CrossProduct(ForwardVector, DirVector);
		if (CrossProduct.Z < 0)
		{
			Theta *= -1.f;
		}
		// AnimBlueprint에서 Theta값에 따른 밀치기 피격 애니메이션을 재생하자.
		ZombieAnimInstance->PlayKnockBack(Theta);
	}

	// 상태 관련 처리
}

void UZombieFSM::HandleDamage()
{
	// TODO: 방향, 종류에 따라 맞는 애니메이션 분기
	// ZombieAnimInstance->PlayDamage();
}

void UZombieFSM::HandleDie()
{
	// TODO: 방향에 따라 죽는 애니메이션 분기
	// ZombieAnimInstance->PlayDie();
	SetState(EZombieState::EZS_Dead);
}
