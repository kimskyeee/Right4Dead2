// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonZombieAIController.h"


// Sets default values
ACommonZombieAIController::ACommonZombieAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	SetTickableWhenPaused(true);
	SetActorTickInterval(0.1f);
	bStartAILogicOnPossess = true;
	bStopAILogicOnUnposses = true;
	bCanBeInCluster = true;
}

// Called when the game starts or when spawned
void ACommonZombieAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACommonZombieAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

