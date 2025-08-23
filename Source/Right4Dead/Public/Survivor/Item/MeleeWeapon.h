// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FireWeapon.h"
#include "MeleeWeapon.generated.h"

UCLASS()
class RIGHT4DEAD_API AMeleeWeapon : public AWeaponItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMeleeWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//TMap
	UPROPERTY(VisibleAnywhere)
	TMap<FName, int> BoneMap;

	UPROPERTY(EditDefaultsOnly, Category="Use")
	float CooldownSec = 1.0f;

	void InitBoneMap();

protected:
	UPROPERTY(Transient)
	float NextReadyTime = 0.f;

	virtual void OnTap(float Elapsed) override;
	virtual void OnHoldBegan() override;
	virtual void OnHoldTick(float Elapsed) override;
	
	void MeleeAttack();
};
