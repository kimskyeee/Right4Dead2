// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UISurvivorIndicator.generated.h"

/**
 * 
 */
UCLASS()
class RIGHT4DEAD_API UUISurvivorIndicator : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 생성자 선언
	UUISurvivorIndicator(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UBorder* Indicator;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector HitLocation = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere)
	class UMaterialInterface* BaseMaterial; // 기반 머티리얼 참조
	UPROPERTY()
	class UMaterialInstanceDynamic* Material; // 동적 인스턴스는 런타임에서 직접 할당해야합니다
	
	UPROPERTY(EditAnywhere)
	class ASurvivor* Char;
};
