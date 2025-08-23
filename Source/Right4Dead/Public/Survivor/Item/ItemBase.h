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
	TObjectPtr<class UItemSpec> Spec = nullptr;

	// 장착/해제
	virtual void OnEquipped(ASurvivor* NewOwner, USceneComponent* Parent, FName Socket);
	virtual void OnUnequipped();

	FName AttachSocket = TEXT("WeaponSocket");
	FName GetAttachSocketName() const { return AttachSocket; }

	// 픽업/드랍
	virtual void OnPickedUp(ASurvivor* NewOwner);	
	virtual void OnDropped();

	virtual void StopPhysicsAndZeroVelocity();
	virtual void SetWorldVisible(bool bVisible);
	
	// 사용(탭/홀드 Phase 전달): 캐릭터는 Phase만 전달한다
	virtual void HandleUse(EUsingType Phase, float ElapsedHold);
	virtual void RightAttack();

	// 소모 시 브로드캐스트(Thrown/HandleObject 등)
	UPROPERTY(BlueprintAssignable)
	FOnItemConsumed OnConsumed;

	// Slot 업데이트
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TObjectPtr<UTexture2D> UIIcon = nullptr;

	UTexture2D* GetUIIcon() const { return UIIcon; }

public:
	UPROPERTY()
	TWeakObjectPtr<ASurvivor> CachedOwner;
	UPROPERTY()
	mutable TWeakObjectPtr<class USurvivorArmAnim> CachedAnim;

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
	TObjectPtr<UAnimMontage> Montage_Shove;
	UPROPERTY(EditDefaultsOnly, Category="Anim")
	TObjectPtr<UAnimMontage> Montage_UnEquip;

	// 실린더 소환
	FTimerHandle CylinderTimerHandle;
	
	//콜리전박스 생성
	UPROPERTY(EditAnywhere, Category = "Shove Attack")
	TObjectPtr<UStaticMesh> CylinderMesh = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> ShoveCollisionCylinder= nullptr;

	UFUNCTION()
	void OnShoveOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
						const FHitResult& SweepResult);
	void SpawnShoveCylinder();
	void DestroyShoveCylinder();

protected:
	bool bUseActive = false;
	bool bHoldTriggered = false;  // 임계값 통과 여부

	class USurvivorArmAnim* GetArmAnim() const;
	void PlayMontageOnce(UAnimMontage* Montage, float Rate = 1.f);
	void EnsureLoopMontage(UAnimMontage* LoopMontage);
	void StopMontageIfPlaying(UAnimMontage* Montage, float BlendOut = 0.1f);

	// 가상 훅: 파생 무기/소비형에서 오버라이드
	virtual void OnUseStart(); // 눌린 그 프레임에 
	virtual void OnTap(float Elapsed); // 짧게 눌렀다 뗐을 때만
	virtual void OnHoldBegan(); // 임계값 통과 순간 1회
	virtual void OnHoldTick(float Elapsed); // 임계값 통과 후 매 프레임
	virtual void OnHoldReleased(float Elapsed); // 홀드 후 뗐을 때
	virtual void OnUseFinish(bool bCanceled); // 정리
};
