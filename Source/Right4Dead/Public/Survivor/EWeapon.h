#pragma once

#include "CoreMinimal.h"
#include "EWeapon.generated.h"

UENUM(BlueprintType)
enum class EWeapon : uint8
{
	None UMETA(DisplayName = "None"),
	PumpShotgun UMETA(DisplayName = "AutoShotgun"),
	SMG UMETA(DisplayName = "UZI"),
	FireAxe UMETA(DisplayName = "FireAxe"),
	Chainsaw UMETA(DisplayName = "Chainsaw"),
	BaseballBat UMETA(DisplayName = "BaseballBat"),
	Katana UMETA(DisplayName = "Knife"),
	Pistol UMETA(DisplayName = "Pistol"),
	MAX UMETA(Hidden)
};

