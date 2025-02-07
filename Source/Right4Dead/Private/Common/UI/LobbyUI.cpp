// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/UI/LobbyUI.h"

#include "Right4DeadGameInstance.h"
#include "Kismet/GameplayStatics.h"

void ULobbyUI::SetCampaignDifficulty(const EDifficulty& Difficulty) const
{
	if (URight4DeadGameInstance* GameInstance = Cast<URight4DeadGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		GameInstance->SetDifficulty(Difficulty);
	}
}