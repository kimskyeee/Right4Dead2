#pragma once

#include "CoreMinimal.h"
#include "EItemList.generated.h"

UENUM(BlueprintType)
enum class EItemList : uint8
{
	None UMETA(DisplayName = "None"),
	Bell UMETA(DisplayName = "Whitaker Bell"),
	Coke UMETA(DisplayName = "Coke"),
	Door UMETA(DisplayName = "Door"), 
	Bullet UMETA(DisplayName = "Bullet"), 

	MAX UMETA(Hidden)
};


