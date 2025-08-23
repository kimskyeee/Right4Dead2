// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ThrownItem.h"

#include "CommonZombie.h"
#include "ExplosionDamageType.h"
#include "Survivor.h"
#include "Camera/CameraComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "Particles/ParticleSystemComponent.h"


class ACommonZombie;
// Sets default values
AThrownItem::AThrownItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	TrailParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail"));
	TrailParticle->SetupAttachment(StaticMesh);

	BeepLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("BeepLight"));
	BeepLight->SetupAttachment(StaticMesh);
	
	//파이프폭탄 사운드
	const ConstructorHelpers::FObjectFinder<USoundWave>TempPipeBombBeep(TEXT("/Script/Engine.SoundWave'/Game/Assets/Sounds/WeaponNAttack/PipeBomb/pipebomb_beep.pipebomb_beep'"));
	if (TempPipeBombBeep.Succeeded())
	{
		PipeBombBeep = TempPipeBombBeep.Object;
	}
	const ConstructorHelpers::FObjectFinder<USoundWave>TempPipeBombEnd(TEXT("/Script/Engine.SoundWave'/Game/Assets/Sounds/WeaponNAttack/PipeBomb/Pipebomb_explode3.Pipebomb_explode3'"));
	if (TempPipeBombEnd.Succeeded())
	{
		PipeBombEnd = TempPipeBombEnd.Object;
	}

	//폭발 이펙트
	ConstructorHelpers::FObjectFinder<UParticleSystem>TempExplosion(TEXT("/Script/Engine.ParticleSystem'/Game/Assets/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (TempExplosion.Succeeded())
	{
		ExplosionEffect = TempExplosion.Object;
	}
}

// Called when the game starts or when spawned
void AThrownItem::BeginPlay()
{
	Super::BeginPlay();

	BeepLight->SetVisibility(false);
}

// Called every frame
void AThrownItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 던져지고 나서
	if (bIsThrown==true)
	{
		PlayBeepSound(DeltaTime);
	}
}

void AThrownItem::HandleUse(EUsingType Phase, float ElapsedHold)
{
	if (Phase == EUsingType::Started)
	{
		bArmed = true;
		ThrownAttack();
	}
	else if (Phase == EUsingType::Completed)
	{
		// 던지고 나면 없애기
		bArmed = false;
		OnConsumed.Broadcast(this);
		Destroy();
	}
}

void AThrownItem::ThrownAttack()
{
	//투척
	//몽타주 특정시점(추가필요)에서 무기해제 (던지고 나서도 손에 들고있으면 안됨)
	Char->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	bIsThrown = true;
	
	//몽타주 플레이
	// if (me->CurrentWeapon->WeaponData.WeaponFireMontage)
	{
		// me->Arms->GetAnimInstance()->Montage_Play(me->CurrentWeapon->WeaponData.WeaponFireMontage);
	}
	if (bIsThrown)
	{
		// me->CurrentWeapon = nullptr;
		ThrowWeapon();
	}
}

void AThrownItem::ThrowWeapon()
{
	TrailParticle->BeginTrails(TEXT("Socket"), TEXT("Socket0"), ETrailWidthMode_FromFirst, 1);
	TrailParticle->BeginTrails(TEXT("Socket1"), TEXT("Socket2"), ETrailWidthMode_FromFirst, 1);

	FVector StartLocation = Char->GetActorLocation() + FVector(0, 0, 80);

	// 카메라 방향 반영
	FVector CameraForwardVector = Char->FirstCameraComp->GetForwardVector();  // 카메라의 Forward Vector 사용
	FVector TargetLocation = StartLocation + CameraForwardVector * 1500; // 카메라 방향으로 목표 위치 설정
	
	//0~1사이 (포물선 궤적 높이라고 이해하자)
	float ArcValue = 0.75f;
	FVector OutVelocity = FVector::ZeroVector;
	if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, OutVelocity, StartLocation, TargetLocation, GetWorld()->GetGravityZ(),ArcValue))
	{
		FPredictProjectilePathParams PredictParams (20.0f, StartLocation, OutVelocity, 15.0f);
		//->20은 트레이싱이 보여질 프로젝타일 크기 /  15는 시뮬레이션 되는 Max 시간
		
		PredictParams.OverrideGravityZ=GetWorld()->GetGravityZ();
		FPredictProjectilePathResult PredictResult;
		UGameplayStatics::PredictProjectilePath(this, PredictParams, PredictResult);

		//이제 날려보내자
		RootSphere->SetSimulatePhysics(true);
		RootSphere->SetPhysicsLinearVelocity(OutVelocity);
		//근데 바닥에 닿으면 멈춰야함!
		RootSphere->OnComponentHit.AddDynamic(this, &AThrownItem::OnThrowWeaponHit);
		RootSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		RootSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
		RootSphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		RootSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
		RootSphere->SetNotifyRigidBodyCollision(true); //충돌이벤트 발생가능하게 설정
	}
}

void AThrownItem::OnThrowWeaponHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//이미 바닥이면 실행하지말자
	if (bHasLanded) return;

	//바닥에 닿으면 속도를 0으로 만들자
	bHasLanded = true;
	BeepLight->SetVisibility(true);
	
	//속도 멈추기
	RootSphere->SetPhysicsLinearVelocity(FVector::ZeroVector);
	RootSphere->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

	//마찰력을 증가시키자
	RootSphere->SetLinearDamping(Damping); //이속 감소
	RootSphere->SetAngularDamping(Damping); //회전 감소

	//6초동안 sphere trace로 좀비 감지하기 (범위 3000이상)
	//1초마다 loop
	GetWorld()->GetTimerManager().SetTimer(PipeBombTraceTimerHandle, this, &AThrownItem::PipeBombTraceZombies, TraceRate, true);

	//6초후 폭발
	GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &AThrownItem::ExplodeWeapon, BombTimerRate, false);
}

void AThrownItem::PipeBombTraceZombies()
{
	//Sphere Trace 실행
	FVector TraceStart = GetActorLocation();
	FVector TraceEnd = TraceStart;
	float TraceRadius = 3000.0f; //감지 범위

	TArray<FHitResult> HitResults;
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(TraceRadius);

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults, TraceStart, TraceEnd,
		FQuat::Identity, ECC_GameTraceChannel6, SphereShape
	);

	//감지된 액터들 중 ACommonZombie 타입인지 확인하고 이벤트 호출
	for (const FHitResult& HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor && HitActor->IsA(ACommonZombie::StaticClass()))
		{
			ACommonZombie* Zombie = Cast<ACommonZombie>(HitActor);
			if (Zombie)
			{
				Zombie->HandlePipeBombBeep(this);
			}
		}
	}
}

void AThrownItem::ExplodeWeapon()
{
	//좀비 감지 타이머 중지
	GetWorld()->GetTimerManager().ClearTimer(PipeBombTraceTimerHandle);
	GetWorldTimerManager().ClearTimer(BeepSoundTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ExplosionTimerHandle);
	
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	//데미지 적용
	UGameplayStatics::ApplyRadialDamage(
		this,BombDamage,
		GetActorLocation(),
		DamageRadius,
		UExplosionDamageType::StaticClass(),
		IgnoreActors,
		this,
		GetWorld()->GetFirstPlayerController(),
		true,
		ECC_Visibility);

	
	UGameplayStatics::PlaySound2D(this, PipeBombEnd, 1, 1);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());

	Destroy();
}
