// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponItem.h"
#include "FireWeapon.generated.h"

UCLASS()
class RIGHT4DEAD_API AFireWeapon : public AWeaponItem
{
	GENERATED_BODY()
	
	
public:
	// Sets default values for this actor's properties
	AFireWeapon();

	UPROPERTY(EditDefaultsOnly)
	float RateOfFire = 10.f; // 초당 10발
	UPROPERTY(Transient)
	float NextShotTime = 0.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category="Anim")
	TObjectPtr<UAnimMontage> Montage_Reload;
	
	UFUNCTION() void FireOnce();
	UFUNCTION() void DecreaseAmmoCount();
	UFUNCTION() void Reload();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 CurrentAmmo = 50;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxAmmo = 50;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxAmmoAmount = 650;

	UPROPERTY()
	bool bFiredThisPress = false; // 중복방지

	virtual void OnUseStart() override;
	virtual void OnTap(float Elapsed) override;
	virtual void OnHoldBegan() override;
	virtual void OnHoldTick(float Elapsed) override;
	virtual void OnHoldReleased(float Elapsed) override;
};
