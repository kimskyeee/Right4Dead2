// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase.h"

#include "GameFramework/Character.h"
#include "Item/ItemInfo.h"
#include "Survivor.generated.h"

class AWeaponBase;

UCLASS()
class RIGHT4DEAD_API ASurvivor : public ACharacter, public IActorBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASurvivor();

protected:
	UFUNCTION()
	void OnOverlapItem(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                     const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlapItem(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// 외관추가
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USkeletalMeshComponent* Head;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USkeletalMeshComponent* Arms;

	// 3인칭일때 메쉬
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USkeletalMeshComponent* ThirdPerson;
	
	// 카메라 추가
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UCameraComponent* FirstCameraComp;

	// 카메라 쉐이크 추가
	UPROPERTY(EditAnywhere,BlueprintReadWrite, category="CameraShake")
	TSubclassOf<class UCameraShakeBase> GunCameraShake;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, category="CameraShake")
	TSubclassOf<class UCameraShakeBase> DamagedCameraShake;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, category="CameraShake")
	TSubclassOf<class UCameraShakeBase> SweepCameraShake;

	// 슬롯 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USlotComponent* SlotComp;

public:
	// 디버그모드 온오프
	UPROPERTY(EditAnywhere, Category="DEBUG")
	bool bDebugPlay = false;

	//카메라 전환 (1<->3인칭)
	UFUNCTION()
	void SwitchCamera(const bool& bThirdPerson);
	UFUNCTION()
	bool IsThirdPersonCurrent() const { return bIsThirdPerson; }
	UPROPERTY(visibleAnywhere)
	bool bIsThirdPerson = false;
	
	UPROPERTY(EditAnywhere)
	class UCameraComponent* ThirdPersonCameraComp;

public:
	// UI 관련
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> MainUIFactory;
	UPROPERTY(EditAnywhere)
	class UUISurvivorMain* SurvivorMainUI;

	UPROPERTY()
	class UUISurvivorCrosshair* CrosshairUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<class UUITakeDamage> TakeDamageUIClass;
	UPROPERTY()
	class UUITakeDamage* TakeDamageUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<class UUIAttackZombie> AttackZombieUIClass;
	UPROPERTY()
	class UUIAttackZombie* AttackZombieUI;

	UPROPERTY()
	class UUISurvivorMedKit* MedKitUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<class UUISurvivorMedKit> MedKitUIClass;
	
	UPROPERTY()
	class UUISurvivorCokeDelivery* CokeDeliveryUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<class UUISurvivorCokeDelivery> CokeDeliveryUIClass;
	
	UPROPERTY()
	class UUISurvivorIndicator* AttackIndicatorUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<class UUISurvivorIndicator> AttackIndicatorUIClass;
	
	//기본
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	float CurrentHP;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	float MaxHP=100.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	bool bIsDamaged = false;

	// 이동
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	float Speed = 700.f;
	UPROPERTY()
	FVector Direction;

	//마우스 버튼을 꾹 누르고 있는지 판단하는 변수 추가
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	float CurrentLeftTime=0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	float ChangeLeftTime=0.5f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	bool bIsLeftPressed=false;

	// UI
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	bool bIsAttacked = false; // 지금 화면에 애니메이션이 실행중인가?
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	int32 AttackCount; // 몇번째 공격인가?

	// Input
	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputMappingContext* IMC_Survivor;

	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* IA_Move;
	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* IA_Crouch;
	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* IA_Turn;
	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* IA_Look;
	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* IA_Jump;

	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* IA_UseWeapon;
	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* IA_Drop;

	UPROPERTY(EditAnywhere,Category="Input")
	class UInputAction* IA_Reload;

	// 슬롯 전환
	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* IA_EquipSlot1;
	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* IA_EquipSlot2;
	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* IA_EquipSlot3;
	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* IA_EquipSlot4;

	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* IA_CycleNext;
	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* IA_ScrollSlotAxis;

	UPROPERTY(editanywhere, Category="Input")
	class UInputAction* IA_PickUp;

	UPROPERTY(EditAnywhere,Category="Input")
	class UInputAction* IA_RightClick; // 우클릭 (밀쳐내기)
	
	// Input 함수
	UFUNCTION() void SurvivorMove(const struct FInputActionValue& InputValue);
	UFUNCTION() void SurvivorCrouch(const struct FInputActionValue& InputValue);
	UFUNCTION() void SurvivorTurn(const struct FInputActionValue& InputValue);
	UFUNCTION() void SurvivorLook(const struct FInputActionValue& InputValue);
	UFUNCTION() void SurvivorJump(const struct FInputActionValue& InputValue);

	UFUNCTION() void PickUpWeapon(); // E 키
	UFUNCTION() void RightClickAttack(const struct FInputActionValue& InputValue); // 우클릭 (밀쳐내기)
	
	UFUNCTION() void OnUseStarted();
	UFUNCTION() void OnUseTriggered();
	UFUNCTION() void OnUseCompleted();
	UFUNCTION() void OnUseCanceled();
	
	UFUNCTION() void OnEquipSlot1();
	UFUNCTION() void OnEquipSlot2();
	UFUNCTION() void OnEquipSlot3();
	UFUNCTION() void OnEquipSlot4();

	UFUNCTION() void OnCycleNext(); 
	UFUNCTION() void OnSlotScroll(const FInputActionValue& V);

	// input 관련 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Settings")
	bool bIsCrouching;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void OnDamaged(float Damage) override;
	virtual void OnDie() override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	bool bIsDead=false;

	UPROPERTY(BlueprintReadWrite)
    bool bGod = false;

public:
	//사운드 재생
	class USoundWave* SwingMiss; //도끼휘두르기
	class USoundWave* SwingHitZombie; //도끼가 좀비를 때리면
	class USoundWave* SwingHitWorld; //도끼가 그냥 물체를 때리면
	class USoundWave* WeaponSwing; //무기 있을때 스윙
	class USoundCue* TakeDamageSound; //맞았을때 사운드

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundWave* CokeDeliverySound; //콜라배달 사운드
	UPROPERTY()
	UAudioComponent* ColaDeliveryAudio; 

	// 피해 인디케이터
	UFUNCTION()	void DisplayIndicator(AActor* Causer);

public:
	UPROPERTY()
	float ElapsedHold = 0.0f;

	UFUNCTION()
	EItemType GetCurrentItemType();

protected:
	//아이템 발견 (trace)
	UPROPERTY()
	TWeakObjectPtr<class AItemBase> FocusedPickup;

	UPROPERTY(EditAnywhere, Category = "Overlay")
	UMaterialInterface* OverlayMaterial;

	UFUNCTION() void TraceForPickup();
	UFUNCTION() void SetPickupFocus(class AItemBase* NewFocus);
	UFUNCTION() void ClearPickupFocus();
};
