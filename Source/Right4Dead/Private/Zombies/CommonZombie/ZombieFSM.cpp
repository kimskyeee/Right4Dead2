#include "ZombieFSM.h"

#include "CommonZombie.h"
#include "Survivor.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"

UZombieFSM::UZombieFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}

/*
 * Protected
*/
void UZombieFSM::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<ACommonZombie>(GetOwner());
	Movement = Owner->GetCharacterMovement();
	AnimInstance = Owner->GetAnimInstance();
	AIController = Owner->GetAIController();
	if (Owner == nullptr) UE_LOG(LogTemp, Warning, TEXT("Owner nullptr"));
	if (Movement == nullptr) UE_LOG(LogTemp, Warning, TEXT("Movement nullptr"));
	if (AnimInstance == nullptr) UE_LOG(LogTemp, Warning, TEXT("AnimInstance nullptr"));
	if (AIController == nullptr) UE_LOG(LogTemp, Warning, TEXT("AIController nullptr"));
	
	// Verbose 세팅은 인스턴스에서만 껏다 켯다 할 수 있도록 함
	bVerboseSearch = false;
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
void UZombieFSM::SetState(const EZombieState NewState)
{
	// 기존 상태에 따라 정리 작업
	switch (State)
	{
	case EZombieState::EZS_Idle:
		EndIdle();
		break;
	case EZombieState::EZS_Chase:
		EndChase();
		break;
	case EZombieState::EZS_Attack:
		EndAttack();
		break;
	case EZombieState::EZS_Dead:
		EndDead();
		break;
	}

	// 새로운 상태에 따른 초기화 작업
	switch (NewState)
	{
	case EZombieState::EZS_Idle:
		StartIdle();
		break;
	case EZombieState::EZS_Chase:
		StartChase();
		break;
	case EZombieState::EZS_Attack:
		StartAttack();
		break;
	case EZombieState::EZS_Dead:
		StartDead();
		break;
	}
	Distance = 0.0f;
	State = NewState;
}

#pragma region Idle
void UZombieFSM::StartIdle()
{
	ChaseTarget = nullptr;
	CurrentIdleTime = 0.0f;
	AIController->StopMovement();
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
	CurrentChaseTime = 0.0f;
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
		if (AIController)
		{
			if (AIController->GetMoveStatus() == EPathFollowingStatus::Type::Idle)
			{
				AIController->MoveToActor(ChaseTarget);
			}
		}
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
	CurrentAttackTime = AttackInterval;
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
void UZombieFSM::EndAttack()
{
	CurrentAttackTime = 0.0f;
}
#pragma endregion Attack

#pragma region Dead
void UZombieFSM::StartDead()
{
	ChaseTarget = nullptr;
	CurrentDeadTime = 0.0f;
	AIController->StopMovement();
}
void UZombieFSM::TickDead()
{
}
void UZombieFSM::EndDead()
{
}
#pragma endregion Dead

/*
 * Public
*/
void UZombieFSM::Init()
{
	Owner = Cast<AZombieBase>(GetOwner());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

AActor* UZombieFSM::GetChaseTarget() const
{
	return ChaseTarget;
}

EZombieState UZombieFSM::GetState() const
{
	return State;
}

void UZombieFSM::SetAIController(AAIController* InAIController)
{
	AIController = InAIController;
}
void UZombieFSM::HandleShove(const FVector& FromLocation)
{
	if (AnimInstance)
	{
		
		const FVector LocationA = Owner->GetActorLocation();
		const FVector ForwardVectorA = Owner->GetActorForwardVector();
		
		// 상대 액터의 Z축 좌표는 좀비와 동일하게 맞춘다.
		const FVector LocationB =  FVector(FromLocation.X, FromLocation.Y, LocationA.Z);
		
		// 플레이어와 좀비의 위치 벡터를 뺄셈 계산하여 플레이어 기준에서 좀비쪽으로 가리키는 방향 벡터를 구한다.
		const FVector DirVector = (LocationB - LocationA).GetSafeNormal();
		
		// 내적으로 위아래를 구분할 수 있게 하고
		double Theta = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ForwardVectorA, DirVector)));
		
		// 외적으로 좌우를 구분할 수 있게 한다. (Z값이 0 미만이면 Theta의 부호를 반대로 하기만 하면 된다)
		const FVector CrossProduct = FVector::CrossProduct(ForwardVectorA, DirVector);
		if (CrossProduct.Z < 0)
		{
			Theta *= -1.f;
		}
		// AnimBlueprint에서 Theta값에 따른 밀치기 피격 애니메이션을 재생하자.
		// -45 ~ 45 : 좀비가 정면에서 밀치기를 맞았다 (뒤로 밀리는 애니메이션)
		//  45 ~ 135 : 좀비가 오른쪽에서 밀치기를 맞았다 (왼쪽으로 밀리는 애니메이션)
		// -180 ~ -135 / 135 ~ 180 : 좀비가 뒤에서 밀치기를 맞았다 (앞으로 밀리는 애니메이션)
		// -135 ~ -45 : 좀비가 왼쪽에서 밀치기를 맞았다 (오른쪽으로 밀리는 애니메이션)
		// 추후 C++에서 분기를 나눌 예정
		// TODO:
		// CommonZombieAnimInstance->PlayKnockBack(Theta);
	}

	// 상태 관련 처리
}
void UZombieFSM::HandleDamage()
{
	// TODO: 방향, 종류에 따라 맞는 애니메이션 분기
	// CommonZombieAnimInstance->PlayDamage();
}
void UZombieFSM::HandleDie()
{
	// TODO: 방향에 따라 죽는 애니메이션 분기
	// CommonZombieAnimInstance->PlayDie();
	SetState(EZombieState::EZS_Dead);
}

void UZombieFSM::SetChaseTarget(class AActor* Actor)
{
	ChaseTarget = Actor;
}
