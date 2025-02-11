#pragma once

#include "CoreMinimal.h"
#include "EWeaponType.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None UMETA(DisplayName = "None"),
	Primary UMETA(DisplayName = "Primary Weapon"),   // 주무기
	Secondary UMETA(DisplayName = "Secondary Weapon"), // 보조무기
	Melee UMETA(DisplayName = "Melee Weapon"), //근접무기
	/*PumpShotgun UMETA(DisplayName = "AutoShotgun"),
	SMG UMETA(DisplayName = "UZI"),
	FireAxe UMETA(DisplayName = "FireAxe"),
	BaseballBat UMETA(DisplayName = "BaseballBat"),
	Katana UMETA(DisplayName = "Knife"),
	Pistol UMETA(DisplayName = "Pistol"),
	Pipe UMETA(DisplayName = "Pipe"),*/
	MAX UMETA(Hidden)
};

