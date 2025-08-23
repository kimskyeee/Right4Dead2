// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/FireWeapon.h"

#include "BulletDamageType.h"
#include "Survivor.h"
#include "SurvivorArmAnim.h"
#include "Item/ItemSpec.h"
#include "Kismet/GameplayStatics.h"


class USurvivorArmAnim;
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

void AFireWeapon::FireOnce()
{
	// 단발 기준 로직
	if (CurrentAmmo < 0) return;

	UE_LOG(LogTemp, Warning, TEXT("[FireOnce] this=%p &CurrentAmmo=%p Value=%d"),
	this, &CurrentAmmo, CurrentAmmo);
	
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

	UAnimInstance* AnimInst = Char->Arms->GetAnimInstance();
	if (!AnimInst) return;
			
	USurvivorArmAnim* ArmAnimInst = Cast<USurvivorArmAnim>(AnimInst);
	if (!ArmAnimInst) return;

	if (UAnimMontage* Montage = Montage_Use_Tap)
	{
		ArmAnimInst->Montage_Play(Montage);
	}

	//카메라 쉐이크
	auto PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->PlayerCameraManager->StartCameraShake(Char->GunCameraShake);
	}
}

void AFireWeapon::DecreaseAmmoCount()
{
	UE_LOG(LogTemp, Warning, TEXT("[Dec] this=%p &CurrentAmmo=%p Before=%d"),
		this, &CurrentAmmo, CurrentAmmo);
	
	if (CurrentAmmo > 0)
	{
		CurrentAmmo--;
	}

	UE_LOG(LogTemp, Warning, TEXT("[Dec] After=%d"), CurrentAmmo);
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

		UAnimInstance* AnimInst = Char->Arms->GetAnimInstance();
		if (!AnimInst) return;
			
		USurvivorArmAnim* ArmAnimInst = Cast<USurvivorArmAnim>(AnimInst);
		if (!ArmAnimInst) return;

		if (UAnimMontage* Montage = Montage_Reload)
		{
			ArmAnimInst->Montage_Play(Montage);
		}
	}
}

void AFireWeapon::OnUseStart()
{
	UE_LOG(LogTemp, Warning, TEXT("OnUseStart"));
	const float Now = GetWorld()->GetTimeSeconds();
	if (RateOfFire <= 0.f) return;
	if (Now < NextShotTime) return;
	if (CurrentAmmo <= 0)   return;

	FireOnce();
	NextShotTime = Now + 1.f / RateOfFire;
}

void AFireWeapon::OnTap(float Elapsed)
{

}

void AFireWeapon::OnHoldBegan()
{
	UE_LOG(LogTemp, Warning, TEXT("OnHoldBegan"));
	EnsureLoopMontage(Montage_Use_HoldLoop);
}

void AFireWeapon::OnHoldTick(float Elapsed)
{
	UE_LOG(LogTemp, Warning, TEXT("OnHoldTick Now=%.3f Next=%.3f Ammo=%d"),
	  Elapsed, NextShotTime, CurrentAmmo);
	
	const float Now = GetWorld()->GetTimeSeconds();
	if (RateOfFire <= 0.f) return;
	
	while (Now >= NextShotTime)
	{
		if (CurrentAmmo <= 0) return; // 탄약 없음
		FireOnce();
		NextShotTime += 1.f / RateOfFire;
	}
	EnsureLoopMontage(Montage_Use_HoldLoop);
}

void AFireWeapon::OnHoldReleased(float Elapsed)
{
	StopMontageIfPlaying(Montage_Use_HoldLoop);
	bFiredThisPress = false;
}

