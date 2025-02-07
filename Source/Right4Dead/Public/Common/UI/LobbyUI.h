// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Right4DeadGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUI.generated.h"

/**
 * 
 */
UCLASS()
class RIGHT4DEAD_API ULobbyUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetCampaignDifficulty(const EDifficulty& Difficulty) const;
};