// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInfo.h"
#include "Engine/DataAsset.h"
#include "ItemSpec.generated.h"

/**
 * 
 */

UCLASS()
class RIGHT4DEAD_API UItemSpec : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	EItemType ItemType = EItemType::None;
	UPROPERTY(EditDefaultsOnly)
	ESlotType PreferredSlot = ESlotType::None;
	UPROPERTY(EditDefaultsOnly)
	bool bOccupiesSlot = false; // 슬롯 존재 여부
	UPROPERTY(EditDefaultsOnly)
	bool bConsumesOnUse = false; // 사용 후 소멸/파괴 여부
	UPROPERTY(EditDefaultsOnly)
	float HoldThresholdSec = 0.2f; // 홀드로 전한 임계값
	UPROPERTY(EditDefaultsOnly)
	bool bAutoFireWhenHeld = false; // 총기 연사
};
