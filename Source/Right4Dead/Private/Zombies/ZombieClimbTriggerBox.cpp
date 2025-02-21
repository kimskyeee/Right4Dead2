// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieClimbTriggerBox.h"

#include "CommonZombie.h"
#include "ZombieFSM.h"
#include "Components/BoxComponent.h"
#include "Right4Dead/Right4Dead.h"


// Sets default values
AZombieClimbTriggerBox::AZombieClimbTriggerBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ClimbEndTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ClimbEndTriggerBox"));
	ClimbEndTriggerBox->SetupAttachment(RootComponent);
	ClimbEndTriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	GetCollisionComponent()->bMultiBodyOverlap = true;
	ClimbEndTriggerBox->bMultiBodyOverlap = true;
}

void AZombieClimbTriggerBox::OnClimbEndTriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	// PRINT_CALLINFO();
	if (ACommonZombie* Zombie = Cast<ACommonZombie>(OtherActor))
	{
		Zombie->EndClimbing();
	}
}

// Called when the game starts or when spawned
void AZombieClimbTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	ClimbEndTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AZombieClimbTriggerBox::OnClimbEndTriggerBoxBeginOverlap);
}

void AZombieClimbTriggerBox::NotifyActorBeginOverlap(AActor* OtherActor)
{
	// PRINT_CALLINFO();

	// ClimbEndTriggerBox가 이미 오버랩을 감지했으면 부모 NotifyActorBeginOverlap을 무시
	if (ClimbEndTriggerBox->IsOverlappingActor(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Ignoring Parent NotifyActorBeginOverlap due to child Box overlap."));
		return;
	}
	Super::NotifyActorBeginOverlap(OtherActor);
	if (AZombieBase* Zombie = Cast<AZombieBase>(OtherActor))
	{
		const AActor* Target = Zombie->ZombieFSM->ChaseTarget;
		if (Target) {
			if (IsOverlappingActor(Target))
			{
				UE_LOG(LogTemp, Warning, TEXT("들어있잖아~"));
				return;
			}
			Zombie->StartClimbing(ClimbEndTriggerBox->GetComponentTransform());
		}
	}
}
