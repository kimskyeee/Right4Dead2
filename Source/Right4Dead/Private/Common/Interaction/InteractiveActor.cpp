// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/InteractiveActor.h"

#include "Survivor.h"


// Sets default values
AInteractiveActor::AInteractiveActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//overlap되면 Material Instance (Overlay 설정)
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> TempWeaponOverlay(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Blueprints/Survivor/Materials/M_Outline_Inst.M_Outline_Inst'"));
	if (TempWeaponOverlay.Succeeded())
	{
		OverlayMaterial = TempWeaponOverlay.Object;
	}
}

// Called when the game starts or when spawned
void AInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractiveActor::SetOverlayMaterial()
{
	StaticMesh->SetOverlayMaterial(OverlayMaterial);
}

void AInteractiveActor::ClearOverlayMaterial()
{
	StaticMesh->SetOverlayMaterial(nullptr);
}

void AInteractiveActor::Interaction()
{
}
