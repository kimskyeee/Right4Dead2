// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieAnimInstance.h"
#include "Components/ActorComponent.h"
#include "ZombieAudioComponent.generated.h"

class UZombieBaseFSM;
class AZombieBase;

UCLASS(ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent))
class RIGHT4DEAD_API UZombieAudioComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UZombieAudioComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Debugging")
	TObjectPtr<AZombieBase> Owner = nullptr;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Debugging")
	TObjectPtr<UZombieBaseFSM> ZombieFsm = nullptr;
	
	UPROPERTY(BlueprintReadWrite, Category = "Audio")
	TObjectPtr<USoundCue> ChaseSoundCue = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Audio")
	TObjectPtr<USoundCue> FootstepSoundCue = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Audio")
	TObjectPtr<USoundCue> DieSoundCue = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Audio")
	TObjectPtr<USoundCue> PunchSoundCue = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Audio")
	TObjectPtr<USoundCue> HitByBulletSoundCue = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Audio")
	TObjectPtr<USoundCue> DismembermentSoundCue = nullptr;

	UFUNCTION(BlueprintImplementableEvent)
	void OnChangedState(const EZombieState& BeforeState, const EZombieState& AfterState);
	UFUNCTION(BlueprintImplementableEvent)
	void OnNormalAttacked();
	UFUNCTION(BlueprintImplementableEvent)
	void OnBulletHit();
	UFUNCTION(BlueprintImplementableEvent)
	void OnDismemberment();
};