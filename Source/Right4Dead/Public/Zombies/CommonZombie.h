// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieBase.h"
#include "CommonZombie.generated.h"

UCLASS()
class RIGHT4DEAD_API ACommonZombie : public AZombieBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ACommonZombie();

protected:
	float HitDamageRatio = 2;
	float AttackDamage = 1;
	bool bClimbing = false;
	bool bFirst = false;
	bool bSecond = false;
	FVector FD = FVector::ZeroVector;
	FVector SD = FVector::ZeroVector;
	UPROPERTY(EditAnywhere)
	AActor* Target = nullptr;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void InitDifficulty() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void StartClimbing(FVector FirstDest, FVector SecondDest);
};