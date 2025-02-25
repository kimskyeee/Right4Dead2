// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "AWeapon_Heal.generated.h"

UCLASS()
class RIGHT4DEAD_API AAWeapon_Heal : public AWeaponBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAWeapon_Heal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(visibleAnywhere, BlueprintReadWrite)
	bool bIsHoldingLeft=false;
	UPROPERTY()
	class ASurvivor* survivor;
	
	virtual void LeftClickEvent() override;
};
