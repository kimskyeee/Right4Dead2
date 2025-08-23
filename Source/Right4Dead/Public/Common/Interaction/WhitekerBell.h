// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveActor.h"
#include "WhitekerBell.generated.h"

UCLASS()
class RIGHT4DEAD_API AWhitekerBell : public AInteractiveActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWhitekerBell();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Root = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Bell = nullptr;

	virtual void Interaction() override;
	virtual void SetOverlayMaterial() override;
	virtual void ClearOverlayMaterial() override;

	UFUNCTION(CallInEditor, BlueprintImplementableEvent, Category = "Debugging")
	void InteractionBell();
};
