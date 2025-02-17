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
	EWeaponType WeaponName = EWeaponType::None;
	UPROPERTY(editanywhere, BlueprintReadWrite, Category="WeaponData")
	TSubclassOf<AWeaponBase> WeaponFactory;
	UPROPERTY(editanywhere, BlueprintReadWrite, Category="WeaponData")
	UAnimMontage* WeaponDrawMontage = nullptr;
	UPROPERTY(editanywhere, BlueprintReadWrite, Category="WeaponData")
	UAnimMontage* WeaponFireMontage = nullptr;
	UPROPERTY(editanywhere, BlueprintReadWrite, Category="WeaponData")
	UAnimMontage* WeaponReloadMontage = nullptr;
	UPROPERTY(editanywhere, BlueprintReadWrite, Category="WeaponData")
	uint8 AmmoCount = 0;
	UPROPERTY(editanywhere, BlueprintReadWrite, Category="WeaponData")
	uint8 MaxAmmoCount = 0;
	UPROPERTY(editanywhere, BlueprintReadWrite, Category="WeaponData")
	float Damage = 0;
	UPROPERTY(editanywhere, BlueprintReadWrite, Category="WeaponData")
	float ReloadTime = 0;
	UPROPERTY(editanywhere, BlueprintReadWrite, Category="WeaponData")
	float BulletSpread = 0;
};
