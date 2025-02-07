// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Right4DeadGameInstance.generated.h"

UENUM()
enum class EDifficulty : uint8
{
	Easy, Normal, Advanced, Expert
};

/**
 * 
 */
UCLASS()
class RIGHT4DEAD_API URight4DeadGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	EDifficulty Difficulty = EDifficulty::Easy;

public:
	void SetDifficulty(const EDifficulty& NewDifficulty);

	EDifficulty GetDifficulty() const;
};