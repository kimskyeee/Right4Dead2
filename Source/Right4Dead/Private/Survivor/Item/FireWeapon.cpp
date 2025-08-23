// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/FireWeapon.h"

#include "BulletDamageType.h"
#include "Item/ItemSpec.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AFireWeapon::AFireWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFireWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFireWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFireWeapon::HandleUse(EUsingType Phase, float ElapsedHold)
{
	const bool bAuto = Spec && Spec->bAutoFireWhenHeld;
	
	if (Phase == EUsingType::Started)
	{
		// 한번 발사 (단발)
		bHolding = true;
		FireAttack();
		Accum = 0.f;
	}
	else if (Phase == EUsingType::Ongoing && bAuto && bHolding) 
	{
		// 지속 발사면 (연발)
		Accum += GetWorld()->GetDeltaSeconds();
		if (Accum >= FireCooldown)
		{
			FireAttack();
			Accum = 0.f;
		}
	}
	else if (Phase == EUsingType::Completed)
	{
		// 홀드 끝내기
		bHolding = false;
	}
}

void AFireWeapon::FireAttack()
{
	// 단발 기준 로직
	
	// 총알이 있어야....
	if (CurrentAmmo < 0) return;
	
	//총무기 라인트레이스
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	APlayerCameraManager* FirstCam = UGameplayStatics::GetPlayerCameraManager(GetWorld(),0);
	if (!FirstCam)
	{
		return;
	}
	
	FVector Start = FirstCam->GetCameraLocation();
	FVector End = Start + (FirstCam->GetActorForwardVector() * 30000);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_GameTraceChannel6, Params);
	    
	if (bHit && Hit.GetActor())
	{
		if (false == Hit.BoneName.IsNone())
		{
			const FVector HitFromDirection = (GetActorForwardVector() + FVector(0, 0, 0.5f)).GetSafeNormal();
			UGameplayStatics::ApplyPointDamage(Hit.GetActor(), 10, HitFromDirection, Hit, nullptr, nullptr, UBulletDamageType::StaticClass());
		}
	}

	//총알 개수 줄이자
	DecreaseAmmoCount();

	/*//몽타주 플레이
	if (CurrentWeapon->WeaponData.WeaponFireMontage)
	{
		Arms->GetAnimInstance()->Montage_Play(CurrentWeapon->WeaponData.WeaponFireMontage);
	}
	else
	{
		return;
	}

	//카메라 쉐이크
	auto PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->PlayerCameraManager->StartCameraShake(GunCameraShake);
	}*/
	
}

void AFireWeapon::DecreaseAmmoCount()
{
	if (CurrentAmmo > 0)
	{
		CurrentAmmo--;
	}
}

void AFireWeapon::Reload()
{
	// 총알이 아직 부족하고, 남은 탄약이 존재하는 경우에만 장전 가능
	if (CurrentAmmo < MaxAmmo && MaxAmmoAmount > 0)
	{
		// 현재 총기에 남아있는 탄약과 필요한 탄약 계산
		int32 NeededAmmo = MaxAmmo - CurrentAmmo;
		int32 AmmoToLoad = FMath::Min(NeededAmmo, MaxAmmoAmount);

		// 장전된 탄 수 반영
		CurrentAmmo += AmmoToLoad;
		MaxAmmoAmount -= AmmoToLoad;

		/*// 몽타주 플레이
		if (UAnimInstance* AnimInstance = Arms->GetAnimInstance())
		{
			AnimInstance->Montage_Play(CurrentWeapon->WeaponData.WeaponReloadMontage);
		}*/
	}
}

