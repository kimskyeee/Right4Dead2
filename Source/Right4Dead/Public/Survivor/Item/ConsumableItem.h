// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "ConsumableItem.generated.h"

UCLASS()
class RIGHT4DEAD_API AConsumableItem : public AItemBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AConsumableItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	float NeedHold = 5.0f; // 5초동안 홀드
	
protected:
	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundWave* CokeDeliverySound; //콜라배달 사운드
	UPROPERTY()
	class UAudioComponent* ColaDeliveryAudio;
	
	bool bHoldActive = false;
	bool bCompleted = false; // 소비 확정 여부
	bool bInUseGuard = false; // 재진입 가드

	virtual void ApplyItemEffect();
	
	virtual void CompleteConsume(); // 5초 도달 시 호출
	virtual void CancelConsume(); // 5초 미만에서 뗐을 때 호출

public:
	virtual void OnTap(float Elapsed) override;
	virtual void OnHoldBegan() override;
	virtual void OnHoldTick(float Elapsed) override;
	virtual void OnHoldReleased(float Elapsed) override;
};
