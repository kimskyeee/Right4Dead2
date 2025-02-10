#pragma once

#include "CoreMinimal.h"
#include "EWeapon.h"
#include "WeaponBase.h"
#include "FGunData.generated.h"

USTRUCT(BlueprintType)
struct FGunData
{
	GENERATED_BODY()

	UPROPERTY(editanywhere, BlueprintReadWrite)
	EWeapon WeaponName;

	UPROPERTY(editanywhere, BlueprintReadWrite)
	int32 AmmoCount;
	UPROPERTY(editanywhere, BlueprintReadWrite)
	int32 MaxAmmoCount;
	UPROPERTY(editanywhere, BlueprintReadWrite)
	float ReloadTime;
	UPROPERTY(editanywhere, BlueprintReadWrite)
	float BulletSpread;
	
};
