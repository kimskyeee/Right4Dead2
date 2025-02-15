// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonZombie.h"

#include "Right4DeadGameInstance.h"
#include "Survivor.h"
#include "ZombieAIController.h"
#include "ZombieAnimInstance.h"
#include "ZombieFSM.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "Right4Dead/Right4Dead.h"

// Sets default values
ACommonZombie::ACommonZombie()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Hp = 50.0f;
	Speed = 250.0f;
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshObj(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/ThirdPerson/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));
	if (SkeletalMeshObj.Succeeded())
	{
		GetMesh()->SetSkeletalMeshAsset(SkeletalMeshObj.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -89));
		GetMesh()->SetRelativeRotation(FRotator(0, 270, 0));
		ConstructorHelpers::FClassFinder<UAnimInstance> AnimBlueprintClass(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/Zombies/CommonZombie/ABP_CommonZombie.ABP_CommonZombie_C'"));
		if (AnimBlueprintClass.Succeeded())
		{
			GetMesh()->SetAnimInstanceClass(AnimBlueprintClass.Class);
		}
	}
	ZombieFSM = CreateDefaultSubobject<UZombieFSM>(TEXT("ZombieFSM"));
	AIControllerClass = AZombieAIController::StaticClass();

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->GravityScale = 1.75f;
	Movement->MaxAcceleration = 1500.0f;
	Movement->BrakingFrictionFactor = 1.0f;
	Movement->bUseSeparateBrakingFriction = true;
	Movement->MaxWalkSpeed = 500.0f;
	Movement->MinAnalogWalkSpeed = 20.0f;
	Movement->BrakingDecelerationWalking = 2000.0f;
	Movement->JumpZVelocity = 700.0f;
	Movement->BrakingDecelerationFalling = 1500.0f;
	Movement->AirControl = 0.35f;
	Movement->RotationRate = FRotator(0, 500, 0);
	Movement->bOrientRotationToMovement = true;
	Movement->bUseRVOAvoidance = true;
	Movement->AvoidanceConsiderationRadius = 160;
	Movement->SetFixedBrakingDistance(200.0f);
	Movement->GetNavMovementProperties()->bUseAccelerationForPaths = true;
	Movement->GetNavMovementProperties()->bUseFixedBrakingDistanceForPaths = true;
	Movement->GetNavAgentPropertiesRef().AgentRadius = 42.0f;
	Movement->GetNavAgentPropertiesRef().AgentHeight = 192.0f;
}

// Called when the game starts or when spawned
void ACommonZombie::BeginPlay()
{
	Super::BeginPlay();
	
	AIController = Cast<AZombieAIController>(GetController());
	if (nullptr == AIController)
	{
		if (nullptr == (AIController = Cast<AZombieAIController>(GetWorld()->SpawnActor(AIControllerClass))))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed Set AI Controller"));
		}
		else
		{
			AIController->Possess(this);
			ZombieFSM->ZombieAI = AIController;
		}
	}
	
	ZombieAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
	ZombieFSM->ZombieAnimInstance = ZombieAnimInstance;
	if (nullptr == ZombieAnimInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed Set ZombieAnimInstance"));
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
	AIController->GetPathFollowingComponent()->PauseMove();
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
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
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	AIController->GetPathFollowingComponent()->ResumeMove();
	ClimbDestination = FTransform::Identity;
}

AActor* ACommonZombie::GetChasingTarget()
{
	return Target;
}

void ACommonZombie::TriggerAttack()
{
	PRINT_CALLINFO();
	ZombieAnimInstance->PlayAttack();
}

void ACommonZombie::HandleShove(const FVector& FromLocation)
{
	Super::HandleShove(FromLocation);
	ZombieFSM->HandleShove(FromLocation);
}

void ACommonZombie::OnDamaged(float Damage)
{
	Super::OnDamaged(Damage);
	if (Hp > 0)
	{
		
	}
}

void ACommonZombie::OnDie()
{
	Super::OnDie();
	ZombieFSM->HandleDie();
}

void ACommonZombie::ForceDie()
{
	OnDie();
	GetCharacterMovement()->bUseRVOAvoidance = false;
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
	GetMesh()->SetCollisionProfileName("NoCollision");
}
