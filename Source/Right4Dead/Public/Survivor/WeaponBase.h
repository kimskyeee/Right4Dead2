// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EWeaponType.h"
#include "FWeaponData.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

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
	class USphereComponent* Root;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UStaticMeshComponent* PrimaryWeapon;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USceneComponent* Muzzle;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USceneComponent* Eject;

	// 무기 데이터 구조체
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Data")
	FWeaponData WeaponData;
	
	//함수 생성
	void LineTrace(FVector MuzzleLocation, FVector ImpactPoint,FRotator ProjectileRotation);

	//Event
	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void OnFire();
    UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void OnReload();

	//Weapon 변수
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	EWeaponType Name;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	float ReloadTime;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	float BulletSpread;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	float AmmoCount;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	float MaxAmmo;

	
	//SKYE: 월드에 소환된 무기(장착X)와 내가 장착하고 있는 무기를 구분하기 위한 설정 추가
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	bool IsEquipped;
	void SetEquipped(bool bEquip);
	
};
