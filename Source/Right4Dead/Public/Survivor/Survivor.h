// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase.h"
#include "FWeaponData.h"
#include "WeaponBase.h"
#include "GameFramework/Character.h"
#include "Survivor.generated.h"

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

	//UI붙이기 (Widget)
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> MainUIFactory;
	UPROPERTY(EditAnywhere)
	class UUISurvivorMain* SurvivorMainUI;

	//외관추가
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USkeletalMeshComponent* Head;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USkeletalMeshComponent* Arms;
	
	//카메라 추가
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UCameraComponent* FirstCameraComp;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Debug");
	bool bDrawLine = true;

	//좌클릭, R키(장전) 바인딩
	void LeftClickAttack(const struct FInputActionValue& InputValue);
	void WeaponReload(const struct FInputActionValue& InputValue);
	//좌클릭시 무기별 실행될 공격을 따로 함수로 구현하기
	void PrimaryWeaponAttack();
	void SecondaryWeaponAttack();
	void MeleeWeaponAttack();
	void NoneAttack();
	//좌클릭 함수 추가
	void Sweep();
	UFUNCTION()
	void OnThrowWeaponHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	                      const FHitResult& Hit);
	void ThrowWeapon();
	FTimerHandle ExplosionTimerHandle;
	void ExplodeWeapon();
	UPROPERTY()
	bool bHasLanded=false;

	//Tmap
	UPROPERTY(VisibleAnywhere)
	TMap<FName, int> BoneMap;

	//우클릭 (밀쳐내기)
	UPROPERTY(EditAnywhere,Category="Input")
	class UInputAction* IA_SurRight;
	void RightClickAttack(const struct FInputActionValue& InputValue);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void OnDamaged(float Damage) override;
	virtual void OnDie() override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	bool bIsDead=false;

	//일단 시험용 빠루 소환
	UFUNCTION()
	void TempMontageStarted(UAnimMontage* Montage);
	UFUNCTION()
	void TempMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	//void CrowLinetrace();

	//콜리전박스 생성 테스트
	UPROPERTY(EditAnywhere, Category = "Shove Attack")
	UStaticMesh* CylinderMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ShoveCollisionCylinder;
	UFUNCTION()
	void OnShoveOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                    const FHitResult& SweepResult);
	void spawnShoveCylinder();

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
	
	//무기 슬롯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData")
	FWeaponData PrimaryWeaponSlot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData")
	FWeaponData SecondaryWeaponSlot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData")
	FWeaponData MeleeWeaponSlot;
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











