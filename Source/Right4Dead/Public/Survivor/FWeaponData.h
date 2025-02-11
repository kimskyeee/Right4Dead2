#pragma once

#include "CoreMinimal.h"
#include "EWeaponType.h"
#include "FWeaponData.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()

	UPROPERTY(editanywhere, BlueprintReadWrite, Category="WeaponData")
	EWeaponType WeaponName;
	UPROPERTY(editanywhere, BlueprintReadWrite, Category="WeaponData")
	USkeletalMesh* WeaponMesh;
	UPROPERTY(editanywhere, BlueprintReadWrite, Category="WeaponData")
	UAnimMontage* WeaponMontage;
	UPROPERTY(editanywhere, BlueprintReadWrite, Category="WeaponData")
	int32 AmmoCount;
	UPROPERTY(editanywhere, BlueprintReadWrite, Category="WeaponData")
	int32 MaxAmmoCount;
	UPROPERTY(editanywhere, BlueprintReadWrite, Category="WeaponData")
	float Damage;
	UPROPERTY(editanywhere, BlueprintReadWrite, Category="WeaponData")
	float ReloadTime;
	UPROPERTY(editanywhere, BlueprintReadWrite, Category="WeaponData")
	float BulletSpread;
	
};
