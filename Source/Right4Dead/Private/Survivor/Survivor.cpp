// Fill out your copyright notice in the Description page of Project Settings.


#include "Survivor.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "EWeaponType.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "StatSystem.h"
#include "UISurvivorMain.h"
#include "WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "Right4Dead/Right4Dead.h"

// Sets default values
ASurvivor::ASurvivor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Head=CreateDefaultSubobject<USkeletalMeshComponent>("Head");
	Head->SetupAttachment(GetMesh());

	//기본 카메라 설정
	FirstCameraComp=CreateDefaultSubobject<UCameraComponent>(TEXT("FirstCameraComp"));
	FirstCameraComp->SetupAttachment(Head);
	FirstCameraComp->SetRelativeLocationAndRotation(FVector(0,0,160),FRotator(0,90,0));
	FirstCameraComp->bUsePawnControlRotation = true;
	
	SpringArmComp=CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocation(FVector(0,0,70));
	SpringArmComp->TargetArmLength = 300.f;
	SpringArmComp->bUsePawnControlRotation = true;
	
	ThirdPersonCameraComp=CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdCameraComp"));
	ThirdPersonCameraComp->SetupAttachment(SpringArmComp);

	Arms=CreateDefaultSubobject<USkeletalMeshComponent>("Arms");
	Arms->SetupAttachment(FirstCameraComp);
	
	bFirstPerson = true;

	//시험용 빠루
	WeaponMesh=CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMeshComp");
	WeaponMesh->SetupAttachment(Arms, TEXT("WeaponSocket"));
	
	/*ConstructorHelpers::FObjectFinder<USkeletalMesh> TempCrow(TEXT("/Script/Engine.SkeletalMesh'/Game/Fab/Crowbar_Low-poly/SKM_crowbar1.SKM_crowbar1'"));
	if (TempCrow.Succeeded())
	{
		WeaponMesh->SetSkeletalMesh(TempCrow.Object);
	}*/

	//몽타주 연동
	ConstructorHelpers::FObjectFinder<UAnimMontage> TempCrowMontage(TEXT("/Script/Engine.AnimMontage'/Game/UltimateFPSAnimationsKIT/Animations/Arms_Montages/knife_arms_Swing_1_Montage.knife_arms_Swing_1_Montage'"));
	if (TempCrowMontage.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("몽타주 연동됨"));
		CrowMontage = TempCrowMontage.Object;
	}
		

	//Input데이터 할당하기
	//이동관련
	ConstructorHelpers::FObjectFinder<UInputMappingContext> TempIMC(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Survivor.IMC_Survivor'"));
	if (TempIMC.Succeeded())
	{
		IMC_Survivor=TempIMC.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempIAMove(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_SurMove.IA_SurMove'"));
	if (TempIAMove.Succeeded())
	{
		IA_SurMove=TempIAMove.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempIATurn(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_SurTurn.IA_SurTurn'"));
	if (TempIATurn.Succeeded())
	{
		IA_SurTurn=TempIATurn.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempIALook(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_SurLook.IA_SurLook'"));
	if (TempIALook.Succeeded())
	{
		IA_SurLook=TempIALook.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempIAJump(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_SurJump.IA_SurJump'"));
	if (TempIALook.Succeeded())
	{
		IA_SurJump=TempIAJump.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempIACrouch(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_SurCrouch.IA_SurCrouch'"));
	if (TempIACrouch.Succeeded())
	{
		IA_SurCrouch=TempIACrouch.Object;
	}

	//공격관련
	ConstructorHelpers::FObjectFinder<UInputAction> TempIAFire(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_SurFire.IA_SurFire'"));
	if (TempIAFire.Succeeded())
	{
		IA_SurFire=TempIAFire.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempIARelaod(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_SurReload.IA_SurReload'"));
	if (TempIARelaod.Succeeded())
	{
		IA_SurReload=TempIARelaod.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempIARight(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_SurRight.IA_SurRight'"));
	if (TempIARight.Succeeded())
	{
		IA_SurRight=TempIARight.Object;
	}

	//무기전환
	ConstructorHelpers::FObjectFinder<UInputAction> TempIAPrimary(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Weapon1.IA_Weapon1'"));
	if (TempIAPrimary.Succeeded())
	{
		IA_PrimaryWeapon=TempIAPrimary.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempIASecondary(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Weapon2.IA_Weapon2'"));
	if (TempIASecondary.Succeeded())
	{
		IA_SecondaryWeapon=TempIASecondary.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempIAMelee(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Weapon3.IA_Weapon3'"));
	if (TempIAMelee.Succeeded())
	{
		IA_MeleeWeapon=TempIAMelee.Object;
	}

	//무기줍기
	ConstructorHelpers::FObjectFinder<UInputAction> TempIAPickUp(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_SurPickUp.IA_SurPickUp'"));
	if (TempIAPickUp.Succeeded())
	{
		IA_PickUp=TempIAPickUp.Object;
	}
	
	//스탯
	StatSystem = CreateDefaultSubobject<UStatSystem>(TEXT("StatSystem"));
	
}

// Called when the game starts or when spawned
void ASurvivor::BeginPlay()
{
	Super::BeginPlay();
	
	//Enhanced Input
	auto* pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		auto subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subSystem)
		{
			subSystem->AddMappingContext(IMC_Survivor,0);
		}
	}

	//카메라 설정
	FirstCameraComp->SetActive(true);
	ThirdPersonCameraComp->SetActive(false);
	SpringArmComp->SetActive(false);

	/*//UI로드
	MainUI=Cast<UUISurvivorMain>(CreateWidget(GetWorld(), MainUIFactory));
	check(MainUI);
	if (MainUI)
	{
		MainUI->AddToViewport();
	}*/
}

// Called every frame
void ASurvivor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//direction을 캐릭터가 바라보는 방향으로 변환
	Direction=FTransform(GetControlRotation()).TransformVector(Direction);

	//이동 : 캐릭터 무브먼트 이용
	AddMovementInput(Direction);
	Direction=FVector::Zero();

	TraceForWeapon();

}

// Called to bind functionality to input
void ASurvivor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto pi=CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (pi)
	{
		pi->BindAction(IA_SurMove, ETriggerEvent::Triggered, this, &ASurvivor::SurMove);
		pi->BindAction(IA_SurLook, ETriggerEvent::Triggered, this, &ASurvivor::SurLook);
		pi->BindAction(IA_SurTurn, ETriggerEvent::Triggered, this, &ASurvivor::SurTurn);
		pi->BindAction(IA_SurJump, ETriggerEvent::Started, this, &ASurvivor::SurJump);
		pi->BindAction(IA_SurCrouch, ETriggerEvent::Started, this, &ASurvivor::SurCrouch);
		pi->BindAction(IA_SurFire, ETriggerEvent::Started, this, &ASurvivor::SurFire);
		pi->BindAction(IA_SurRight, ETriggerEvent::Started, this, &ASurvivor::SurRight);
		pi->BindAction(IA_PrimaryWeapon, ETriggerEvent::Started, this, &ASurvivor::EquipPrimaryWeapon);
		pi->BindAction(IA_SecondaryWeapon, ETriggerEvent::Started, this, &ASurvivor::EquipSecondaryWeapon);
		pi->BindAction(IA_MeleeWeapon, ETriggerEvent::Started, this, &ASurvivor::EquipMeleeWeapon);
		pi->BindAction(IA_PickUp,ETriggerEvent::Started,this,&ASurvivor::PickUpWeapon_Input);
	}
}

//카메라 전환 함수 
void ASurvivor::SwitchCamera()
{
	bFirstPerson=!bFirstPerson;
	if (bFirstPerson)
	{
		FirstCameraComp->SetActive(true);
		ThirdPersonCameraComp->SetActive(false);
		SpringArmComp->SetActive(false);
	}
	else
	{
		FirstCameraComp->SetActive(false);
		ThirdPersonCameraComp->SetActive(true);
		SpringArmComp->SetActive(true);
	}
}

void ASurvivor::SurMove(const struct FInputActionValue& InputValue)
{
	FVector2d dir=InputValue.Get<FVector2d>();
	Direction.X = dir.Y;
	Direction.Y = dir.X;
}

void ASurvivor::SurCrouch(const struct FInputActionValue& InputValue)
{
	Crouch();
}

void ASurvivor::SurTurn(const struct FInputActionValue& InputValue)
{
	float value = InputValue.Get<float>();
	AddControllerYawInput(value);
}

void ASurvivor::SurLook(const struct FInputActionValue& InputValue)
{
	float value = InputValue.Get<float>();
	AddControllerPitchInput(value);
}

void ASurvivor::SurJump(const struct FInputActionValue& InputValue)
{
	Jump();
}

void ASurvivor::SurFire(const struct FInputActionValue& InputValue)
{
	UE_LOG(LogTemp, Warning, TEXT("SurFire 함수 호출됨"));
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	APlayerCameraManager* FirstCam = UGameplayStatics::GetPlayerCameraManager(GetWorld(),0);
	if (!FirstCam)
	{
		UE_LOG(LogTemp, Error, TEXT("카메라 매니저가 없음"));
		return;
	}
	FVector Start = FirstCam->GetCameraLocation();
	FVector End = Start + (FirstCam->GetActorForwardVector() * 30000);

	UE_LOG(LogTemp, Warning, TEXT("라인트레이스 시작 위치: %s"), *Start.ToString());
	UE_LOG(LogTemp, Warning, TEXT("라인트레이스 끝 위치: %s"), *End.ToString());
    
	const float DebugLineLifetime = 2.0f;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Visibility, Params);

	UE_LOG(LogTemp, Warning, TEXT("라인트레이스 실행됨: %s"), bHit ? TEXT("히트") : TEXT("미스"));
	
	// 디버그 라인 그리기
	if (bDrawLine)
	{
		if (bHit)
		{
			// 히트가 발생한 경우 빨간색으로 표시
			DrawDebugLine(GetWorld(), Start, Hit.Location, FColor::Red, false, DebugLineLifetime, 0, 0.5f);
		}
		else
		{
			// 히트가 없는 경우 초록색으로 표시
			DrawDebugLine(GetWorld(),Start,End,FColor::Green,false,DebugLineLifetime, 0,0.5f);
		}
	}
    
	if (bHit && Hit.GetActor())
	{
		UGameplayStatics::ApplyDamage(Hit.GetActor(),FireDamage,GetController(),this,UDamageType::StaticClass());
	}
}

void ASurvivor::SurReload(const struct FInputActionValue& InputValue)
{
}


// 공격 : 밀쳐내기
void ASurvivor::SurRight(const struct FInputActionValue& InputValue)
{
	UE_LOG(LogTemp, Warning, TEXT("SurRight 함수 호출"));
	
	if (UAnimInstance* AnimInstance = Arms->GetAnimInstance())
	{
		UE_LOG(LogTemp, Warning, TEXT("뭐가없음"));
		AnimInstance->OnMontageStarted.AddDynamic(this, &ASurvivor::TempMontageStarted);
		AnimInstance->OnMontageEnded.AddDynamic(this, &ASurvivor::TempMontageEnded);
		AnimInstance->Montage_Play(CrowMontage);
	}
}

float ASurvivor::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                            class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	OnDamaged(DamageAmount);
	return DamageAmount;
}

void ASurvivor::OnDamaged(float Damage)
{
	//체력깎기
	float CurrentHealth = StatSystem->Health;
	CurrentHealth -= Damage;
	//0되면 ondie호출하기
	if (CurrentHealth <= 0)
	{
		OnDie();
	}
	
}

void ASurvivor::OnDie()
{
	StatSystem->bIsDead = true;
}

void ASurvivor::TempMontageStarted(UAnimMontage* Montage)
{
	UE_LOG(LogTemp, Warning, TEXT("몽타주시작"));
	GetWorld()->GetTimerManager().SetTimer(CrowTimerHandle, this, &ASurvivor::CrowLinetrace, GetWorld()->GetDeltaSeconds(), true);
	
	UAnimInstance* AnimInstance = Arms->GetAnimInstance();
	AnimInstance->OnMontageStarted.RemoveDynamic(this, &ASurvivor::TempMontageStarted);
}

void ASurvivor::TempMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG(LogTemp, Warning, TEXT("몽타주끝"));
	GetWorld()->GetTimerManager().ClearTimer(CrowTimerHandle);
	
	UAnimInstance* AnimInstance = Arms->GetAnimInstance();
	AnimInstance->OnMontageEnded.RemoveDynamic(this, &ASurvivor::TempMontageEnded);
}

void ASurvivor::CrowLinetrace()
{
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	/*FVector Start = CrowMeshComp->GetSocketLocation(TEXT("Start"));
	FVector End = CrowMeshComp->GetSocketLocation(TEXT("End"));

	UE_LOG(LogTemp, Warning, TEXT("라인트레이스 시작 위치: %s"), *Start.ToString());
	UE_LOG(LogTemp, Warning, TEXT("라인트레이스 끝 위치: %s"), *End.ToString());
    
	const float DebugLineLifetime = 2.0f;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Visibility, Params);

	UE_LOG(LogTemp, Warning, TEXT("라인트레이스 실행됨: %s"), bHit ? TEXT("빠루히트") : TEXT("빠루미스"));
	
	// 디버그 라인 그리기
	if (bDrawLine)
	{
		if (bHit)
		{
			// 히트가 발생한 경우 빨간색으로 표시
			DrawDebugLine(GetWorld(), Start, Hit.Location, FColor::Red, false, DebugLineLifetime, 0, 0.5f);
		}
		else
		{
			// 히트가 없는 경우 초록색으로 표시
			DrawDebugLine(GetWorld(),Start,End,FColor::Green,false,DebugLineLifetime, 0,0.5f);
		}
	}
    
	if (bHit && Hit.GetActor())
	{
		UGameplayStatics::ApplyDamage(Hit.GetActor(),FireDamage,GetController(),this,UDamageType::StaticClass());
	}	*/
}

void ASurvivor::EquipPrimaryWeapon(const struct FInputActionValue& InputValue)
{
	if (PrimaryWeaponSlot.WeaponMesh)
	{
		EquipWeapon(&PrimaryWeaponSlot);
	}
}

void ASurvivor::EquipSecondaryWeapon(const struct FInputActionValue& InputValue)
{
	if (SecondaryWeaponSlot.WeaponMesh)
	{
		EquipWeapon(&SecondaryWeaponSlot);
	}
}

void ASurvivor::EquipMeleeWeapon(const struct FInputActionValue& InputValue)
{
	if (MeleeWeaponSlot.WeaponMesh)
	{
		EquipWeapon(&MeleeWeaponSlot);
	}
}

//무기 발견하기 (카메라 라인트레이스)
void ASurvivor::TraceForWeapon()
{
	FVector Start = FirstCameraComp->GetComponentLocation(); // 카메라 위치
	FVector ForwardVector = FirstCameraComp->GetForwardVector(); // 카메라의 정면 방향
	FVector End = Start + (ForwardVector * 1000.f); // 1000cm(10m) 앞까지 탐색

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // 자기 자신은 무시
	// FCollisionObjectQueryParams ObjectParams;
	// ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
	
	const float DebugLineLifetime = 2.0f;

	// 라인 트레이스 실행
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor Class: %s"), *HitActor->GetClass()->GetName());
		}
		
		AWeaponBase* HitWeapon = Cast<AWeaponBase>(HitResult.GetActor()); // 무기인지 확인

		if (HitWeapon)
		{
			FocusedWeapon = HitWeapon; // 감지한 무기를 저장
			UE_LOG(LogTemp, Warning, TEXT("현재 바라보는 무기: %s"), *FocusedWeapon->GetName());
			DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red, false, DebugLineLifetime, 0, 0.5f);
		}
		else
		{
			FocusedWeapon = nullptr; // 무기가 아니면 초기화
			UE_LOG(LogTemp, Warning, TEXT("무기가 아님"));
			DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red, false, DebugLineLifetime, 0, 0.5f);
		}
	}
	else
	{
		FocusedWeapon = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("아무것도 없음")); // 감지된 게 없으면 초기화
	}
}

//무기줍기 (E) 
void ASurvivor::PickUpWeapon_Input(const FInputActionValue& Value)
{
	if (FocusedWeapon) // 플레이어가 바라보고 있는 무기가 있다면
	{
		UE_LOG(LogTemp, Warning, TEXT("주울 무기 발견!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("주울 무기가 없습니다!"));
	}
}

void ASurvivor::PickUpWeapon(FWeaponData NewWeapon)
{
	switch (NewWeapon.WeaponName)
	{
	case EWeaponType::Primary:
		if (PrimaryWeaponSlot.WeaponMesh) // 이미 무기가 있다면 교체
		{
			UnequipWeapon();
		}
		PrimaryWeaponSlot = NewWeapon;
		EquipWeapon(&PrimaryWeaponSlot);
		break;

	case EWeaponType::Secondary:
		if (SecondaryWeaponSlot.WeaponMesh) // 이미 무기가 있다면 교체
		{
			UnequipWeapon();
		}
		SecondaryWeaponSlot = NewWeapon;
		EquipWeapon(&SecondaryWeaponSlot);
		break;

	case EWeaponType::Melee:
		if (MeleeWeaponSlot.WeaponMesh) // 이미 무기가 있다면 교체
		{
			UnequipWeapon();
		}
		MeleeWeaponSlot = NewWeapon;
		EquipWeapon(&MeleeWeaponSlot);
		break;

	default:
		UE_LOG(LogTemp, Warning, TEXT("무기 모르겠어용"));
		break;
	}
}

//무기 장착
void ASurvivor::EquipWeapon(FWeaponData* WeaponData)
{
	if (WeaponData && WeaponData->WeaponMesh)
	{
		// 현재 무기 내리기
		UnequipWeapon();

		// 새 무기 장착
		WeaponMesh->SetSkeletalMesh(WeaponData->WeaponMesh);
		if (WeaponData->WeaponMontage && GetMesh()->GetAnimInstance())
		{
			GetMesh()->GetAnimInstance()->Montage_Play(WeaponData->WeaponMontage);
		}

		// 현재 무기 업데이트
		CurrentWeapon = *WeaponData;  // 구조체 복사
		bHasWeapon = true;
	}
}

//무기 내리기
void ASurvivor::UnequipWeapon()
{
	if (bHasWeapon && UnequipMontage && GetMesh()->GetAnimInstance())
	{
		// 무기 내리는 몽타주 재생
		GetMesh()->GetAnimInstance()->Montage_Play(UnequipMontage);

		// 무기 메시 제거
		WeaponMesh->SetSkeletalMesh(nullptr);

		// 현재 무기 초기화
		CurrentWeapon = FWeaponData();  // 기본 값으로 초기화
		bHasWeapon = false;
	}
}





