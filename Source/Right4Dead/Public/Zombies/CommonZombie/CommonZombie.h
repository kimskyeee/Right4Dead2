#pragma once

#include "CoreMinimal.h"
#include "ZombieBase.h"
#include "CommonZombie.generated.h"

class UZombieAnimInstance;
class UZombieFSM;
class AZombieAIController;

UCLASS()
class RIGHT4DEAD_API ACommonZombie : public AZombieBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ACommonZombie();

protected:
	float HitDamageRatio = 2;
	float AttackDamage = 1;
	FTransform ClimbDestination = FTransform::Identity;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void InitDifficulty() override;

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> Target = nullptr;
	UPROPERTY()
	TObjectPtr<AZombieAIController> AIController = nullptr;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UZombieFSM> ZombieFSM = nullptr;
	UPROPERTY()
	TObjectPtr<UZombieAnimInstance> ZombieAnimInstance = nullptr;
	
	bool bClimbing = false;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void StartClimbing(const FTransform& Destination);
	void EndClimbing();
	AActor* GetChasingTarget();
	void TriggerAttack();
	UFUNCTION(BlueprintCallable)
	virtual void HandleShove(const FVector& FromLocation) override;
	virtual void OnDamaged(float Damage) override;
	virtual void OnDie() override;
	UFUNCTION(CallInEditor, Category="Debugging")
	void ForceDie();
};