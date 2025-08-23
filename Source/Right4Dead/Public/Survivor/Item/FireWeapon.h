// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponItem.h"
#include "FireWeapon.generated.h"

UCLASS()
class RIGHT4DEAD_API AFireWeapon : public AWeaponItem
{
	GENERATED_BODY()
	float FireCooldown = 0.09f;
	float Accum = 0.f;
	bool bHolding = false;
	
public:
	// Sets default values for this actor's properties
	AFireWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void HandleUse(EUsingType Phase, float ElapsedHold) override;
	
	UFUNCTION() void FireAttack();
	UFUNCTION() void DecreaseAmmoCount();
	UFUNCTION() void Reload();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 CurrentAmmo = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxAmmo = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxAmmoAmount = 0;
};
