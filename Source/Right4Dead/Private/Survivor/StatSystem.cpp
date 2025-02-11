// Fill out your copyright notice in the Description page of Project Settings.


#include "StatSystem.h"

#include "Survivor.h"
#include "UISurvivorHP.h"
#include "UISurvivorMain.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


// Sets default values for this component's properties
UStatSystem::UStatSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatSystem::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("실행"));

	ASurvivor* player = Cast<ASurvivor>(GetOwner());
	if (player->MainUI)
	{
		UE_LOG(LogTemp, Warning, TEXT("호출"));
		UUISurvivorMain* Main = Cast<UUISurvivorMain>(player->MainUI);
		if (Main)
		{
			InitializeStat();
			UE_LOG(LogTemp, Warning, TEXT("초기화완료"));
		}
	}
}

// Called every frame
void UStatSystem::TickComponent(float DeltaTime, ELevelTick TickType,
                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatSystem::UpdateHPText_Implementation()
{
	ASurvivor* player = Cast<ASurvivor>(GetOwner());
	if (player->MainUI)
	{
		UUISurvivorMain* Main = Cast<UUISurvivorMain>(player->MainUI);
		if (Main && Main->PlayerHP)
		{
			Main->PlayerHP->SetHealthBarPercent(Health, MaxHealth);
		}
	}
}

void UStatSystem::InitializeStat_Implementation()
{
	Health = MaxHealth;
	ASurvivor* player = Cast<ASurvivor>(GetOwner());
	if (player->MainUI)
	{
		UUISurvivorMain* Main = Cast<UUISurvivorMain>(player->MainUI);
		if (Main && Main->PlayerHP)
		{
			Main->PlayerHP->SetHealthBarPercent(Health, MaxHealth);
		}
	}
}

void UStatSystem::IncreaseHealth_Implementation(float HealthPoint)
{
	Health += HealthPoint;
	Health = FMath::Clamp(Health,0,MaxHealth);
	
	ASurvivor* player = Cast<ASurvivor>(GetOwner());
	if (player->MainUI)
	{
		UUISurvivorMain* Main = Cast<UUISurvivorMain>(player->MainUI);
		if (Main && Main->PlayerHP)
		{
			Main->PlayerHP->SetHealthBarPercent(Health, MaxHealth);
		}
	}

	if (Health>=MaxHealth)
	{
		Health = MaxHealth;
	}
	
}

void UStatSystem::DecreaseHealth_Implementation(float DamagePoint)
{
	Health -= DamagePoint;
	Health = FMath::Clamp(Health,0,MaxHealth);
	
	ASurvivor* player = Cast<ASurvivor>(GetOwner());
	if (player->MainUI)
	{
		UUISurvivorMain* Main = Cast<UUISurvivorMain>(player->MainUI);
		if (Main && Main->PlayerHP)
		{
			Main->PlayerHP->SetHealthBarPercent(Health, MaxHealth);
		}
	}

	if (Health<=0)
	{
		Health = 0;
		bIsDead = true;
	}
	else
		bIsDead = false;
}



