// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemBase.h"

#include "CommonZombie.h"
#include "ShoveDamageType.h"
#include "Survivor.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"


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

void AItemBase::OnEquipped(class ASurvivor* OwnerChar)
{
	CachedOwner = OwnerChar;
	CachedAnim = (OwnerChar && OwnerChar->GetMesh()) ? OwnerChar->GetMesh()->GetAnimInstance() : nullptr;

	if (Montage_Equip)
	{
		PlayItemMontage(Montage_Equip);
	}
}

void AItemBase::OnUnequipped()
{
	CachedAnim = nullptr;
	CachedOwner = nullptr;
}

void AItemBase::RightAttack()
{
	SpawnShoveCylinder();
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
	ShoveCollisionCylinder->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ShoveCollisionCylinder->SetGenerateOverlapEvents(true);
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


