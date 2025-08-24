// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/ItemInfo.h"
#include "UIWeaponSlot.generated.h"

/**
 * 
 */

USTRUCT()
struct FSlotView
{
	GENERATED_BODY()
	
	// 공통(1~4): 배경
	class UImage* BGWhite = nullptr;
	class UImage* BGGreen = nullptr;

	// 1번 전용
	UImage* P_IconNormal = nullptr;
	UImage* P_IconEquip = nullptr;

	// 2~4번 전용
	UImage* IconNormal = nullptr;

	// 1번만: 탄약 UI
	class UTextBlock* TextCur = nullptr;
	class UTextBlock* TextMax = nullptr;

	// 슬롯별 기본 아이콘(디자이너 지정)
	class UTexture2D* DefaultNormalIcon = nullptr; // 1~4 공통으로 슬롯 기본 아이콘
	class UTexture2D* CachedUnequipIcon = nullptr; // 1번 전용(비활성 시 보여줄 최근 프라이머리 아이콘)
};


UCLASS()
class RIGHT4DEAD_API UUIWeaponSlot : public UUserWidget
{
	GENERATED_BODY()
public:
    virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

    // 1번(Primary)
    UPROPERTY(meta=(BindWidget)) UImage* S1_BGWhite;
    UPROPERTY(meta=(BindWidget)) UImage* S1_BGGreen;
    UPROPERTY(meta=(BindWidget)) UImage* S1_IconNormal; // Primary Normal
    UPROPERTY(meta=(BindWidget)) UImage* S1_IconEquip; // Primary Equip
    UPROPERTY(meta=(BindWidgetOptional)) UTextBlock* S1_TextCurrent;
    UPROPERTY(meta=(BindWidgetOptional)) UTextBlock* S1_TextMax;

    // 2번
    UPROPERTY(meta=(BindWidget)) UImage* S2_BGWhite;
    UPROPERTY(meta=(BindWidget)) UImage* S2_BGGreen;
    UPROPERTY(meta=(BindWidget)) UImage* S2_IconNormal;   // 단일 이미지

    // 3번(Thrown)
    UPROPERTY(meta=(BindWidget)) UImage* S3_BGWhite;
    UPROPERTY(meta=(BindWidget)) UImage* S3_BGGreen;
    UPROPERTY(meta=(BindWidget)) UImage* S3_IconNormal;

    // 4번(Handle)
    UPROPERTY(meta=(BindWidget)) UImage* S4_BGWhite;
    UPROPERTY(meta=(BindWidget)) UImage* S4_BGGreen;
    UPROPERTY(meta=(BindWidget)) UImage* S4_IconNormal;

    // 슬롯별 기본 아이콘
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Slot")
	UTexture2D* S1_DefaultNormal = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Slot")
	UTexture2D* S2_DefaultNormal = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Slot")
	UTexture2D* S3_DefaultNormal = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Slot")
	UTexture2D* S4_DefaultNormal = nullptr;
	
    UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class USlotComponent> SlotComp;

	UFUNCTION(BlueprintCallable)
	void SetupSlotComponent(USlotComponent* InComp);
	
	// 탄약 델리게이트 중복 방지용
	TWeakObjectPtr<class AFireWeapon> BoundFire = nullptr;

private:
    // 1..4 인덱스 사용(0 비움)
    TArray<FSlotView> Slots;
    int32 ActiveIdx = 0;

	UFUNCTION()
	void InitUISlot();
	
    // 델리게이트 바인딩
    UFUNCTION()
	void HandleInHandsChanged(ESlotType ActiveSlot, UTexture2D* ActiveIcon);
    UFUNCTION()
	void HandleSlotIconChanged(ESlotType NewSlot, UTexture2D* NewIcon);
    UFUNCTION()
	void HandleSlotItemChangedPtr(ESlotType SlotType, class AItemBase* Item);
    UFUNCTION()
	void OnAmmoChanged(int32 Cur, int32 Max);
	
    void ApplyActive(int32 Idx, bool bActive);
    void SetActiveIcon(int32 Idx, UTexture2D* Icon); // 활성 아이콘
    void SetInactiveVisual(int32 Idx);  // 비활성 표현(1번은 Unequip/Normal, 2~4는 항상 DefaultNormal)

	void BindAll();
	void UnbindAll();
	
    void BindPrimaryAmmo();
    void UnbindPrimaryAmmo();
	
    int32 Index(ESlotType SlotType) const;
};
