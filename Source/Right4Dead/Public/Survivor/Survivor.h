// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Survivor.generated.h"

UCLASS()
class RIGHT4DEAD_API ASurvivor : public ACharacter
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
	
	//총쏘기함수
	void SurFire(const struct FInputActionValue& InputValue);
	void SurReload(const struct FInputActionValue& InputValue);

	void Fire();
	void StopFire();
	void Reload();

	//공격 전환
	UPROPERTY(EditDefaultsOnly,Category="Settings")
	int32 WeaponEquipped;

	//체력구현
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 200.f;
	
};




