// Fill out your copyright notice in the Description page of Project Settings.


#include "Survivor.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "StatSystem.h"
#include "UISurvivorMain.h"
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
	CrowMeshComp=CreateDefaultSubobject<USkeletalMeshComponent>("CrowMeshComp");
	CrowMeshComp->SetupAttachment(Arms, TEXT("WeaponSocket"));
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempCrow(TEXT("/Script/Engine.SkeletalMesh'/Game/Fab/Crowbar_Low-poly/SKM_crowbar1.SKM_crowbar1'"));
	if (TempCrow.Succeeded())
	{
		CrowMeshComp->SetSkeletalMesh(TempCrow.Object);
		CrowMeshComp->SetRelativeLocationAndRotation(FVector(-60,20,80),FRotator(-20,90,0));
	}

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

	//이동 : 캐릭터 무브먼트 이용
	AddMovementInput(Direction);
	Direction=FVector::Zero();

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

	FVector Start = CrowMeshComp->GetSocketLocation(TEXT("Start"));
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
	}	
}


