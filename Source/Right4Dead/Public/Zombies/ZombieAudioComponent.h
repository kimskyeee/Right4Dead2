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
	UPROPERTY(BlueprintReadOnly, Category = "Debugging")
	TObjectPtr<AZombieBase> Owner = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Debugging")
	TObjectPtr<UZombieBaseFSM> ZombieFsm = nullptr;
	
	UPROPERTY(BlueprintReadWrite, Category = "Audio")
	TObjectPtr<UAudioComponent> ChaseAudio = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "Audio")
	TObjectPtr<USoundCue> DieSoundCue = nullptr;

	UFUNCTION(BlueprintImplementableEvent)
	void OnChangedState(const EZombieState& BeforeState, const EZombieState& AfterState);
};