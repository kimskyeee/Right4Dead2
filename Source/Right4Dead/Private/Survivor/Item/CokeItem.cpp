// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/CokeItem.h"

#include "CokeDelivery.h"
#include "Survivor.h"
#include "UISurvivorCokeDelivery.h"
#include "Components/SlateWrapperTypes.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ACokeItem::ACokeItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACokeItem::BeginPlay()
{
	Super::BeginPlay();
	AttachSocket = TEXT("HandleSocket");
}

// Called every frame
void ACokeItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACokeItem::ApplyItemEffect()
{
	CheckDeliveryPos();
}

void ACokeItem::CheckDeliveryPos()
{
	/*// 콜라면
	auto* Coke = Cast<AWeaponCoke>(CurrentWeapon);
	if (Coke)
	{
		//한번만 눌렀을때
		if (!bCanDeliveryCola)
		{
			if (UAnimInstance* AnimInstance = Arms->GetAnimInstance())
			{
				AnimInstance->Montage_Play(CokeShoveMontage);
				// 짤그락 사운드
				UGameplayStatics::PlaySound2D(this, WeaponSwing, 1, 1);
			}
			SwitchCamera(false);
		}
	}
	
	// 배달 위치 범위 내에 있는지 체크
	if (bCanDeliveryCola)
	{
		auto* CokeDelivery = Cast<ACokeDelivery>(UGameplayStatics::GetActorOfClass(GetWorld(), ACokeDelivery::StaticClass()));
		CokeDelivery->bIsCanOpen = true;
		CokeDelivery->InteractionDelivery();
		if (CokeDeliveryUI)
		{
			CokeDeliveryUI->SetVisibility(ESlateVisibility::Visible);
			CokeDeliveryUI->AddToViewport();
		}

		//콜라배달음
		ColaDeliveryAudio->Play();
	}*/
}

