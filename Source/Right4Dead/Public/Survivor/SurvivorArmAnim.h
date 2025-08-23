// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survivor.h"
#include "Animation/AnimInstance.h"
#include "Item/ItemInfo.h"
#include "SurvivorArmAnim.generated.h"

/**
 * 
 */
UCLASS()
class RIGHT4DEAD_API USurvivorArmAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Char")
	TWeakObjectPtr<class ASurvivor> OwnerChar = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="SlotComp")
	TWeakObjectPtr<class USlotComponent> SlotComp = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Anim")
	FName ArmsSlotName = TEXT("DefaultSlot");
	
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UFUNCTION()
	void AnimNotify_AxeCameraShake();
	UFUNCTION()
	void AnimNotify_SpawnCylinder();
	UFUNCTION()
	void AnimNotify_ENDReloading();

	UPROPERTY(BlueprintReadOnly, Category="Anim")
	EItemType CurrentType = EItemType::None;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FSM")
	float speed = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FSM")
	float direction = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FSM")
	bool bIsInAir=false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FSM")
	bool bIsEquipping = false;

	UFUNCTION()
	void BindDelegates();
	UFUNCTION()
	void UnbindDelegates();
	
	UFUNCTION()
	void HandleInHandsChanged(class AItemBase* NewItem);
	UFUNCTION()
	void PlayEquipMontageForItem(class AItemBase* NewItem);
};
