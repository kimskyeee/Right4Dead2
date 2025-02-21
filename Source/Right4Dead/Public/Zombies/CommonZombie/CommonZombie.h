#pragma once

#include "CoreMinimal.h"
#include "ZombieBase.h"
#include "CommonZombie.generated.h"

class AAIController;
class UZombieAnimInstance;
class UZombieFSM;

UCLASS()
class RIGHT4DEAD_API ACommonZombie : public AZombieBase
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ACommonZombie();
	
private:
	UFUNCTION(CallInEditor, Category="Debugging")
	void ForceDie();

protected:
	float HitDamageRatio = 2;
	float AttackDamage = 1;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void InitDifficulty() override;

public:
	virtual void HandleNormalAttack() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnDamaged(float Damage) override;
	virtual void OnDie() override;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMesh> HeadMesh = nullptr;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMesh> ArmLeftMesh = nullptr;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMesh> ArmRightMesh = nullptr;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMesh> LegLeftMesh = nullptr;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMesh> LegRightMesh = nullptr;
	void HandleDismemberment(const FPointDamageEvent* PointDamageEvent);

	UFUNCTION(BlueprintCallable)
	virtual void HandleShove(const FVector& FromLocation) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};