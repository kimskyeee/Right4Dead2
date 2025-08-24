// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemBase.h"

#include "CommonZombie.h"
#include "ShoveDamageType.h"
#include "Survivor.h"
#include "SurvivorArmAnim.h"
#include "Camera/CameraComponent.h"
#include "Item/ItemSpec.h"
#include "Kismet/GameplayStatics.h"


class USurvivorArmAnim;
class ACommonZombie;
// Sets default values
AItemBase::AItemBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 실린더 메시 설정
	ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMeshAsset(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	if (CylinderMeshAsset.Succeeded())
	{
		CylinderMesh = CylinderMeshAsset.Object;
	}
	
	SetRootComponent(StaticMesh);
	StaticMesh->CastShadow = false;

	//충돌체 설정
	// StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMesh->bDisallowNanite = true;
	StaticMesh->SetSimulatePhysics(false);
	StaticMesh->SetGenerateOverlapEvents(true);
	StaticMesh->SetCollisionProfileName(TEXT("WorldWeapon"));
	StaticMesh->SetCanEverAffectNavigation(false);
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	Char = Cast<ASurvivor>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AItemBase::OnEquipped(ASurvivor* NewOwner, USceneComponent* Parent, FName Socket)
{
	CachedOwner = NewOwner;
	SetOwner(NewOwner);

	StopPhysicsAndZeroVelocity();
	StaticMesh->SetCollisionProfileName(TEXT("EquipWeapon"));
	
	// 캐시 세팅
	if (NewOwner && NewOwner->Arms)
	{
		if (UAnimInstance* Inst = NewOwner->Arms->GetAnimInstance())
		{
			CachedAnim = Cast<USurvivorArmAnim>(Inst);
		}
		else
		{
			CachedAnim = nullptr;
		}
	}
	else
	{
		CachedAnim = nullptr;
	}
	
	PlayMontageOnce(Montage_Equip);
	
	// 손 소켓에 부착
	const FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Parent, Rules, Socket);
	this->SetActorRelativeRotation(FRotator(0, 0, 0));
	this->SetActorRelativeLocation(FVector(0, 0, 0));

	SetWorldVisible(true);
}

void AItemBase::OnUnequipped()
{
	StopPhysicsAndZeroVelocity();
	
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	StaticMesh->SetCollisionProfileName(TEXT("SlotWeapon"));
	SetWorldVisible(false);
	
	CachedAnim = nullptr;
}

void AItemBase::OnPickedUp(ASurvivor* NewOwner)
{
	UE_LOG(LogTemp, Warning, TEXT("AItemBase::OnPickedUp"));
	
	CachedOwner = NewOwner;
	SetOwner(NewOwner);

	StopPhysicsAndZeroVelocity();
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AItemBase::OnDropped()
{
	// 월드 물리/충돌 활성화
	StaticMesh->SetCollisionProfileName(TEXT("WorldWeapon"));
	StaticMesh->SetSimulatePhysics(true);

	// 초기 속도 클리어
	StaticMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	StaticMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

	// 손 슬롯에서 분리
	SetWorldVisible(true);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	// 캐시 무효화
	CachedAnim = nullptr;
	CachedOwner = nullptr;
	
	SetOwner(nullptr);
}

void AItemBase::StopPhysicsAndZeroVelocity()
{
	StaticMesh->SetSimulatePhysics(false);
	StaticMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	StaticMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
}

void AItemBase::SetWorldVisible(bool bVisible)
{
	SetActorHiddenInGame(!bVisible);
}

void AItemBase::HandleUse(EUsingType Phase, float ElapsedHold)
{
	const float Threshold = (Spec ? Spec->HoldThresholdSec : 0.2f);
	
	UE_LOG(LogTemp, Warning, TEXT("Use %s  Elapsed=%.3f  Hold=%d"),
	*StaticEnum<EUsingType>()->GetDisplayNameTextByValue(static_cast<int64>(Phase)).ToString(), ElapsedHold, bHoldTriggered?1:0);
	
	switch (Phase)
	{
	case EUsingType::Started:
		bUseActive = true;
		bHoldTriggered = false;
		OnUseStart();
		break;

	case EUsingType::Ongoing:
		if (!bUseActive) break;
		if (!bHoldTriggered && ElapsedHold >= Threshold)
		{
			bHoldTriggered = true;
			OnHoldBegan(); // 여기서 HoldStart/Loop 시작
		}
		if (bHoldTriggered)
		{
			OnHoldTick(ElapsedHold); // 자동발사 등 처리
		}
		break;

	case EUsingType::Completed:
		if (!bUseActive) break;
		if (bHoldTriggered) OnHoldReleased(ElapsedHold);
		else OnTap(ElapsedHold);
		OnUseFinish(false);
		bUseActive = false;
		break;

	case EUsingType::Canceled:
		if (!bUseActive) break;
		StopMontageIfPlaying(Montage_Use_HoldLoop);
		OnUseFinish(true);
		bUseActive = false;
		break;
	}
}

void AItemBase::RightAttack()
{
	if (!CachedOwner.IsValid() || !CachedOwner->Arms) return;
	
	PlayMontageOnce(Montage_Shove, 1.0f);
}

void AItemBase::SpawnShoveCylinder()
{
	if (ShoveCollisionCylinder)
	{
		ShoveCollisionCylinder->DestroyComponent();
		ShoveCollisionCylinder = nullptr;
	}

	//Static Mesh가 정상적으로 로드되었는지 확인
	if (!CylinderMesh)
	{
		return;
	}

	//새로운 Collision Cylinder 생성
	ShoveCollisionCylinder = NewObject<UStaticMeshComponent>(this);
	if (!ShoveCollisionCylinder)
	{
		return;
	}

	ShoveCollisionCylinder->RegisterComponent();
	ShoveCollisionCylinder->SetGenerateOverlapEvents(true);
	
	ShoveCollisionCylinder->SetCollisionObjectType(ECC_WorldDynamic);
	ShoveCollisionCylinder->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ShoveCollisionCylinder->SetCollisionResponseToAllChannels(ECR_Ignore);
	ShoveCollisionCylinder->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	ShoveCollisionCylinder->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnShoveOverlap);
	ShoveCollisionCylinder->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	ShoveCollisionCylinder->SetStaticMesh(CylinderMesh);

	//위치 및 크기 설정
	FVector StartLocation = Char->FirstCameraComp->GetComponentLocation();
	FVector CylinderLocation = FVector(StartLocation.X, StartLocation.Y, StartLocation.Z-15);
	ShoveCollisionCylinder->SetWorldLocation(CylinderLocation);
	ShoveCollisionCylinder->SetWorldScale3D(FVector(3, 3, 0.2));
	
	FRotator CameraRotation = Char->FirstCameraComp->GetComponentRotation();
	ShoveCollisionCylinder->SetWorldRotation(CameraRotation);

	ShoveCollisionCylinder->SetVisibility(false);
	GetWorld()->GetTimerManager().SetTimer(CylinderTimerHandle, this, &AItemBase::DestroyShoveCylinder, 0.1f, false);
}

void AItemBase::OnShoveOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult)
{
    ACommonZombie* CommonZombie = Cast<ACommonZombie>(OtherActor);
	//조건1: 좀비가 맞았는지?
    if (CommonZombie)
    {
        //조건2: 좀비가 플레이어 전방 기준 좌우 45도 안에 있는가?
        FVector ZombieLocation = (CommonZombie->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        FVector SurvivorForwardVector = GetActorForwardVector();

    	//내적으로 각도 구하기
        float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(SurvivorForwardVector, ZombieLocation)));
        if (Angle <= 45.0f)
        {
            //피해 적용
            UGameplayStatics::ApplyDamage(CommonZombie, 10, Char->GetController(), this, UShoveDamageType::StaticClass());
        }
    }
}

void AItemBase::DestroyShoveCylinder()
{
	if (ShoveCollisionCylinder)
	{
		ShoveCollisionCylinder->DestroyComponent();
		ShoveCollisionCylinder = nullptr;
	}
}

class USurvivorArmAnim* AItemBase::GetArmAnim() const
{
	// 캐시가 유효하면 즉시 반환
	if (CachedAnim.IsValid()) return CachedAnim.Get();

	// 없으면 재조회
	if (!CachedOwner.IsValid() || !CachedOwner->Arms) return nullptr;
	
	if (UAnimInstance* Inst = CachedOwner->Arms->GetAnimInstance())
	{
		if (USurvivorArmAnim* Arm = Cast<USurvivorArmAnim>(Inst))
		{
			// const 함수 내부 캐시 갱신을 위해 mutable 사용
			CachedAnim = Arm;
			return Arm;
		}
	}
	return nullptr;
}

void AItemBase::PlayMontageOnce(UAnimMontage* Montage, float Rate)
{
	if (auto* Arm = GetArmAnim())
	{
		if (Montage && !Arm->Montage_IsPlaying(Montage))
		{
			Arm->Montage_Play(Montage, Rate);
		}
	}
}

void AItemBase::EnsureLoopMontage(UAnimMontage* LoopMontage)
{
	if (auto* Arm = GetArmAnim())
	{
		if (LoopMontage && !Arm->Montage_IsPlaying(LoopMontage))
		{
			Arm->Montage_Play(LoopMontage);
		}
	}
}

void AItemBase::StopMontageIfPlaying(UAnimMontage* Montage, float BlendOut)
{
	if (auto* Arm = GetArmAnim())
	{
		if (Montage && Arm->Montage_IsPlaying(Montage))
		{
			Arm->Montage_Stop(BlendOut, Montage);
		}
	}
}

void AItemBase::OnUseStart()
{
	PlayMontageOnce(Montage_Use_Tap);
}

void AItemBase::OnHoldBegan()
{
	PlayMontageOnce(Montage_Use_HoldStart);
	EnsureLoopMontage(Montage_Use_HoldLoop);
}

void AItemBase::OnHoldTick(float Elapsed)
{
	// 총기 오버라이드 필요
}

void AItemBase::OnHoldReleased(float Elapsed)
{
	StopMontageIfPlaying(Montage_Use_HoldLoop);
	PlayMontageOnce(Montage_Use_Release);
}

void AItemBase::OnTap(float Elapsed)
{
	PlayMontageOnce(Montage_Use_Tap);
}

void AItemBase::OnUseFinish(bool bCanceled)
{
}
