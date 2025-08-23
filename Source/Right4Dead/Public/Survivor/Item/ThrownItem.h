// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponItem.h"
#include "ThrownItem.generated.h"

UCLASS()
class RIGHT4DEAD_API AThrownItem : public AWeaponItem
{
	GENERATED_BODY()
	bool bArmed = false;
	
public:
	// Sets default values for this actor's properties
	AThrownItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void HandleUse(EUsingType Phase, float ElapsedHold) override;
	void ThrownAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* TrailParticle;  

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsThrown = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bHasLanded = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Damping = 5.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float BombTimerRate = 5.8f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TraceRate = 1.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float LIfeTime = 6.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float CurrentLifeTime = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float CurrentBeepTime = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float BeepInterval = 0.8f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float CurrentAggroTime = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float BombDamage = 500.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float DamageRadius = 600.f;
	
	FTimerHandle PipeBombTraceTimerHandle;
	FTimerHandle BeepSoundTimerHandle;
	FTimerHandle ExplosionTimerHandle;
	
	UFUNCTION()
	void ThrowWeapon();
	UFUNCTION()
	void OnThrowWeaponHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
						  UPrimitiveComponent* OtherComp, FVector NormalImpulse,
						  const FHitResult& Hit);

	UFUNCTION()
	void PipeBombTraceZombies();
	UFUNCTION()
	void ExplodeWeapon();

	UFUNCTION(CallInEditor,BlueprintImplementableEvent,Category="PipeBomb")
	void PlayBeepSound(float DeltaTime);

	UPROPERTY(EditAnywhere)
	class UParticleSystem* ExplosionEffect;
	UPROPERTY(EditAnywhere)
	class UPointLightComponent* BeepLight;
	
	class USoundWave* PipeBombBeep; //폭탄삐삡
	class USoundWave* PipeBombEnd; //폭탄터짐
};
