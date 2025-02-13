// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/CommonZombie.h"

#include "AIController.h"
#include "Right4DeadGameInstance.h"
#include "Survivor.h"
#include "ZombieAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Right4Dead/Right4Dead.h"

// Sets default values
ACommonZombie::ACommonZombie()
{
	PRINT_CALLINFO();
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Hp = 50.0f;
	Speed = 250.0f;
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshObj(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/ThirdPerson/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (SkeletalMeshObj.Succeeded())
	{
		GetMesh()->SetSkeletalMeshAsset(SkeletalMeshObj.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -89));
		GetMesh()->SetRelativeRotation(FRotator(0, 270, 0));
		ConstructorHelpers::FClassFinder<UAnimInstance> AnimBlueprintClass(TEXT("/Script/Engine.AnimBlueprint'/Game/Assets/ThirdPerson/Characters/Mannequins/Animations/ABP_Manny.ABP_Manny_C'"));
		if (AnimBlueprintClass.Succeeded())
		{
			GetMesh()->SetAnimInstanceClass(AnimBlueprintClass.Class);
		}
	}
	AIControllerClass = AZombieAIController::StaticClass();
}

// Called when the game starts or when spawned
void ACommonZombie::BeginPlay()
{
	Super::BeginPlay();
	
	AAIController* AIController = Cast<AAIController>(GetController());
	if (nullptr == AIController)
	{
		if (nullptr == (AIController = Cast<AAIController>(GetWorld()->SpawnActor(AIControllerClass))))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed Set AI Controller"));
		}
		AIController->Possess(this);
	}

	if (nullptr == Target)
	{
		if (nullptr == (Target = UGameplayStatics::GetActorOfClass(this, ASurvivor::StaticClass())))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed Set Target"));
		}
		AIController->MoveToActor(Target);
	}
}

void ACommonZombie::InitDifficulty()
{
	// GameInstance 가져오기
	if (const URight4DeadGameInstance* GameInstance = Cast<URight4DeadGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		switch (GameInstance->GetDifficulty())
		{
		case EDifficulty::Easy:
			TakeDamageMultiplier = 2; // 200%
			AttackDamage = 1;
			break;
		case EDifficulty::Normal:
			TakeDamageMultiplier = 1; // 100%
			AttackDamage = 2;
			break;
		case EDifficulty::Advanced:
			TakeDamageMultiplier = 0.75f; // 75%
			AttackDamage = 5;
			break;
		case EDifficulty::Expert:
			TakeDamageMultiplier = 0.5f; // 50%
			AttackDamage = 20;
			break;
		}
	}
}

// Called every frame
void ACommonZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bClimbing)
	{

		const FVector P0 = GetActorLocation();
		const bool bIsNearZ = GetCharacterMovement()->GetFeetLocation().Z >= ClimbDestination.GetLocation().Z;
		if (false == bIsNearZ) // 아직 덜 올라왔다면
		{
			// Z축으로만 오른다
			const FVector P = P0 + FVector(0, 0, 1) * Speed * DeltaTime;
			SetActorLocation(P);
		}
		else
		{
			const FVector P = P0 + GetActorForwardVector() * Speed * DeltaTime;
			SetActorLocation(P);
		}
	}
}

void ACommonZombie::StartClimbing(const FTransform& Destination)
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		/*// 이동 가능한 경로 찾기
		UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), Target);
		if (NavPath && NavPath->IsValid() && NavPath->PathPoints.Num() > 0)
		{
			return;
		}*/
		
		AIController->StopMovement();
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	}
	bClimbing = true;
	ClimbDestination = Destination;
	SetActorRotation(Destination.Rotator());
}

void ACommonZombie::EndClimbing()
{
	if (false == bClimbing)
	{
		return;
	}
	bClimbing = false;
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->MoveToActor(Target);
	}
	ClimbDestination = FTransform::Identity;
}

AActor* ACommonZombie::GetChasingTarget()
{
	return Target;
}

void ACommonZombie::OnChangedTarget() const
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->MoveToActor(Target);
	}
}
