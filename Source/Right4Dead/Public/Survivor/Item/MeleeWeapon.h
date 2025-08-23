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

	virtual void HandleUse(EUsingType Phase, float ElapsedHold) override;
	void MeleeAttack();

	//TMap
	UPROPERTY(VisibleAnywhere)
	TMap<FName, int> BoneMap;
	
	UPROPERTY(Editanywhere)
	float LastSecondaryAttackTime; // 마지막 도끼공격
	UPROPERTY(Editanywhere)
	float SecondaryAttackCooldown = 1.0f; // 쿨타임

	void InitBoneMap();
};
