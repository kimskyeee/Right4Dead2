// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "CommonDoor.generated.h"

UCLASS()
class RIGHT4DEAD_API ACommonDoor : public AItemBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACommonDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Root = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Frame = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> Hinge = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Door = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsOpen = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsCanOpen = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsCokeDelivery = false;
	
	virtual void Interaction() override;
	virtual void SetOverlayMaterial(UMaterialInterface* MyOverlayMaterial) override;
	virtual void ClearOverlayMaterial() override;

	UFUNCTION(CallInEditor, BlueprintImplementableEvent, Category = "Debugging")
	void InteractionCommonDoor();
};
