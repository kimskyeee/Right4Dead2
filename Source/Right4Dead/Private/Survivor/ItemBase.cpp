// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"

#include "Components/SphereComponent.h"


// Sets default values
AItemBase::AItemBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
	//외관
	Root=CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	SetRootComponent(Root);
	ItemMesh=CreateDefaultSubobject<UStaticMeshComponent>("ItemMesh");
	ItemMesh->SetupAttachment(RootComponent);

	//충돌체 설정
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Root->SetGenerateOverlapEvents(true);
	//SKYE: 무기 프리셋 변경3
	Root->SetCollisionProfileName(TEXT("WorldWeapon"));
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemBase::SetOverlayMaterial(UMaterialInterface* MyOverlayMaterial)
{
	ItemMesh->SetOverlayMaterial(MyOverlayMaterial);
}

void AItemBase::ClearOverlayMaterial()
{
	ItemMesh->SetOverlayMaterial(nullptr);
}

