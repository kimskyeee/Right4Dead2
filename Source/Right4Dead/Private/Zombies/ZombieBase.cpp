#include "ZombieBase.h"

#include "ShoveDamageType.h"
#include "Engine/DamageEvents.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
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

	if (DamageEvent.DamageTypeClass == UShoveDamageType::StaticClass())
	{
		HandleShove(DamageCauser->GetActorForwardVector());
	}
	
	return FinalDamage;
}

void AZombieBase::OnTakeAnyDamageHandler(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	class AController* InstigatedBy, AActor* DamageCauser)
{
	PRINT_CALLINFO();
	// OnTakeAnyDamageHandler는 DamageEvent가 Point 또는 Radial이라도 마지막에 반드시 호출된다.
}

bool IsChildBone(USkeletalMeshComponent* SkeletalMeshComp, const FName BoneName, const FName TargetBoneName)
{
	if (BoneName == TargetBoneName)
	{
		return true; // 현재 Bone이 목표 Bone과 동일한 경우
	}

	// 현재 Bone의 부모 Bone을 가져옴
	const FName ParentBoneName = SkeletalMeshComp->GetParentBone(BoneName);

	// 부모 Bone이 없으면 최상위 Bone이므로 false 반환
	if (ParentBoneName == NAME_None)
	{
		return false;
	}

	// 재귀적으로 부모 Bone을 탐색
	return IsChildBone(SkeletalMeshComp, ParentBoneName, TargetBoneName);
}

void SpawnPartMesh(USkeletalMeshComponent* SkeletalMesh, FName BoneName, UStaticMesh* SpawnMesh, FVector ImpulseDirection, float Power)
{
	SkeletalMesh->HideBoneByName(BoneName, PBO_None);
	const FTransform Transform = SkeletalMesh->GetBoneTransform(BoneName);
	auto* StaticMeshActor = SkeletalMesh->GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Transform);
	StaticMeshActor->SetMobility(EComponentMobility::Type::Movable);
	StaticMeshActor->GetStaticMeshComponent()->SetSimulatePhysics(true);
	StaticMeshActor->GetStaticMeshComponent()->SetStaticMesh(SpawnMesh);
	ImpulseDirection *= Power;
	StaticMeshActor->GetStaticMeshComponent()->AddImpulse(ImpulseDirection);
}

void AZombieBase::OnTakePointDamageHandler(AActor* DamagedActor, float Damage, class AController* InstigatedBy,
	FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
	const class UDamageType* DamageType, AActor* DamageCauser)
{
	const bool bIsHead = IsChildBone(GetMesh(), BoneName, TEXT("neck_01"));
	const bool bIsArmLeft = IsChildBone(GetMesh(), BoneName, TEXT("lowerarm_l"));
	const bool bIsArmRight = IsChildBone(GetMesh(), BoneName, TEXT("lowerarm_r"));
	const bool bIsLegLeft = IsChildBone(GetMesh(), BoneName, TEXT("thigh_l"));
	const bool bIsLegRight = IsChildBone(GetMesh(), BoneName, TEXT("thigh_r"));
	const bool bIsThorax = IsChildBone(GetMesh(), BoneName, TEXT("spine_02"));
	if (bIsHead)
	{
		Damage *= PartDamageMultipliers.Head;
		SpawnPartMesh(GetMesh(), TEXT("neck_01"), HeadMesh, FVector(1, 0, 0), 1000);
	}
	else if (bIsArmLeft)
	{
		Damage *= PartDamageMultipliers.Legs;
		SpawnPartMesh(GetMesh(), TEXT("lowerarm_l"), ArmLeftMesh, FVector(1, 0, 0), 1000);
	}
	else if (bIsArmRight)
	{
		Damage *= PartDamageMultipliers.Legs;
		SpawnPartMesh(GetMesh(), TEXT("lowerarm_r"), ArmRightMesh, FVector(1, 0, 0), 1000);
	}
	else if (bIsLegLeft)
	{
		Damage *= PartDamageMultipliers.Legs;
		SpawnPartMesh(GetMesh(), TEXT("thigh_l"), LegLeftMesh, FVector(1, 0, 0), 1000);
	}
	else if (bIsLegRight)
	{
		Damage *= PartDamageMultipliers.Legs;
		SpawnPartMesh(GetMesh(), TEXT("thigh_r"), LegRightMesh, FVector(1, 0, 0), 1000);
	}
	else if (bIsThorax)
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

void AZombieBase::OnDamaged(const float Damage)
{
	PRINT_CALLINFO();
	if ((Hp -= Damage) < 0)
	{
		OnDie();
	}
}

void AZombieBase::OnDie()
{
	PRINT_CALLINFO();
}
