// Fill out your copyright notice in the Description page of Project Settings.


#include "UITakeDamage.h"

#include "Survivor.h"
#include "Kismet/GameplayStatics.h"

void UUITakeDamage::PlayAnimationByName()
{
	auto player = Cast<ASurvivor>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	if (player->bIsDamaged)
	{
		PlayAnimation(TakeDamage);
	}
	
}
