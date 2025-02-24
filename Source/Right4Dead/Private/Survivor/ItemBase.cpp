// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"

#include "Components/SphereComponent.h"
#include "Right4Dead/Right4Dead.h"


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
	
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemBase::SetOverlayMaterial(UMaterialInterface* MyOverlayMaterial)
{
	///ItemMesh->SetOverlayMaterial(MyOverlayMaterial);
}

void AItemBase::ClearOverlayMaterial()
{
	// ItemMesh->SetOverlayMaterial(nullptr);
}

void AItemBase::Interaction()
{
	PRINT_CALLINFO();
}

