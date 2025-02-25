#pragma once

#include "CoreMinimal.h"
#include "EWeaponType.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None UMETA(DisplayName = "None"),
	Primary UMETA(DisplayName = "Primary Weapon"),   // 주무기 (총류, 스나이퍼추가될 수 있음)
	Secondary UMETA(DisplayName = "Secondary Weapon"), // 보조무기 (도끼, 빠따, 일본도, 원래는 권총)
	Melee UMETA(DisplayName = "Melee Weapon"), // 근접무기..인데 투척무기 (파이프폭탄, 화염병)
	HandleObject UMETA(DisplayName = "Hadle Object"), // 힐키트
	CokeDelivery UMETA(DisplayName = "Coke Delivery"), // 콜라배달 전용슬롯
	// 콜라들고있을때 좌클릭(던짐), 우클릭시(밀치기)
	
	MAX UMETA(Hidden)
};

