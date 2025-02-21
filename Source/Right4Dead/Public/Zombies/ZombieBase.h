// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase.h"
#include "GameFramework/Character.h"
#include "ZombieBase.generated.h"

USTRUCT()
struct FPartDamageMultipliers
{
	GENERATED_BODY()
	float Head = 4;
	float Stomach = 1.25f;
	float Legs = 0.75f;
	float Thorax = 1;
};

UCLASS(Abstract)
class RIGHT4DEAD_API AZombieBase : public ACharacter, public IActorBase
{
	GENERATED_BODY()

private:
	float FinalDamage;
	
protected:
	UPROPERTY(EditInstanceOnly, Category=Debugging)
	float Hp;
	float Speed;
	float TakeDamageMultiplier;
	FPartDamageMultipliers PartDamageMultipliers;
	bool bTakeDamaged;
	virtual void BeginPlay() override;
	virtual void InitDifficulty() PURE_VIRTUAL(AZombieBase::InitDifficulty, );
	
public:
	AZombieBase();

	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION()
	void OnTakeAnyDamageHandler(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	UFUNCTION()
	void OnTakePointDamageHandler(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent,
								  FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);
	UFUNCTION()
	void OnTakeRadialDamageHandler(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, const FHitResult& HitInfo,
								   class AController* InstigatedBy, AActor* DamageCauser);
	
	virtual void HandleNormalAttack();
	virtual void HandleSpecialAttack();
	virtual void HandleShove(const FVector& FromLocation);
	virtual void OnDamaged(float Damage) override;
	virtual void OnDie() override;
};