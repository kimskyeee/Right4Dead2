// Fill out your copyright notice in the Description page of Project Settings.


#include "Survivor.h"

#include "CommonZombie.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "UIAttackZombie.h"
#include "UISurvivorCokeDelivery.h"
#include "UISurvivorCrosshair.h"
#include "UISurvivorIndicator.h"
#include "UISurvivorMain.h"
#include "UISurvivorMedKit.h"
#include "UITakeDamage.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Logging/LogTrace.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Interaction/InteractiveActor.h"
#include "Item/ItemBase.h"
#include "Item/SlotComponent.h"

// Sets default values
ASurvivor::ASurvivor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Head = CreateDefaultSubobject<USkeletalMeshComponent>("Head");
	Head->SetupAttachment(GetMesh());

	// 기본 카메라 설정
	FirstCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstCameraComp"));
	FirstCameraComp->SetupAttachment(Head);
	FirstCameraComp->SetRelativeLocationAndRotation(FVector(0,0,160),FRotator(0,90,0));
	FirstCameraComp->bUsePawnControlRotation = true;
	
	ThirdPerson = CreateDefaultSubobject<USkeletalMeshComponent>("ThirdPerson");
	ThirdPerson->SetupAttachment(RootComponent);
	ThirdPerson->SetVisibility(false);
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(ThirdPerson);
	SpringArmComp->SetRelativeLocation(FVector(0,0,200));
	SpringArmComp->TargetArmLength = 300.f;
	SpringArmComp->bUsePawnControlRotation = true;
	
	ThirdPersonCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdCameraComp"));
	ThirdPersonCameraComp->SetupAttachment(SpringArmComp);

	Arms = CreateDefaultSubobject<USkeletalMeshComponent>("Arms");
	Arms->SetupAttachment(FirstCameraComp);
	Arms->SetRelativeLocationAndRotation(FVector(-5,0,-170), FRotator(0,-90,0));

	SlotComp = CreateDefaultSubobject<USlotComponent>(TEXT("SlotComp"));
	
	bIsThirdPerson = false; // 시작은 1인칭

	// Input데이터 할당하기
	ConstructorHelpers::FObjectFinder<UInputMappingContext> TempIMC(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Survivor.IMC_Survivor'"));
	if (TempIMC.Succeeded())
	{
		IMC_Survivor = TempIMC.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempIAMove(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_SurMove.IA_SurMove'"));
	if (TempIAMove.Succeeded())
	{
		IA_Move = TempIAMove.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempIATurn(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_SurTurn.IA_SurTurn'"));
	if (TempIATurn.Succeeded())
	{
		IA_Turn = TempIATurn.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempIALook(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_SurLook.IA_SurLook'"));
	if (TempIALook.Succeeded())
	{
		IA_Look = TempIALook.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempIAJump(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_SurJump.IA_SurJump'"));
	if (TempIALook.Succeeded())
	{
		IA_Jump = TempIAJump.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempIACrouch(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_SurCrouch.IA_SurCrouch'"));
	if (TempIACrouch.Succeeded())
	{
		IA_Crouch = TempIACrouch.Object;
	}
	
	ConstructorHelpers::FObjectFinder<UInputAction> TempIARelaod(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_SurReload.IA_SurReload'"));
	if (TempIARelaod.Succeeded())
	{
		IA_Reload = TempIARelaod.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempIARight(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_SurRight.IA_SurRight'"));
	if (TempIARight.Succeeded())
	{
		IA_RightClick = TempIARight.Object;
	}
	
	ConstructorHelpers::FObjectFinder<UInputAction> TempIAPickUp(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_SurPickUp.IA_SurPickUp'"));
	if (TempIAPickUp.Succeeded())
	{
		IA_PickUp = TempIAPickUp.Object;
	}

	// 사운드 재생
	const ConstructorHelpers::FObjectFinder<USoundWave>TempSwingMiss(TEXT("/Script/Engine.SoundWave'/Game/Assets/Sounds/WeaponNAttack/swing_miss1.swing_miss1'"));
	if (TempSwingMiss.Succeeded())
	{
		SwingMiss = TempSwingMiss.Object;
	}
	const ConstructorHelpers::FObjectFinder<USoundWave>TempSwingHitZombie(TEXT("/Script/Engine.SoundWave'/Game/Assets/Sounds/WeaponNAttack/Melee/Axe/axe_03.axe_03'"));
	if (TempSwingHitZombie.Succeeded())
	{
		SwingHitZombie = TempSwingHitZombie.Object;
	}
	const ConstructorHelpers::FObjectFinder<USoundWave>TempSwingHitWorld(TEXT("/Script/Engine.SoundWave'/Game/Assets/Sounds/WeaponNAttack/Melee/melee_swing_hit_world.melee_swing_hit_world'"));
	if (TempSwingHitWorld.Succeeded())
	{
		SwingHitZombie = TempSwingHitWorld.Object;
	}

	const ConstructorHelpers::FObjectFinder<USoundWave>TempWeaponSwing(TEXT("/Script/Engine.SoundWave'/Game/Assets/Sounds/WeaponNAttack/swish_weaponswing_swipe5.swish_weaponswing_swipe5'"));
	if (TempWeaponSwing.Succeeded())
	{
		WeaponSwing = TempWeaponSwing.Object;
	}

	const ConstructorHelpers::FObjectFinder<USoundCue>TempTakeDamage(TEXT("/Script/Engine.SoundCue'/Game/Assets/Sounds/Survivor/PlayerHurt.PlayerHurt'"));
	if (TempTakeDamage.Succeeded())
	{
		TakeDamageSound = TempTakeDamage.Object;
	}

	//콜라배달 사운드
	ColaDeliveryAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("ColaDeliveryAudio"));
	ColaDeliveryAudio->bAutoActivate = false; // 자동 실행 방지코드라고 함
	ColaDeliveryAudio->SetupAttachment(RootComponent);
	
	const ConstructorHelpers::FObjectFinder<USoundWave>TempCoke(TEXT("/Script/Engine.SoundWave'/Game/Assets/Sounds/Item/Coke/attach_cola_bottles_01.attach_cola_bottles_01'"));
	if (TempCoke.Succeeded())
	{
		CokeDeliverySound = TempCoke.Object;
	}
	
	if (CokeDeliverySound)
	{
		ColaDeliveryAudio->SetSound(CokeDeliverySound);
	}
}

void ASurvivor::OnOverlapItem(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	// 상호작용 아이템이 오버랩 되면, 테두리 생성
	if (AInteractiveActor* InteractiveActor = Cast<AInteractiveActor>(OtherActor))
	{
		InteractiveActor->SetOverlayMaterial();
		
	}
}

void ASurvivor::OnEndOverlapItem(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 해제 되면 테두리 해제
	if (AInteractiveActor* InteractiveActor = Cast<AInteractiveActor>(OtherActor))
	{
		InteractiveActor->ClearOverlayMaterial();
	}
}

void ASurvivor::BeginPlay()
{
	Super::BeginPlay();
	
	// Enhanced Input
	auto* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(IMC_Survivor,0);
		}
	}

	//UI붙이기
	SurvivorMainUI=Cast<UUISurvivorMain>(CreateWidget(GetWorld(),MainUIFactory));
	if (SurvivorMainUI)
	{
		SurvivorMainUI->AddToViewport();
		CurrentHP=MaxHP;
		CrosshairUI = Cast<UUISurvivorCrosshair>(SurvivorMainUI->Crosshair);
	}
	if (TakeDamageUIClass) // UPROPERTY로 설정된 UClass<UUserWidget> 변수
	{
		TakeDamageUI = CreateWidget<UUITakeDamage>(GetWorld(), TakeDamageUIClass);
		if (TakeDamageUI)
		{
			TakeDamageUI->AddToViewport();
		}
	}
	if (AttackZombieUIClass)
	{
		AttackZombieUI = CreateWidget<UUIAttackZombie>(GetWorld(), AttackZombieUIClass);
		if (AttackZombieUI)
		{
			AttackZombieUI->AddToViewport();
		}
	}
	if (MedKitUIClass)
	{
		MedKitUI = CreateWidget<UUISurvivorMedKit>(GetWorld(), MedKitUIClass);
		if (MedKitUI)
		{
			MedKitUI->SetVisibility(ESlateVisibility::Hidden);
			MedKitUI->AddToViewport();
		}
	}
	if (CokeDeliveryUIClass)
	{
		CokeDeliveryUI = CreateWidget<UUISurvivorCokeDelivery>(GetWorld(), CokeDeliveryUIClass);
		if (CokeDeliveryUI)
		{
			CokeDeliveryUI->SetVisibility(ESlateVisibility::Hidden);
			CokeDeliveryUI->AddToViewport();
		}
	}
		
	//카메라 설정
	FirstCameraComp->SetActive(true);
	ThirdPersonCameraComp->SetActive(false);
	SpringArmComp->SetActive(false);
}

// Called every frame
void ASurvivor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	Direction = FTransform(GetControlRotation()).TransformVector(Direction);
	AddMovementInput(Direction);
	Direction = FVector::Zero();

	// 트레이스 
	TraceForPickup();

	// 조준선 UI 업데이트
	float Value = UKismetMathLibrary::VSize(GetVelocity());
	if (CrosshairUI)
	{
		CrosshairUI->CrosshairSpread = UKismetMathLibrary::MapRangeClamped(Value,0,450,5,70);
	}
}

// Called to bind functionality to input
void ASurvivor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto EIC=CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EIC)
	{
		EIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ASurvivor::SurvivorMove);
		EIC->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ASurvivor::SurvivorLook);
		EIC->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &ASurvivor::SurvivorTurn);
		EIC->BindAction(IA_Jump, ETriggerEvent::Started, this, &ASurvivor::SurvivorJump);
		EIC->BindAction(IA_Crouch, ETriggerEvent::Started, this, &ASurvivor::SurvivorCrouch);
		
		EIC->BindAction(IA_PickUp, ETriggerEvent::Started, this, &ASurvivor::PickUpWeapon);
		
		EIC->BindAction(IA_UseWeapon, ETriggerEvent::Started, this, &ASurvivor::OnUseStarted);
		EIC->BindAction(IA_UseWeapon, ETriggerEvent::Triggered, this, &ASurvivor::OnUseTriggered);
		EIC->BindAction(IA_UseWeapon, ETriggerEvent::Completed, this, &ASurvivor::OnUseCompleted);
		EIC->BindAction(IA_UseWeapon, ETriggerEvent::Canceled, this, &ASurvivor::OnUseCanceled);
		
		EIC->BindAction(IA_RightClick, ETriggerEvent::Started, this, &ASurvivor::RightClickAttack);

		EIC->BindAction(IA_EquipSlot1, ETriggerEvent::Started, this, &ASurvivor::OnEquipSlot1);
		EIC->BindAction(IA_EquipSlot2, ETriggerEvent::Started, this, &ASurvivor::OnEquipSlot2);
		EIC->BindAction(IA_EquipSlot3, ETriggerEvent::Started, this, &ASurvivor::OnEquipSlot3);
		EIC->BindAction(IA_EquipSlot4, ETriggerEvent::Started, this, &ASurvivor::OnEquipSlot4);

		EIC->BindAction(IA_CycleNext, ETriggerEvent::Started, this, &ASurvivor::OnCycleNext);
		
		EIC->BindAction(IA_ScrollSlotAxis, ETriggerEvent::Triggered, this, &ASurvivor::OnSlotScroll);
	}
}

//카메라 전환 함수 
void ASurvivor::SwitchCamera(const bool& bThirdPerson)
{
	bIsThirdPerson = bThirdPerson;
	
	if (!bThirdPerson)
	{
		// 1인칭이면
		FirstCameraComp->SetActive(true);
		ThirdPersonCameraComp->SetActive(false);
		SpringArmComp->SetActive(false);
		Arms->SetVisibility(true);
		SlotComp->CurrentInHands->SetHidden(false);
		ThirdPerson->SetVisibility(false);
	}
	else
	{
		// 3인칭이면
		FirstCameraComp->SetActive(false);
		ThirdPersonCameraComp->SetActive(true);
		SpringArmComp->SetActive(true);
		Arms->SetVisibility(false);
		SlotComp->CurrentInHands->SetHidden(true);
		ThirdPerson->SetVisibility(true);
	}
}

void ASurvivor::SurvivorMove(const struct FInputActionValue& InputValue)
{
	FVector2d dir=InputValue.Get<FVector2d>();
	Direction.X = dir.Y;
	Direction.Y = dir.X;
}

void ASurvivor::SurvivorCrouch(const struct FInputActionValue& InputValue)
{
	Crouch();
}

void ASurvivor::SurvivorTurn(const struct FInputActionValue& InputValue)
{
	float value = InputValue.Get<float>();
	AddControllerYawInput(value);
}

void ASurvivor::SurvivorLook(const struct FInputActionValue& InputValue)
{
	float value = InputValue.Get<float>();
	AddControllerPitchInput(value);
}

void ASurvivor::SurvivorJump(const struct FInputActionValue& InputValue)
{
	Jump();
}

void ASurvivor::PickUpWeapon()
{
	if (!SlotComp) return;

	if (FocusedPickup.IsValid())
	{
		if (SlotComp->TryPickup(FocusedPickup.Get()))
		{
			// 성공 → 포커스 비움
			ClearPickupFocus();
		}
	}
}

float ASurvivor::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                            class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	OnDamaged(DamageAmount);
	if (false == bGod)
	{
		DisplayIndicator(DamageCauser);
	}
	return DamageAmount;
}

void ASurvivor::OnDamaged(float Damage)
{
	if (bGod) return;
	
	bIsDamaged=true;
	
	auto pc = GetWorld()->GetFirstPlayerController();
	if (pc)
	{
		pc->PlayerCameraManager->StartCameraShake(DamagedCameraShake);
	}
	// 위젯 애니메이션 재생
	if (TakeDamageUI)
	{
		TakeDamageUI->PlayAnimationByName();
	}
	//체력깎기
	CurrentHP -= Damage;
	UGameplayStatics::PlaySound2D(this, TakeDamageSound, 1, 1);
	
	if (CurrentHP <= 0)
	{
		CurrentHP = 0;
		OnDie();
	}
}

void ASurvivor::OnDie()
{
	bIsDead = true;
}

void ASurvivor::RightClickAttack(const struct FInputActionValue& InputValue)
{
	if (!SlotComp->CurrentInHands.IsValid())
	
	if (SlotComp)
	{
		SlotComp->RightClickUse();
		// 무기 장착하면 짤그락
		UGameplayStatics::PlaySound2D(this, WeaponSwing, 1, 1);
	}
}

void ASurvivor::DisplayIndicator(AActor* Causer)
{
	if (AttackIndicatorUIClass)
	{
		AttackIndicatorUI = CreateWidget<UUISurvivorIndicator>(GetWorld(), AttackIndicatorUIClass);
		if (AttackIndicatorUI)
		{
			AttackIndicatorUI->AddToViewport();
		}
	}
	
	AttackIndicatorUI->HitLocation = Causer->GetActorLocation();
}

void ASurvivor::TraceForPickup()
{
	FVector Start = FirstCameraComp->GetComponentLocation(); // 카메라 위치
	FVector ForwardVector = FirstCameraComp->GetForwardVector(); // 카메라의 정면 방향
	FVector End = Start + (ForwardVector * 300.f); // 100cm(1m) 앞까지 탐색

	const float CapsuleRadius = 30.0f; // 캡슐의 반지름 설정
	const float CapsuleHalfHeight = 50.0f; // 캡슐의 반 높이 설정
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));
	
	TArray<AActor*> ActorsToIgnore;
	if (SlotComp && SlotComp->CurrentInHands.IsValid())
	{
		// 현재 들고 있는 무기면 무기
		ActorsToIgnore.Add(SlotComp->CurrentInHands.Get());
	}

	FHitResult Hit;
	const bool bHit = UKismetSystemLibrary::CapsuleTraceSingleForObjects(
		GetWorld(),	Start, End,
		CapsuleRadius, CapsuleHalfHeight, ObjectTypes,
		false, ActorsToIgnore, EDrawDebugTrace::None,
		Hit, true
	);

	AItemBase* NewTrace = (bHit ? Cast<AItemBase>(Hit.GetActor()) : nullptr);
	SetPickupFocus(NewTrace);
}

void ASurvivor::SetPickupFocus(AItemBase* NewFocus)
{
	if (FocusedPickup.Get() == NewFocus) return;

	if (FocusedPickup.IsValid())
	{
		FocusedPickup = nullptr;
	}

	if (NewFocus)
	{
		FocusedPickup = NewFocus;
	}
}

void ASurvivor::ClearPickupFocus()
{
	if (FocusedPickup.IsValid())
	{
		FocusedPickup = nullptr;
	}
}

void ASurvivor::OnUseStarted()
{
	ElapsedHold = 0.f;
	SlotComp->HandleUse(EUsingType::Started, ElapsedHold);
}

void ASurvivor::OnUseTriggered()
{
	ElapsedHold += GetWorld()->GetDeltaSeconds();
	SlotComp->HandleUse(EUsingType::Ongoing, ElapsedHold);
}

void ASurvivor::OnUseCompleted()
{
	SlotComp->HandleUse(EUsingType::Completed, ElapsedHold);
}

void ASurvivor::OnUseCanceled()
{
	SlotComp->HandleUse(EUsingType::Canceled, ElapsedHold);
	ElapsedHold = 0.f;
}

void ASurvivor::OnEquipSlot1()
{
	if (SlotComp)
	{
		SlotComp->EquipSlot(ESlotType::PrimarySlot);
	}
}

void ASurvivor::OnEquipSlot2()
{
	if (SlotComp)
	{
		SlotComp->EquipSlot(ESlotType::SecondarySlot);
	}
}

void ASurvivor::OnEquipSlot3()
{
	if (SlotComp)
	{
		SlotComp->EquipSlot(ESlotType::ThrownSlot);
	}
}

void ASurvivor::OnEquipSlot4()
{
	if (SlotComp)
	{
		SlotComp->EquipSlot(ESlotType::HandleSlot);
	}
}

void ASurvivor::OnCycleNext()
{
	if (SlotComp)
	{
		SlotComp->CycleFilled(+1);
	}
}

void ASurvivor::OnSlotScroll(const FInputActionValue& V)
{
	if (!SlotComp) return;

	const float Axis = V.Get<float>();
	if (Axis > 0.f)
	{
		SlotComp->CycleFilled(+1); // 휠 위
	}
	else if (Axis < 0.f)
	{
		SlotComp->CycleFilled(-1); // 휠 아래
	}
}

