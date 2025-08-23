// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInfo.h"
#include "Components/ActorComponent.h"
#include "SlotComponent.generated.h"

// UI용
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUIInHandsChanged, ESlotType, Slot, UTexture2D*, Icon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUISlotItemChanged, ESlotType, Slot, UTexture2D*, Icon);

// 게임플레이/애님용
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInHandsItemChanged, class AItemBase* NewItem);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSlotItemChanged, ESlotType Slot, class AItemBase* Item);

USTRUCT()
struct FEquipSlot { GENERATED_BODY()
	UPROPERTY() TWeakObjectPtr<class AItemBase> Item;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RIGHT4DEAD_API USlotComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USlotComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY()
	TArray<FEquipSlot> Slots; // [0] 미사용, [1..4] 사용
	UPROPERTY()
	TWeakObjectPtr<AItemBase> CurrentInHands;

	// Thrown/Handle 이후 복귀용
	UPROPERTY()
	TArray<TWeakObjectPtr<AItemBase>> PreviousItem;

	UFUNCTION()
	bool TryPickup(AItemBase* Item); // E키
	UFUNCTION()
	bool EquipSlot(ESlotType Slot); // 숫자키/Q/휠

	UFUNCTION()
	void Drop(AItemBase* Item);
	UFUNCTION()
	void RemoveItemFromSlot(AItemBase* Item);
	UFUNCTION()
	AItemBase* GetFallBackItem(const AItemBase* Old) const;
	
	UFUNCTION()
	void HandleUse(EUsingType Phase, float Elapsed); // 캐릭터에게 현재 아이템 전달
	UFUNCTION()
	void RightClickUse();

private:
	UPROPERTY()
	int32 SlotSize = 5;
	
	UFUNCTION()
	static int32 Index(ESlotType Slot)
	{
		switch(Slot)
		{
		case
			ESlotType::PrimarySlot:
			return 1;
		case
			ESlotType::SecondarySlot:
			return 2;
		case
			ESlotType::ThrownSlot:
			return 3;
		case
			ESlotType::HandleSlot:
			return 4;
		default: return 0;
		}
	}

	UFUNCTION()
	void EquipItemInHands(AItemBase* NewItem); // 손에 들자
	UFUNCTION()
	void ReturnCurrentToItsPlaceOrDrop();
	
	UFUNCTION()
	void BindConsumption(AItemBase* Item, bool bBind);
	UFUNCTION()
	void OnItemConsumed(AItemBase* Used); // Delegate 콜백

public:
	UFUNCTION()
	bool CycleFilled(int32 Dir);

	UFUNCTION()
	EItemType GetActiveItemType();

public:
	UPROPERTY(BlueprintAssignable, Category="UI")
	FOnUIInHandsChanged OnUIInHandsChanged;
	UPROPERTY(BlueprintAssignable, Category="UI")
	FOnUISlotItemChanged OnUISlotItemChanged;
	
	FOnInHandsItemChanged OnInHandsItemChanged;
	FOnSlotItemChanged OnSlotItemChanged; 

	// 초기 동기화를 위한 헬퍼
	UFUNCTION(BlueprintCallable, Category="UI")
	bool GetSlotIcon(ESlotType Slot, UTexture2D*& OutIcon) const;

private:
	void NotifySlotChanged(ESlotType Slot, AItemBase* Item);
	void NotifyInHandsChanged(AItemBase* Item);
};
