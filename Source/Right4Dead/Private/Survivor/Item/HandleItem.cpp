// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HandleItem.h"

#include "Survivor.h"
#include "UISurvivorMedKit.h"
#include "Components/SlateWrapperTypes.h"


// Sets default values
AHandleItem::AHandleItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHandleItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHandleItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHandleItem::ApplyItemEffect()
{
	Super::ApplyItemEffect();

	/*// 한번만 눌렀을때 실행되어야하는 내용
	if (UAnimInstance* AnimInstance = Arms->GetAnimInstance())
	{
		AnimInstance->Montage_Play(ShoveMontage);
	}*/
	
	// 채력 회복
	// 잃은 체력의 80% 회복
	Char->CurrentHP += 0.8f * (Char->MaxHP - Char->CurrentHP);
	
	if (Char->MedKitUI)
	{
		Char->MedKitUI->SetVisibility(ESlateVisibility::Hidden);
		Char->MedKitUI->RemoveFromParent();
	}
}

