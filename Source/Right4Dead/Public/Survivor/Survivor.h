// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase.h"
#include "EWeaponType.h"
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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//외관추가
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USkeletalMeshComponent* Head;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USkeletalMeshComponent* Arms;
	
	//카메라 추가
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* FirstCameraComp;

	//카메라 전환 (1<->3)
	void SwitchCamera();
	UPROPERTY(visibleAnywhere)
	bool bFirstPerson;
	UPROPERTY(EditAnywhere)
	class UCameraComponent* ThirdPersonCameraComp;

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
	void SurMove(const struct FInputActionValue& InputValue);

	//앉기
	UPROPERTY(EditAnywhere,Category="Input")
	class UInputAction* IA_SurCrouch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Settings")
	bool bIsCrouching;
	//앉기 함수
	void SurCrouch(const struct FInputActionValue& InputValue);

	//회전
	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* IA_SurTurn;
	//회전 함수
	void SurTurn(const struct FInputActionValue& InputValue);

	//위아래 회전
	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* IA_SurLook;
	//회전 함수
	void SurLook(const struct FInputActionValue& InputValue);

	//점프
	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* IA_SurJump;
	//회전 함수
	void SurJump(const struct FInputActionValue& InputValue);

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
	//총쏘기함수
	void SurFire(const struct FInputActionValue& InputValue);
	void SurReload(const struct FInputActionValue& InputValue);

	//우클릭 (밀쳐내기)
	UPROPERTY(EditAnywhere,Category="Input")
	class UInputAction* IA_SurRight;
	void SurRight(const struct FInputActionValue& InputValue);

	//UI
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> MainUIFactory;
	UPROPERTY(EditAnywhere)
	class UUISurvivorMain* MainUI;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Stats")
	class UStatSystem* StatSystem;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void OnDamaged(float Damage) override;
	virtual void OnDie() override;

	//일단 시험용 빠루 소환
	UFUNCTION()
	void TempMontageStarted(UAnimMontage* Montage);
	UFUNCTION()
	void TempMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	//void CrowLinetrace();


	//콜리전박스 생성 테스트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* PushCollisionBox;
	void spawnCollisionBox();

	//몽타주 재생 테스트
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimMontage* CrowMontage;
	FTimerHandle CrowTimerHandle;
	
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

	// 현재 장착된 무기
	UPROPERTY(BlueprintReadOnly, Category = "WeaponData")
	// TObjectPtr<AWeaponBase> == AWeaponBase* (언리얼에서 관리하는거라서 전자가 더 좋다. 후자는 CPP전체)
	TObjectPtr<AWeaponBase> CurrentWeapon;

	//무기 발견 (trace 해서)
	UPROPERTY()
	AWeaponBase* FocusedWeapon;
	void TraceForWeapon(); //스피어트레이스하기 (라인트레이스는 너무정밀함)

	// 무기 줍기 키입력 바인딩
	UPROPERTY(editanywhere, Category="Input")
	class UInputAction* IA_PickUp;
	void PickUpWeapon_Input(const FInputActionValue& Value);
	// 무기 줍기 함수
	void PickUpWeapon(FWeaponData NewWeapon);
	// 무기 교체 함수
	void EquipWeapon(FWeaponData* WeaponData);
	// 무기 내리기 함수
	void UnequipWeapon();
 
	// 무기 내리는 몽타주
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* UnequipMontage;
	
};











