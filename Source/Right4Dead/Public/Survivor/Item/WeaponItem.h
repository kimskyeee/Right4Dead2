// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "WeaponItem.generated.h"

UCLASS()
class RIGHT4DEAD_API AWeaponItem : public AItemBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//외관
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USphereComponent* Root = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UStaticMeshComponent* WeaponMesh = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USceneComponent* Muzzle = nullptr;

	virtual void SetOverlayMaterial() override;
};
