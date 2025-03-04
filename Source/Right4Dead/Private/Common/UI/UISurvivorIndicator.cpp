// Fill out your copyright notice in the Description page of Project Settings.


#include "UISurvivorIndicator.h"

#include "Survivor.h"
#include "Components/Border.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"


// 생성자 구현
UUISurvivorIndicator::UUISurvivorIndicator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 1. 기반 머티리얼 로드
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(
		TEXT("/Game/Blueprints/Survivor/Materials/MI_AttackIndicator.MI_AttackIndicator")
	);
    
	if (MaterialAsset.Succeeded())
	{
		BaseMaterial = MaterialAsset.Object;
	}
}

void UUISurvivorIndicator::NativeConstruct()
{
	Super::NativeConstruct();
	
	// 2. 동적 머티리얼 생성
	if (BaseMaterial)
	{
		Material = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		FSlateBrush NewBrush;
		NewBrush.SetResourceObject(Material);
		Indicator->SetBrush(NewBrush);
	}
}

void UUISurvivorIndicator::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	player=Cast<ASurvivor>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (player)
	{
		FVector PlayerForward=player->GetActorForwardVector();
		FVector PlayerPos=player->GetActorLocation();
		FVector CalPos=PlayerPos-HitLocation;

		float XValue = UKismetMathLibrary::Dot_VectorVector(PlayerForward,CalPos.GetSafeNormal());
		float YValue = (PlayerForward.X)*(CalPos.Y)-(PlayerForward.Y)*(CalPos.X);
		float Value = UKismetMathLibrary::DegAtan2(YValue, XValue);
		
		float ParameterValue = UKismetMathLibrary::MapRangeClamped(Value, 180, -180, 0, 1);
		// Material 사용 부분은 동일하게 유지
		if (Material)
		{
			Material->SetScalarParameterValue(FName("Angle"), ParameterValue);
		}
	}
}
