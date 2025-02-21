#include "ZombieFSM.h"

#include "AIController.h"
#include "CommonZombie.h"
#include "Survivor.h"
#include "ZombieAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"

UZombieFSM::UZombieFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UZombieFSM::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<ACommonZombie>(GetOwner());
	
	// AIController와 ZombieAnimInstance는 Owner에서 주입
	// ZombieAI = Cast<AAIController>(Owner->GetController());
	// ZombieAnimInstance = (Owner->ZombieAnimInstance);
	
	// Verbose 세팅은 인스턴스에서만 껏다 켯다 할 수 있도록 함
	bVerboseSearch = false;
}
void UZombieFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

#pragma region Idle
void UZombieFSM::StartIdle()
{
	ChaseTarget = nullptr;
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
		CurrentIdleTime = 0;
		if (bEnableSearch)
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
				(bVerboseSearch) ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
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
		}
	}
}
void UZombieFSM::EndIdle()
{
	CurrentIdleTime = 0.0f;
}
#pragma endregion Idle

#pragma region Chase
void UZombieFSM::StartChase()
{
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
		TriggerStopChase();
		SetState(EZombieState::EZS_Idle);
		return;
	}

	// 공격 범위 내에 있으면 공격한다.
	if (Distance < NormalAttackRange)
	{
		SetState(EZombieState::EZS_Attack);
		return;
	}
	else
	{
		TriggerStartChase(ChaseTarget);
	}
	
	// 타겟이 인지 거리 내에 있으면 추적 지속 시간을 초기화 한다.
	if (Distance < Awareness)
	{
		CurrentChaseTime = 0.0f;
	}
}
void UZombieFSM::EndChase()
{
	CurrentChaseTime = 0.0f;
}
#pragma endregion Chase

#pragma region Attack
void UZombieFSM::StartAttack()
{
	CurrentAttackTime = NormalAttackInterval;
}
void UZombieFSM::TickAttack()
{
	if (nullptr == ChaseTarget)
	{
		SetState(EZombieState::EZS_Idle);
		return;
	}

	// 공격 범위 안에 타겟이 없으면 추격 상태로 전환한다.
	if (Distance > NormalAttackRange)
	{
		SetState(EZombieState::EZS_Chase);
		return;
	}
	
	CurrentAttackTime += GetWorld()->GetDeltaSeconds();
	if (CurrentAttackTime > NormalAttackInterval)
	{
		TriggerNormalAttack();
		CurrentAttackTime = 0.0f;
		return;
	}
}
void UZombieFSM::EndAttack()
{
	CurrentAttackTime = 0.0f;
}
#pragma endregion Attack

#pragma region Dead
void UZombieFSM::StartDead()
{
	ChaseTarget = nullptr;
}
void UZombieFSM::TickDead()
{
}
void UZombieFSM::EndDead()
{
}

#pragma endregion Dead

void UZombieFSM::HandleShove(const FVector& FromLocation)
{
	// 상태 관련 처리
}
void UZombieFSM::HandleDamage()
{
	// ..
}
void UZombieFSM::HandleDie()
{
	SetState(EZombieState::EZS_Dead);
}