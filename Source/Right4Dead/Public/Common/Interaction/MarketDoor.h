// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveActor.h"
#include "Item/ItemBase.h"
#include "MarketDoor.generated.h"

class AZombieSpawnManager;

UCLASS()
class RIGHT4DEAD_API AMarketDoor : public AInteractiveActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMarketDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USoundWave> AlarmSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> AlarmLocation = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> LeftHinge = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> RightHinge = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Frame = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> LeftDoor = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> LeftBackGrip = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> LeftFrontGrip = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> LeftGlass = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> RightDoor = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> RightBackGrip = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> RightFrontGrip = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> RightGlass = nullptr;
	
	virtual void Interaction() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Debugging")
    bool bIsOpen = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Debugging")
	bool bFirst = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Debugging")
	TObjectPtr<AZombieSpawnManager> ZombieSpawnManager = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Debugging")
	bool bAlarming = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Debugging")
	float HordeTriggerTime = 30.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Debugging")
	float CurrentAlarmTime = 30.0f;
	
	UFUNCTION(CallInEditor, BlueprintImplementableEvent, Category = "Debugging")
	void InteractionDoor();

	UFUNCTION(CallInEditor, BlueprintImplementableEvent, BlueprintCallable, Category = "Debugging")
	void StopAlarm();

	virtual void SetOverlayMaterial(UMaterialInterface* MyOverlayMaterial) override;
	virtual void ClearOverlayMaterial() override;
};