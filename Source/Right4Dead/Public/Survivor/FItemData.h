#pragma once
#include "EItemList.h"

class AItemBase;

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

	UPROPERTY(editanywhere, BlueprintReadWrite, Category="ItemData")
	EItemList ItemName = EItemList::None;
	UPROPERTY(editanywhere, BlueprintReadWrite, Category="ItemData")
	TSubclassOf<AItemBase> ItemFactory;
	UPROPERTY(editanywhere, BlueprintReadWrite, Category="ItemData")
	UAnimMontage* ItemDrawMontage = nullptr;
	UPROPERTY(editanywhere, BlueprintReadWrite, Category="ItemData")
	UAnimMontage* ItemLeftClickMontage = nullptr;
};
