// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EWeaponType.h"
#include "FWeaponData.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

struct FWeaponData;

UCLASS()
class RIGHT4DEAD_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//외관
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USkeletalMeshComponent* PrimaryWeapon;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USceneComponent* Muzzle;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USceneComponent* Eject;

	// 무기 데이터 구조체
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Data")
	FWeaponData WeaponData;
	
	//함수 생성
	void LineTrace(FVector MuzzleLocation, FVector ImpactPoint,FRotator ProjectileRotation);
	UFUNCTION(BlueprintCallable) //BP에서 구현하지 않고 cpp에서 구현한거 사용만 할때
	void HideAmmo();
	UFUNCTION(BlueprintCallable)
	void UnHideAmmo();

	//Event
	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void OnFire();
    UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void OnReload();

	//Recoil (반동) 변수
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Recoil")
	FRotator ControllerRot;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Recoil")
	FRotator PostRecoilRot;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Recoil")
	FRotator MaxRecoilRot;

	//Weapon 변수
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	float ReloadTime;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	float BulletSpread;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	float AmmoCount;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	float MaxAmmo;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	EWeaponType Name;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	bool bWeaponMode;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	bool bVariableMode;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	bool bISShotgun;
	
};
