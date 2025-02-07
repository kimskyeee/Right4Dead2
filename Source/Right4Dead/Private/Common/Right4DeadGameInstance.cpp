// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/Right4DeadGameInstance.h"

void URight4DeadGameInstance::SetDifficulty(const EDifficulty& NewDifficulty)
{
	Difficulty = NewDifficulty;
}

EDifficulty URight4DeadGameInstance::GetDifficulty() const
{
	return Difficulty;
}
