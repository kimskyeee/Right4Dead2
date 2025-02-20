#include "ZombieBase.h"

#include "R4DHelper.h"
#include "ShoveDamageType.h"
#include "Engine/DamageEvents.h"
#include "Engine/StaticMeshActor.h"
#include "Right4Dead/Right4Dead.h"

AZombieBase::AZombieBase()
{
	PrimaryActorTick.bCanEverTick = true;
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

void AZombieBase::BeginPlay()
{
	Super::BeginPlay();
	InitDifficulty();
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
	else
	{
		// 죽었고, 죽은 원인이 PointDamage일 때만 해당 부위 사지분해 실시
		if (const FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)&DamageEvent)
		{
			HandleDismemberment(PointDamageEvent);
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

void SpawnPartMesh(USkeletalMeshComponent* SkeletalMesh, FName BoneName, UStaticMesh* SpawnMesh, FVector ImpulseDirection, float Power)
{
	// 이미 Bone이 숨겨진 상태라면 다시 스폰시키지 않는다
	if (SkeletalMesh->IsBoneHiddenByName(BoneName))
	{
		return;
	}
	// Bone을 숨겨라
	SkeletalMesh->HideBoneByName(BoneName, PBO_None);

	// Bone의 위치
	const FTransform Transform = SkeletalMesh->GetBoneTransform(BoneName);
	// 가짜 Static Mesh를 원래 Bone 위치에 스폰
	auto* StaticMeshActor = SkeletalMesh->GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Transform);
	// 이동 가능하게
	StaticMeshActor->SetMobility(EComponentMobility::Type::Movable);
	// 물리 시뮬레이트 활성화
	StaticMeshActor->GetStaticMeshComponent()->SetSimulatePhysics(true);
	StaticMeshActor->GetStaticMeshComponent()->SetStaticMesh(SpawnMesh);
	// 적절한 방향으로 적절한 세기로 날린다
	ImpulseDirection *= Power;
	StaticMeshActor->GetStaticMeshComponent()->AddImpulse(ImpulseDirection);
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

void AZombieBase::HandleShove(const FVector& FromLocation)
{
	PRINT_CALLINFO();
}

void AZombieBase::HandleDismemberment(const FPointDamageEvent* PointDamageEvent)
{
	PRINT_CALLINFO();
	const FName ParentBoneName = UR4DHelper::GetParentBone(GetMesh(), PointDamageEvent->HitInfo.BoneName);
	if (ParentBoneName == TEXT("neck_01"))
	{
		SpawnPartMesh(GetMesh(), TEXT("neck_01"), HeadMesh, FVector(1, 0, 0), 1000);
	}
	else if (ParentBoneName == TEXT("lowerarm_l"))
	{
		SpawnPartMesh(GetMesh(), TEXT("lowerarm_l"), ArmLeftMesh, FVector(1, 0, 0), 1000);
	}
	else if (ParentBoneName == TEXT("lowerarm_r"))
	{
		SpawnPartMesh(GetMesh(), TEXT("lowerarm_r"), ArmRightMesh, FVector(1, 0, 0), 1000);
	}
	else if (ParentBoneName == TEXT("thigh_l"))
	{
		SpawnPartMesh(GetMesh(), TEXT("thigh_l"), LegLeftMesh, FVector(1, 0, 0), 1000);
	}
	else if (ParentBoneName == TEXT("thigh_r"))
	{
		SpawnPartMesh(GetMesh(), TEXT("thigh_r"), LegRightMesh, FVector(1, 0, 0), 1000);
	}
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
