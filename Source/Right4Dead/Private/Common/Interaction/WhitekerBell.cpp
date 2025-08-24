// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/WhitekerBell.h"


// Sets default values
AWhitekerBell::AWhitekerBell()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bell"));
	StaticMesh->SetupAttachment(RootComponent);

	//충돌체 설정
	StaticMesh->SetGenerateOverlapEvents(true);
	StaticMesh->SetCollisionProfileName(TEXT("WorldWeapon"));
}

// Called when the game starts or when spawned
void AWhitekerBell::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWhitekerBell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWhitekerBell::Interaction()
{
	Super::Interaction();
	InteractionBell();
}

void AWhitekerBell::SetOverlayMaterial(UMaterialInterface* MyOverlayMaterial)
{
	Super::SetOverlayMaterial(MyOverlayMaterial);
}

void AWhitekerBell::ClearOverlayMaterial()
{
	Super::ClearOverlayMaterial();
}
