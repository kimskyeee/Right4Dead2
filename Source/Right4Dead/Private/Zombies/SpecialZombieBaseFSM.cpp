#include "SpecialZombieBaseFSM.h"

#include "ZombieBase.h"
#include "CommonZombie.h"
#include "ZombieSpawnManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

USpecialZombieBaseFSM::USpecialZombieBaseFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
	bEnableIdleSearch = false;
}

void USpecialZombieBaseFSM::BeginPlay()
{
	Super::BeginPlay();
	ChaseTarget = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

void USpecialZombieBaseFSM::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	if (CurrentSpecialAttackTime < SpecialAttackInterval)
	{
		CurrentSpecialAttackTime += DeltaTime;
	}
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USpecialZombieBaseFSM::TickChase()
{
	Super::TickChase();
	
	CurrentChaseTime += GetWorld()->GetDeltaSeconds();
	
	if (CurrentChaseTime > SearchInterval)
	{
		CurrentChaseTime = 0.0f;

		if (Distance < SpecialAttackRange)
		{
			// 만약 특수 공격 사정거리 안에 방해물이 없다면...
			
        	TArray<AActor*> ActorsToIgnore;
        	ActorsToIgnore.Add(Owner);
			// TODO: Survivor의 Capsule 또는 SkeletalMesh를 별도의 CollisionPreset 사용하도록 설정
			const AZombieSpawnManager* ZombieSpawnManager = Cast<AZombieSpawnManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AZombieSpawnManager::StaticClass()));
			for (auto* CommonZombie : ZombieSpawnManager->ActiveZombies)
			{
				ActorsToIgnore.Add(CommonZombie);
			}
        	FHitResult HitResult;
        	const bool bHit = UKismetSystemLibrary::LineTraceSingle(
        		GetWorld(),
        		Owner->GetActorLocation(),
        		ChaseTarget->GetActorLocation(),
        		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel7),
        		false,
        		ActorsToIgnore,
        		EDrawDebugTrace::ForDuration,
        		HitResult,
        		true
        	);

			if (bHit)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), *HitResult.GetActor()->GetName());
				// TriggerStopChase();
				/*if (HitResult.Distance < SpecialAttackRange)
				{
					SetState(EZombieState::EZS_Attack);
				}*/
			}
		}
	}
	
	if (CurrentSpecialAttackTime >= SpecialAttackInterval)
	{
		
	}
}
