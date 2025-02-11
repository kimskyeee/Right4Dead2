// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatSystem.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RIGHT4DEAD_API UStatSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category = "Health")
	void IncreaseHealth(float HealthPoint);
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category = "Health")
	void DecreaseHealth(float DamagePoint);
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category = "Health")
	void UpdateHPText();
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category = "Health")
	void InitializeStat();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth=200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	bool bIsDead;
	
};
