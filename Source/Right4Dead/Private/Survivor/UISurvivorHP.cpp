// Fill out your copyright notice in the Description page of Project Settings.


#include "UISurvivorHP.h"

#include "Components/ProgressBar.h"

void UUISurvivorHP::SetHealthBarPercent(float CurrentHealth, float MaxHealth)
{
	HPProgressBar->SetPercent(CurrentHealth / MaxHealth);
}
