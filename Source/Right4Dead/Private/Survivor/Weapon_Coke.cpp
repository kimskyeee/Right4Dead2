// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Coke.h"


// Sets default values
AWeapon_Coke::AWeapon_Coke()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWeapon_Coke::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AWeapon_Coke::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

