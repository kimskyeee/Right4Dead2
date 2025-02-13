#pragma once

#include "CoreMinimal.h"
#include "EWeaponType.h"
#include "FWeaponData.generated.h"

class AWeaponBase;

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()

	UPROPERTY(editanywhere, BlueprintReadWrite, Category="WeaponData")
	EWeaponType WeaponName;
	UPROPERTY(editanywhere, BlueprintReadWrite, Category="WeaponData")
	TSubclassOf<AWeaponBase> WeaponFactory; 
	UPROPERTY(editanywhere, BlueprintReadWrite, Category="WeaponData")
	UAnimMontage* WeaponMontage;
	UPROPERTY(editanywhere, BlueprintReadWrite, Category="WeaponData")
	UAnimMontage* WeaponFireMontage;
	UPROPERTY(editanywhere, BlueprintReadWrite, Category="WeaponData")
	uint8 AmmoCount;
	UPROPERTY(editanywhere, BlueprintReadWrite, Category="WeaponData")
	uint8 MaxAmmoCount;
	UPROPERTY(editanywhere, BlueprintReadWrite, Category="WeaponData")
	float Damage;
	UPROPERTY(editanywhere, BlueprintReadWrite, Category="WeaponData")
	float ReloadTime;
	UPROPERTY(editanywhere, BlueprintReadWrite, Category="WeaponData")
	float BulletSpread;
};
