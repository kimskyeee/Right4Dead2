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

protected:
	virtual void BeginPlay() override;
	virtual void InitData() override;

public:
	virtual void OnDie() override;

	UPROPERTY()
	UZombieFSM* CommonZombieFSM;
	
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
	
	void TriggerDismemberment(const FPointDamageEvent* PointDamageEvent) const;
	virtual void HandleDie() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void HandlePipeBombBeep(AActor* PipeBombActor) const;
};