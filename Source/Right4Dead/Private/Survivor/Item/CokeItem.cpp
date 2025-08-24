// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/CokeItem.h"

#include "CokeDelivery.h"
#include "Survivor.h"
#include "Components/AudioComponent.h"
#include "Components/SlateWrapperTypes.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ACokeItem::ACokeItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 콜라 전용 몽타주
	ConstructorHelpers::FObjectFinder<UAnimMontage> TempCokeShoveMontage(TEXT("/Script/Engine.AnimMontage'/Game/UltimateFPSAnimationsKIT/Animations/Add/Coke/My_Anim_Attack_I_Montage.My_Anim_Attack_I_Montage'"));
	if (TempCokeShoveMontage.Succeeded())
	{
		Montage_Use_Tap = TempCokeShoveMontage.Object;
	}

	//콜라배달 사운드
	ColaDeliveryAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("ColaDeliveryAudio"));
	ColaDeliveryAudio->bAutoActivate = false; // 자동 실행 방지코드
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
	// 배달 위치 범위 내에 있는지 체크
	if (!bCanDeliveryCola) return;
	
	auto* CokeDelivery = Cast<ACokeDelivery>(UGameplayStatics::GetActorOfClass(GetWorld(), ACokeDelivery::StaticClass()));
	CokeDelivery->bIsCanOpen = true;
	CokeDelivery->InteractionDelivery();
	/*if (CokeDeliveryUI)
	{
		CokeDeliveryUI->SetVisibility(ESlateVisibility::Visible);
		CokeDeliveryUI->AddToViewport();
	}*/

	//콜라배달음
	ColaDeliveryAudio->Play();
}

void ACokeItem::OnTap(float Elapsed)
{
	Super::OnTap(Elapsed);
}

void ACokeItem::CompleteConsume()
{
	Char->FinishedDeliveryCoke();
	Super::CompleteConsume();
}

