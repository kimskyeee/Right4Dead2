
#pragma once

#include "CoreMinimal.h"
#include "ItemInfo.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None UMETA(DisplayName = "None"),
	Primary UMETA(DisplayName = "Primary Weapon"),      // 주무기 (총)
	Secondary UMETA(DisplayName = "Secondary Weapon"),  // 보조무기 - 권총(기본제공)
	Melee UMETA(DisplayName = "Melee Weapon"),          // 보조무기 - 근접무기
	Thrown UMETA(DisplayName = "Thrown Weapon"),        // 투척무기
	
	HandleObject UMETA(DisplayName = "Hadle Object"),   // 힐키트 (회복)
	QuestObject UMETA(DisplayName = "Quest Object"),    // 콜라배달 전용슬롯
};

UENUM(BlueprintType)
enum class ESlotType : uint8
{
	None,                // 슬롯에 들어가지 않음 (퀘스트)
	PrimarySlot = 1,     // 1번 슬롯
	SecondarySlot = 2,   // 2번 슬롯 (Secondary, Melee 공용)
	ThrownSlot = 3,      // 3번 슬롯
	HandleSlot = 4,      // 4번 슬롯
};

UENUM()
enum class EUsingType : uint8
{
	Started,
	Ongoing,
	Completed,
	Canceled,
};

