#include "ZombieBase.h"

#include "R4DHelper.h"
#include "ShoveDamageType.h"
#include "ZombieFSM.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Right4Dead/Right4Dead.h"

void AZombieBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (AAIController* NewAIController = Cast<AAIController>(NewController))
	{
		AIController = NewAIController;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Possessed By Controller. but, not a AI Controller"));
	}
}

AZombieBase::AZombieBase()
{
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AAIController::StaticClass();
	FinalDamage = 0;
	Hp = 0;
	Speed = 0;
	TakeDamageMultiplier = 1;
	PartDamageMultipliers = {4, 1.25f, 0.75f, 1};
	bTakeDamaged = false;
	OnTakeAnyDamage.AddDynamic(this, &AZombieBase::OnTakeAnyDamageHandler);
	OnTakePointDamage.AddDynamic(this, &AZombieBase::OnTakePointDamageHandler);
	OnTakeRadialDamage.AddDynamic(this, &AZombieBase::OnTakeRadialDamageHandler);
}

UAnimInstance* AZombieBase::GetAnimInstance() const
{
	return GetMesh()->GetAnimInstance();
}

AAIController* AZombieBase::GetAIController() const
{
	return AIController;
}

AActor* AZombieBase::GetChaseTarget() const
{
	return GetZombieFsm()->GetChaseTarget();
}

void AZombieBase::BeginPlay()
{
	Super::BeginPlay();
	InitDifficulty();
	AnimInstance = GetMesh()->GetAnimInstance();
}

float AZombieBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                              class AController* EventInstigator, AActor* DamageCauser)
{
	PRINT_CALLINFO();
	
	// 언리얼에서 제공하는 Damage 프레임워크 함수 호출 스택
	// Apply(..)Damage -> TakeDamage -> OnTake(...)DamageHandler
	//								 -> OnTakeAnyDamageHandler
	
	// 1. Super::TakeDamage 호출 전 피해 배율을 적용한다.
	// 2. Point 또는 RadialDamage Handler에서 부위별 데미지 또는 거리별 데미지를 계산하고 FinalDamage에 저장한다.
	// 3. OnDamage를 실행시키고 FinalDamage를 반환한다.
	
	DamageAmount *= TakeDamageMultiplier;
	FinalDamage = DamageAmount;
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	// FinalDamage can set on damage handler
	OnDamaged(FinalDamage);

	if (Hp > 0)
	{
		if (DamageEvent.DamageTypeClass == UShoveDamageType::StaticClass())
		{
			HandleShove(DamageCauser->GetActorLocation());
		}
	}
	
	return FinalDamage;
}

void AZombieBase::OnTakeAnyDamageHandler(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	class AController* InstigatedBy, AActor* DamageCauser)
{
	PRINT_CALLINFO();
	// OnTakeAnyDamageHandler는 DamageEvent가 Point 또는 Radial이라도 마지막에 반드시 호출된다.
}

void AZombieBase::OnTakePointDamageHandler(AActor* DamagedActor, float Damage, class AController* InstigatedBy,
	FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
	const class UDamageType* DamageType, AActor* DamageCauser)
{
	const FName ParentBoneName = UR4DHelper::GetParentBone(GetMesh(), BoneName);
	if (ParentBoneName == TEXT("neck_01"))
	{
		Damage *= PartDamageMultipliers.Head;
	}
	else if (ParentBoneName == TEXT("lowerarm_l") || ParentBoneName == TEXT("lowerarm_r") || ParentBoneName == TEXT("thigh_l") || ParentBoneName == TEXT("thigh_r"))
	{
		Damage *= PartDamageMultipliers.Legs;
	}
	else if (ParentBoneName == TEXT("spine_02"))
	{
		Damage *= PartDamageMultipliers.Thorax;
	}
	else
	{
		Damage *= PartDamageMultipliers.Stomach;
	}
	FinalDamage = Damage;
}

void AZombieBase::OnTakeRadialDamageHandler(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	FVector Origin, const FHitResult& HitInfo, class AController* InstigatedBy, AActor* DamageCauser)
{
	PRINT_CALLINFO();
	FinalDamage = Damage;
}

UZombieFSM* AZombieBase::GetZombieFsm() const
{
	return ASDASDASD;
}

void AZombieBase::HandleShove(const FVector& FromLocation)
{
	PRINT_CALLINFO();
}


void AZombieBase::OnDamaged(const float Damage)
{
	const float OrgHp = Hp;
	Hp -= Damage;
	UE_LOG(LogTemp, Display, TEXT("HP %f -> %f"), OrgHp, Hp);
	if (Hp < 0)
	{
		OnDie();
	}
}

void AZombieBase::OnDie()
{
	PRINT_CALLINFO();
}

void AZombieBase::StartClimbing(const FTransform& Destination)
{
	AIController->GetPathFollowingComponent()->PauseMove();
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	bClimbing = true;
	ClimbDestination = Destination;
	SetActorRotation(Destination.Rotator());
}

void AZombieBase::EndClimbing()
{
	if (false == bClimbing)
	{
		return;
	}
	bClimbing = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	AIController->GetPathFollowingComponent()->ResumeMove();
	ClimbDestination = FTransform::Identity;
}

void AZombieBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (bClimbing)
	{
		const FVector P0 = GetActorLocation();
		const bool bIsNearZ = GetCharacterMovement()->GetFeetLocation().Z >= ClimbDestination.GetLocation().Z;
		if (false == bIsNearZ) // 아직 덜 올라왔다면
		{
			// Z축으로만 오른다
			const FVector P = P0 + FVector(0, 0, 1) * Speed * DeltaSeconds;
			SetActorLocation(P);
		}
		else
		{
			const FVector P = P0 + GetActorForwardVector() * Speed * DeltaSeconds;
			SetActorLocation(P);
		}
	}
}
