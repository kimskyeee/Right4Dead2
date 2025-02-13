// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "FWeaponData.h" 
#include "Survivor.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//외관
	Root=CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	SetRootComponent(Root);
	PrimaryWeapon=CreateDefaultSubobject<UStaticMeshComponent>("PrimaryWeapon");
	PrimaryWeapon->SetupAttachment(RootComponent);
	PrimaryWeapon->CastShadow = false;
	Muzzle=CreateDefaultSubobject<USceneComponent>("Muzzle");
	Muzzle->SetupAttachment(PrimaryWeapon);
	Muzzle->SetRelativeLocationAndRotation(FVector(0,57,-2.5),FRotator(0,90,0));
	Eject=CreateDefaultSubobject<USceneComponent>("Eject");
	Eject->SetupAttachment(Muzzle);
	Eject->SetRelativeLocation(FVector(-50,0,0));

	Root->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	//Root->SetCollisionProfileName(TEXT("BlockAll"));
	
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PrimaryActorTick.bCanEverTick = false;
	IsEquipped = false;
}


//공격함수 추가
void AWeaponBase::LineTrace(FVector MuzzleLocation, FVector ImpactPoint,FRotator ProjectileRotation)
{
	ASurvivor* target = Cast<ASurvivor>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	
	FHitResult HitResult;
	FVector SpreadStart;
	FVector CameraFVector;
	FCollisionQueryParams Params;
	//나를 무시하는 조건을 추가해주기
	Params.AddIgnoredActor(this);
	
	if (target)
	{
		SpreadStart = target->FirstCameraComp->GetComponentLocation();
		CameraFVector = target->FirstCameraComp->GetForwardVector();
	}

	float End = UKismetMathLibrary::RandomFloatInRange(BulletSpread*(-1),BulletSpread);
	FVector SpreadEnd = (CameraFVector*20000) + FVector(End,End,End);
	
	GetWorld()->LineTraceSingleByChannel(HitResult,SpreadStart,SpreadEnd,ECC_Visibility,Params);

	FVector MuzzleEnd = HitResult.ImpactPoint;
	FVector MuzzleStart = Muzzle->GetComponentLocation();

	GetWorld()->LineTraceSingleByChannel(HitResult,MuzzleStart,MuzzleEnd,ECC_Visibility,Params);

	MuzzleLocation = MuzzleStart;
	ImpactPoint = HitResult.ImpactPoint;
	ProjectileRotation = UKismetMathLibrary::FindLookAtRotation(HitResult.TraceStart, HitResult.TraceEnd);

}

void AWeaponBase::SetEquipped(bool bEquip)
{
	IsEquipped = bEquip;

	//월드에 있는 무기는 bIsEquipped를 false로 설정
	//무기를 장착하면 true로 설정을 변경해주자
}

void AWeaponBase::OnReload_Implementation()
{
}

void AWeaponBase::OnFire_Implementation()
{
}




