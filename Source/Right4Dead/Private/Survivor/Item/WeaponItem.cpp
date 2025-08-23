// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/WeaponItem.h"



// Sets default values
AWeaponItem::AWeaponItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//외관
	Muzzle=CreateDefaultSubobject<USceneComponent>("Muzzle");
	Muzzle->SetupAttachment(StaticMesh);
	Muzzle->SetRelativeLocationAndRotation(FVector(0,57,-2.5),FRotator(0,90,0));
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
