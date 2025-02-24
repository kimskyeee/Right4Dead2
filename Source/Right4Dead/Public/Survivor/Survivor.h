// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase.h"
#include "FWeaponData.h"
#include "WeaponBase.h"
#include "GameFramework/Character.h"
#include "Survivor.generated.h"

class AItemBase;
class UBoxComponent;

UCLASS()
class RIGHT4DEAD_API ASurvivor : public ACharacter, public IActorBase
{
	GENERATED_BODY()
	
	

public:
	// Sets default values for this character's properties
	ASurvivor();

protected:
	UFUNCTION()
	void OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                     const FHitResult& SweepResult);
	UFUNCTION()
	void OnWeaponEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 보유중인 모든 무기 인스턴스
	UPROPERTY()
	TArray<FWeaponData> WeaponInstances; 
	
	// 디버그모드 온오프
	UPROPERTY(EditAnywhere, Category=DEBUG)
	bool bDebugPlay = false;
	
	//UI붙이기 (Widget)
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> MainUIFactory;
	UPROPERTY(EditAnywhere)
	class UUISurvivorMain* SurvivorMainUI;
	//캐싱
	UPROPERTY()
	class UUISurvivorCrosshair* CrosshairUI;
	//애니메이션 재생 시에 add to viewport
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<class UUITakeDamage> TakeDamageUIClass;
	UPROPERTY()
	class UUITakeDamage* TakeDamageUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<class UUIAttackZombie> AttackZombieUIClass;
	UPROPERTY()
	class UUIAttackZombie* AttackZombieUI;
	//회복 UI 캐싱
	UPROPERTY()
	class UUISurvivorMedKit* MedKitUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<class UUISurvivorMedKit> MedKitUIClass;

	//외관추가
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USkeletalMeshComponent* Head;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USkeletalMeshComponent* Arms;
	//3인칭일때 메쉬
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USkeletalMeshComponent* ThirdPerson;
	
	//카메라 추가
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UCameraComponent* FirstCameraComp;

	//카메라 쉐이크 추가
	UPROPERTY(EditAnywhere,BlueprintReadWrite, category="CameraShake")
	TSubclassOf<class UCameraShakeBase> GunCameraShake;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, category="CameraShake")
	TSubclassOf<class UCameraShakeBase> DamagedCameraShake;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, category="CameraShake")
	TSubclassOf<class UCameraShakeBase> SweepCameraShake;

	//카메라 전환 (1<->3)
	void SwitchCamera();
	UPROPERTY(visibleAnywhere)
	bool bFirstPerson;
	UPROPERTY(EditAnywhere)
	class UCameraComponent* ThirdPersonCameraComp;

	//기본
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	float CurrentHP;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	float MaxHP=100.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	bool bIsDamaged=false;
	
	//지금 화면에 피 애니메이션 실행중인지 체크하고
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	bool bIsAttacked=false;
	//몇번 공격했는지 체크하자
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	int32 AttackCount;
	
	//이동
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	float Speed=700.f;
	UPROPERTY()
	FVector Direction;
	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputMappingContext* IMC_Survivor;
	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* IA_SurMove;
	//이동 함수
	void SurvivorMove(const struct FInputActionValue& InputValue);

	//앉기
	UPROPERTY(EditAnywhere,Category="Input")
	class UInputAction* IA_SurCrouch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Settings")
	bool bIsCrouching;
	//앉기 함수
	void SurvivorCrouch(const struct FInputActionValue& InputValue);

	//회전
	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* IA_SurTurn;
	//회전 함수
	void SurvivorTurn(const struct FInputActionValue& InputValue);

	//위아래 회전
	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* IA_SurLook;
	//회전 함수
	void SurvivorLook(const struct FInputActionValue& InputValue);

	//점프
	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* IA_SurJump;
	//회전 함수
	void SurvivorJump(const struct FInputActionValue& InputValue);

	//총쏘기, 멈추기, 장전
	UPROPERTY(EditAnywhere,Category="Input")
	class UInputAction* IA_SurFire;
	UPROPERTY(EditAnywhere,Category="Input")
	class UInputAction* IA_SurReload;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Settings")
	bool bIsReloading;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Settings",meta=(ClampMin="0"));
	float FireDamage = 10.0f;

	//좌클릭, R키(장전) 바인딩
	void LeftClickAttack(const struct FInputActionValue& InputValue);
	void WeaponReload(const struct FInputActionValue& InputValue);
	void DecreaseAmmoCount();
	//좌클릭시 무기별 실행될 공격을 따로 함수로 구현하기
	void PrimaryWeaponAttack();
	void SecondaryWeaponAttack();
	void MeleeWeaponAttack();
	void HandleHoldAttack();
	void HandleReleaseAttack();
	void NoneAttack();
	//좌클릭 함수 추가
	void Sweep();
	FTimerHandle CylinderTimerHandle;
	UFUNCTION()
	void OnThrowWeaponHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	                      const FHitResult& Hit);
	void ThrowWeapon();
	FTimerHandle ExplosionTimerHandle;
	void ExplodeWeapon();
	//좀비들이 6초동안 파이프폭탄을 인지하게
	void PipeBombTraceZombies();
	FTimerHandle PipeBombTraceTimerHandle;
	
	UPROPERTY()
	bool bHasLanded=false;
	UPROPERTY()
	bool bIsHoldingLeft=false;
	UPROPERTY()
	float HoldTime;
	UPROPERTY()
	float MaxHoldTime = 5.0f;

	//TMap
	UPROPERTY(VisibleAnywhere)
	TMap<FName, int> BoneMap;
	//무기 TMap
	UPROPERTY(VisibleAnywhere)
	TMap<FName, int> WeaponMap;

	//우클릭 (밀쳐내기)
	UPROPERTY(EditAnywhere,Category="Input")
	class UInputAction* IA_SurRight;
	void RightClickAttack(const struct FInputActionValue& InputValue);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void OnDamaged(float Damage) override;
	virtual void OnDie() override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	bool bIsDead=false;
	
	//콜리전박스 생성 테스트
	UPROPERTY(EditAnywhere, Category = "Shove Attack")
	UStaticMesh* CylinderMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ShoveCollisionCylinder;
	UFUNCTION()
	void OnShoveOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                    const FHitResult& SweepResult);
	void spawnShoveCylinder();
	void DestroyShoveCylinder();

	//몽타주 재생 테스트
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimMontage* ShoveMontage;
	FTimerHandle ShoveTimerHandle;
	
	//무기 교체 키바인딩, 함수
	UPROPERTY(EditAnywhere,Category="Input")
	class UInputAction* IA_PrimaryWeapon;
	void EquipPrimaryWeapon(const struct FInputActionValue& InputValue);
	UPROPERTY(EditAnywhere,Category="Input")
	class UInputAction* IA_SecondaryWeapon;
	void EquipSecondaryWeapon(const struct FInputActionValue& InputValue);
	UPROPERTY(EditAnywhere,Category="Input")
	class UInputAction* IA_MeleeWeapon;
	void EquipMeleeWeapon(const struct FInputActionValue& InputValue);
	UPROPERTY(EditAnywhere,Category="Input")
	class UInputAction* IA_HandleObject;
	void EquipHandleObject(const struct FInputActionValue& InputValue);
	
	//무기 슬롯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData")
	FWeaponData PrimaryWeaponSlot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData")
	FWeaponData SecondaryWeaponSlot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData")
	FWeaponData MeleeWeaponSlot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData")
	FWeaponData HandleObjectSlot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData")
	TOptional<FWeaponData> CurrentWeaponSlot;

	// 현재 장착된 무기
	UPROPERTY(BlueprintReadOnly, Category = "WeaponData")
	// TObjectPtr<AWeaponBase> == AWeaponBase* (언리얼에서 관리하는거라서 전자가 더 좋다. 후자는 CPP전체)
	TObjectPtr<AWeaponBase> CurrentWeapon;
	// 현재 장착된 무기의 슬롯 인덱스를 반환하는 함수
	UFUNCTION(BlueprintCallable, Category = "WeaponData")
	int32 GetCurrentWeaponSlotIndex() const;

	//무기 발견 (trace 해서)
	UPROPERTY()
	AWeaponBase* FocusedWeapon;
	void TraceForWeapon(); //스피어트레이스하기 (라인트레이스는 정밀도가 필요해서 인식이 잘 안됨... 꼭 트레이스가 아니라 박스 콜리전으로 고쳐도됨)
	//라인트레이스 말고 오버랩으로 오버레이 해보기
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UBoxComponent* WeaponOverlapBox;
	UPROPERTY(EditAnywhere, Category = "Weapon")
	UMaterialInterface* OverlayMaterial;

	//아이템 발견 (trace)
	UPROPERTY()
	AItemBase* FocusedItem;
	
	// 무기 줍기 키입력 바인딩
	UPROPERTY(editanywhere, Category="Input")
	class UInputAction* IA_PickUp;
	void PickUpWeapon_Input(const FInputActionValue& Value);
	// 무기 줍기 함수
	void PickUpWeapon(FWeaponData NewWeapon);
	// 무기 교체 함수
	void EquipWeapon(FWeaponData* WeaponData);
	AWeaponBase* FindWeaponInWorld(FWeaponData* WeaponData);
	// 무기 내리기 함수
	void UnequipWeapon();
	
	// 무기 던지고 나서 판단하기
	UPROPERTY(editanywhere, Category="ThrownWeapon")
	bool bIsThrown;
 
	// 무기 내리는 몽타주
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* UnequipMontage;
	
};











