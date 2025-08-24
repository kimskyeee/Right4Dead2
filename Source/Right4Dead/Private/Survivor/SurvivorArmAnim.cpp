// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivorArmAnim.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Item/ItemBase.h"
#include "Item/SlotComponent.h"


void USurvivorArmAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	// ABP소유하고 있는 pawn 얻어오기
	auto OwnerPawn = TryGetPawnOwner();
	OwnerChar = Cast<ASurvivor>(OwnerPawn);
	
	SlotComp = OwnerChar->FindComponentByClass<USlotComponent>();
	BindDelegates();

	// 초기 Hands 상태 반영
	if (SlotComp.IsValid() && SlotComp->CurrentInHands.IsValid())
	{
		HandleInHandsChanged(SlotComp->CurrentInHands.Get());
	}
}

void USurvivorArmAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!OwnerChar.IsValid()) return;
	
	//방향, 속도
	FVector velocity = OwnerChar->GetVelocity();
	FVector forward = OwnerChar->GetActorForwardVector();
	speed = FVector::DotProduct(velocity, forward);

	FVector right = OwnerChar->GetActorRightVector();
	direction = FVector::DotProduct(velocity, right);
	
	//공중에 있는지 여부
	bIsInAir = OwnerChar->GetCharacterMovement()->IsFalling();

	CurrentType = OwnerChar->GetCurrentItemType();
}


void USurvivorArmAnim::AnimNotify_AxeCameraShake()
{
	auto PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->PlayerCameraManager->StartCameraShake(OwnerChar->SweepCameraShake);
	}
}

void USurvivorArmAnim::AnimNotify_SpawnCylinder()
{
	// 실린더 스윕 발생
	if (SlotComp.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("AnimNotify_SpawnCylinder"));
		SlotComp->CurrentInHands->SpawnShoveCylinder();		
	}
}

void USurvivorArmAnim::AnimNotify_ENDReloading()
{
	auto Player = Cast<ASurvivor>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Player)
	{
		// player->bIsReloading = false;
	}
}

void USurvivorArmAnim::BindDelegates()
{
	if (SlotComp.IsValid())
	{
		SlotComp->OnInHandsItemChanged.AddDynamic(this, &USurvivorArmAnim::HandleInHandsChanged);
	}
}

void USurvivorArmAnim::UnbindDelegates()
{
	if (SlotComp.IsValid())
	{
		SlotComp->OnInHandsItemChanged.RemoveAll(this);
	}
}

void USurvivorArmAnim::HandleInHandsChanged(class AItemBase* NewItem)
{
	PlayEquipMontageForItem(NewItem);
}

void USurvivorArmAnim::PlayEquipMontageForItem(class AItemBase* NewItem)
{
	// 소모품/빈손이면 스킵(필요 시 Unequip 몽타주 재생)
	AItemBase* Item = Cast<AItemBase>(NewItem);
	if (!Item) return;
	
	// 기존 Equip 중지(동일 슬롯만)
	if (IsAnyMontagePlaying())
	{
		Montage_Stop(0.15f); 
	}

	UAnimMontage* MontageToPlay = nullptr;

	if (!MontageToPlay)
	{
		// 미지정: 아무 것도 재생하지 않음(안전)
		bIsEquipping = false;
		return;
	}

	// Arms 전용 Slot에서 재생되도록, 몽타주 섹션/슬롯 설정이 맞는지 애셋 쪽에서 확인
	const float Rate = 1.f;
	const float StartPos = 0.f;

	bIsEquipping = true;
	Montage_Play(MontageToPlay, Rate, EMontagePlayReturnType::MontageLength, StartPos, true);
}