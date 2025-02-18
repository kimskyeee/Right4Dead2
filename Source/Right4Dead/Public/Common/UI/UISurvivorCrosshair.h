// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UISurvivorCrosshair.generated.h"

/**
 * 
 */
UCLASS()
class RIGHT4DEAD_API UUISurvivorCrosshair : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//필요변수 생각
	//1. 현재무기 값 인지 (EWeaponType에 따라 크로스헤어 변경)
	//2. 건 무기일경우 -> 움직이거나 쏘거나 할때 동적 크로스헤어 생성

	//바꿀 애들 불러오기
	UPROPERTY(meta = (BindWidget))
	class UBorder* TopCross;
	UPROPERTY(meta = (BindWidget))
	class UBorder* BottomCross;
	UPROPERTY(meta = (BindWidget))
	class UBorder* LeftCross;
	UPROPERTY(meta = (BindWidget))
	class UBorder* RightCross;
	
	//이동에따라 퍼지게 하자
	UPROPERTY(editanywhere,BlueprintReadWrite)
	float CrosshairSpread;
	UPROPERTY(editanywhere,BlueprintReadWrite)
	float CrosshairThickness=4.f;
	UPROPERTY(editanywhere,BlueprintReadWrite)
	float CrosshairLength=25.f;
};
