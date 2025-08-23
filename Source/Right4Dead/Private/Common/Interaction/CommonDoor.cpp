// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/CommonDoor.h"


// Sets default values
ACommonDoor::ACommonDoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frame"));
	StaticMesh->SetupAttachment(GetRootComponent());

	Hinge = CreateDefaultSubobject<USceneComponent>(TEXT("Hinge"));
	Hinge->SetupAttachment(GetRootComponent());
	Hinge->SetRelativeLocation(FVector(10, -22.5, 0));

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(Hinge);
	Door->SetRelativeLocation(FVector(0, 4.75, 0));

	Door->SetCollisionProfileName(TEXT("WorldWeapon"));
}

// Called when the game starts or when spawned
void ACommonDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACommonDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACommonDoor::Interaction()
{
	Super::Interaction();
	InteractionCommonDoor();
}

void ACommonDoor::SetOverlayMaterial(UMaterialInterface* MyOverlayMaterial)
{
	// 아무것도 하지 않음
}

void ACommonDoor::ClearOverlayMaterial()
{
	// 아무것도 하지 않음
}


