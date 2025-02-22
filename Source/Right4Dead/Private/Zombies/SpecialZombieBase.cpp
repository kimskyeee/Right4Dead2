// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialZombieBase.h"


// Sets default values
ASpecialZombieBase::ASpecialZombieBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpecialZombieBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpecialZombieBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASpecialZombieBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

