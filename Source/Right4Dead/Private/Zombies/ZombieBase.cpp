#include "ZombieBase.h"

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
	PRINT_CALLINFO();
	// if (bIsHead) Damage *= PartDamageMultipliers.Head;
	// if (bIsLegs) Damage *= PartDamageMultipliers.Legs;
	// if (bIsStomach) Damage *= PartDamageMultipliers.Stomach;
	// if (bIsThorax) Damage *= PartDamageMultipliers.Thorax;
	FinalDamage = Damage;
}

void AZombieBase::OnTakeRadialDamageHandler(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	FVector Origin, const FHitResult& HitInfo, class AController* InstigatedBy, AActor* DamageCauser)
{
	PRINT_CALLINFO();
	FinalDamage = Damage;
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
