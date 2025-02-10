// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieClimbTriggerBox.h"

#include "CommonZombie.h"
#include "Right4Dead/Right4Dead.h"


// Sets default values
AZombieClimbTriggerBox::AZombieClimbTriggerBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AZombieClimbTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZombieClimbTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AZombieClimbTriggerBox::NotifyActorBeginOverlap(AActor* OtherActor)
{
	PRINT_CALLINFO();
	Super::NotifyActorBeginOverlap(OtherActor);
	ACommonZombie* Zombie = Cast<ACommonZombie>(OtherActor);
	if (Zombie)
	{
		FVector FirstDestination = Zombie->GetActorLocation();
		FirstDestination.Z += 800;
		FVector SecondDestination = FirstDestination;
		SecondDestination.Y -= 300;
		Zombie->StartClimbing(FirstDestination, SecondDestination);
	}
}
