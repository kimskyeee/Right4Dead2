// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInfo.h"
#include "GameFramework/Actor.h"
#include "Interaction/InteractiveActor.h"
#include "ItemBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemConsumed, class AItemBase*, Item);

UCLASS()
class RIGHT4DEAD_API AItemBase : public AInteractiveActor
{
	GENERATED_BODY()

public:                                                        
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class ASurvivor* Char = nullptr;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UItemSpec* Spec = nullptr;

	// 장착/해제
	virtual void OnEquipped(class ASurvivor* OwnerChar);
	virtual void OnUnequipped();

	// 픽업/드랍
	virtual void OnPickedUp()
	{
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}
	
	virtual void OnDropped()
	{
		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);
		if (auto* Prim = FindComponentByClass<UPrimitiveComponent>())
		{
			Prim->SetSimulatePhysics(true);
		}
	}
	
	// 사용(탭/홀드 Phase 전달): 캐릭터는 Phase만 전달한다
	virtual void HandleUse(EUsingType Phase, float ElapsedHold) {}
	virtual void RightAttack();

	// 소모 시 브로드캐스트(Thrown/HandleObject 등)
	UPROPERTY(BlueprintAssignable)
	FOnItemConsumed OnConsumed;

protected:
	UPROPERTY()
	TWeakObjectPtr<ACharacter> CachedOwner;
	UPROPERTY()
	TWeakObjectPtr<UAnimInstance> CachedAnim;

	// 공통적으로 쓸 수 있는 몽타주 슬롯들(필요한 것만 사용)
	UPROPERTY(EditDefaultsOnly, Category="Anim")
	TObjectPtr<UAnimMontage> Montage_Equip;
	UPROPERTY(EditDefaultsOnly, Category="Anim")
	TObjectPtr<UAnimMontage> Montage_Use_Tap;
	UPROPERTY(EditDefaultsOnly, Category="Anim")
	TObjectPtr<UAnimMontage> Montage_Use_HoldStart;
	UPROPERTY(EditDefaultsOnly, Category="Anim")
	TObjectPtr<UAnimMontage> Montage_Use_HoldLoop;
	UPROPERTY(EditDefaultsOnly, Category="Anim")
	TObjectPtr<UAnimMontage> Montage_Use_Release;
	UPROPERTY(EditDefaultsOnly, Category="Anim")
	TObjectPtr<UAnimMontage> Montage_UnEquip;

	// 재생
	bool PlayItemMontage(UAnimMontage* ItemMontage) const
	{
		if (!CachedAnim.IsValid() || !ItemMontage) return false;
		
		const float Len = CachedAnim->Montage_Play(ItemMontage);
		
		return (Len > 0.f);
	}

	bool IsPlaying(UAnimMontage* ItemMontage) const
	{
		return CachedAnim.IsValid() && ItemMontage && CachedAnim->Montage_IsPlaying(ItemMontage);
	}

	// 실린더 소환
	FTimerHandle CylinderTimerHandle;
	
	//콜리전박스 생성
	UPROPERTY(EditAnywhere, Category = "Shove Attack")
	UStaticMesh* CylinderMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ShoveCollisionCylinder;
	UFUNCTION()
	void OnShoveOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
						const FHitResult& SweepResult);
	void SpawnShoveCylinder();
	void DestroyShoveCylinder();
};
