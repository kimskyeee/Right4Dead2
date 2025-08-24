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
	UE_LOG(LogTemp, Warning, TEXT("[Indicator Tick]"));

	if (LastHitTime > 0.f && (GetWorld()->GetTimeSeconds() - LastHitTime) > 1.f)
	{
		SetVisibility(ESlateVisibility::Hidden);
		LastHitTime = -1.f;
	}
	
	Char = Cast<ASurvivor>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!Char || !Material) return;
	
	// 플레이어→공격자 벡터
	FVector DirWorld = HitLocation - (Char->GetActorLocation());

	// 수평면 투영(roll/pitch 영향 제거)
	DirWorld.Z = 0.f;
	FVector Fwd = Char->GetActorForwardVector();
	Fwd.Z = 0.f;
	
	if (!Fwd.Normalize() || !DirWorld.Normalize()) return;

	// cos, sin 계산
	const float CosTheta = FVector::DotProduct(Fwd, DirWorld);
	const float SinTheta = FVector::CrossProduct(Fwd, DirWorld).Z; // 우-hand 기준, 좌우 방향 부호

	// 각도 계산 (왼쪽: +, 오른쪽: - 가 됨)
	const float AngleDeg = FMath::RadiansToDegrees(FMath::Atan2(SinTheta, -CosTheta));

	// 0 ~ 1 정규화 (재질 파라미터용) 0 = -180°, 0.5 = 0°, 1 = +180°
	float ParameterValue = (AngleDeg + 180.f) / 360.f;
	
	Material->SetScalarParameterValue(FName("Angle"), ParameterValue);
}

void UUISurvivorIndicator::PlayHitAnimation()
{
	if (IndicatorAnim)
	{
		PlayAnimation(IndicatorAnim, 0, 1);
	}

	SetVisibility(ESlateVisibility::HitTestInvisible);
	LastHitTime = GetWorld()->GetTimeSeconds();
}
