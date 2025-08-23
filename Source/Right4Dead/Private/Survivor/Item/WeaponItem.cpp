// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/WeaponItem.h"

#include "Components/SphereComponent.h"


// Sets default values
AWeaponItem::AWeaponItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//외관
	Root = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	SetRootComponent(Root);
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
	WeaponMesh->SetupAttachment(RootComponent);
	WeaponMesh->CastShadow = false;
	Muzzle=CreateDefaultSubobject<USceneComponent>("Muzzle");
	Muzzle->SetupAttachment(WeaponMesh);
	Muzzle->SetRelativeLocationAndRotation(FVector(0,57,-2.5),FRotator(0,90,0));

	//충돌체 설정
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Root->SetGenerateOverlapEvents(true);
	Root->SetCanEverAffectNavigation(false);
}

// Called when the game starts or when spawned
void AWeaponItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponItem::SetOverlayMaterial()
{
	WeaponMesh->SetOverlayMaterial(OverlayMaterial);
}

