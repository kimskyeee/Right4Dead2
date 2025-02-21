// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonZombie.h"

#include "AIController.h"
#include "R4DHelper.h"
#include "Right4DeadGameInstance.h"
#include "Survivor.h"
#include "ZombieAnimInstance.h"
#include "ZombieFSM.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Engine/StaticMeshActor.h"
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
	AIControllerClass = AAIController::StaticClass();

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

	const ConstructorHelpers::FObjectFinder<UStaticMesh> HeadObj(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Zombie/CommonZombie/Models/UE4MannequinDismemberment/SK_Mannequin_Head.SK_Mannequin_Head'"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> ArmLeftObj(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Zombie/CommonZombie/Models/UE4MannequinDismemberment/SK_Mannequin_Arm_Left.SK_Mannequin_Arm_Left'"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> ArmRightObj(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Zombie/CommonZombie/Models/UE4MannequinDismemberment/SK_Mannequin_Arm_Right.SK_Mannequin_Arm_Right'"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> LegLeftObj(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Zombie/CommonZombie/Models/UE4MannequinDismemberment/SK_Mannequin_Leg_Left.SK_Mannequin_Leg_Left'"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> LegRightObj(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Zombie/CommonZombie/Models/UE4MannequinDismemberment/SK_Mannequin_Leg_Right.SK_Mannequin_Leg_Right'"));
	if (HeadObj.Succeeded())
		HeadMesh = HeadObj.Object;
	if (ArmLeftObj.Succeeded())
		ArmLeftMesh = ArmLeftObj.Object;
	if (ArmRightObj.Succeeded())
		ArmRightMesh = ArmRightObj.Object;
	if (LegLeftObj.Succeeded())
		LegLeftMesh = LegLeftObj.Object;
	if (LegRightObj.Succeeded())
		LegRightMesh = LegRightObj.Object;
}

// Called when the game starts or when spawned
void ACommonZombie::BeginPlay()
{
	Super::BeginPlay();
	
	AIController = Cast<AAIController>(GetController());
	if (nullptr == AIController)
	{
		if (nullptr == (AIController = Cast<AAIController>(GetWorld()->SpawnActor(AIControllerClass))))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed Set AI Controller"));
		}
		else
		{
			AIController->Possess(this);
		}
	}
	ZombieFSM->ZombieAI = AIController;
	
	ZombieAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
	ZombieFSM->ZombieAnimInstance = ZombieAnimInstance;
	if (nullptr == ZombieAnimInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed Set ZombieAnimInstance"));
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

void ACommonZombie::TriggerAttack()
{
	PRINT_CALLINFO();
	ZombieAnimInstance->PlayAttack();
	if (ASurvivor* Survivor = Cast<ASurvivor>(ZombieFSM->ChaseTarget))
	{
		Survivor->OnDamaged(AttackDamage);
	}
}

void ACommonZombie::HandleShove(const FVector& FromLocation)
{
	Super::HandleShove(FromLocation);
	ZombieFSM->HandleShove(FromLocation);
}

void SpawnPartMesh(USkeletalMeshComponent* SkeletalMesh, FName BoneName, UStaticMesh* SpawnMesh, FVector ImpulseDirection, float Power)
{
	// 이미 Bone이 숨겨진 상태라면 다시 스폰시키지 않는다
	if (SkeletalMesh->IsBoneHiddenByName(BoneName))
	{
		return;
	}
	// Bone을 숨겨라
	SkeletalMesh->HideBoneByName(BoneName, PBO_None);

	// Bone의 위치
	const FTransform Transform = SkeletalMesh->GetBoneTransform(BoneName);
	// 가짜 Static Mesh를 원래 Bone 위치에 스폰
	auto* StaticMeshActor = SkeletalMesh->GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Transform);
	// 이동 가능하게
	StaticMeshActor->SetMobility(EComponentMobility::Type::Movable);
	// 물리 시뮬레이트 활성화
	StaticMeshActor->GetStaticMeshComponent()->SetSimulatePhysics(true);
	StaticMeshActor->GetStaticMeshComponent()->SetStaticMesh(SpawnMesh);
	// 적절한 방향으로 적절한 세기로 날린다
	ImpulseDirection *= Power;
	StaticMeshActor->GetStaticMeshComponent()->AddImpulse(ImpulseDirection);
}

void ACommonZombie::HandleDismemberment(const FPointDamageEvent* PointDamageEvent)
{
	PRINT_CALLINFO();
	const FName ParentBoneName = UR4DHelper::GetParentBone(GetMesh(), PointDamageEvent->HitInfo.BoneName);
	if (ParentBoneName == TEXT("neck_01"))
	{
		SpawnPartMesh(GetMesh(), TEXT("neck_01"), HeadMesh, FVector(1, 0, 0), 1000);
	}
	else if (ParentBoneName == TEXT("lowerarm_l"))
	{
		SpawnPartMesh(GetMesh(), TEXT("lowerarm_l"), ArmLeftMesh, FVector(1, 0, 0), 1000);
	}
	else if (ParentBoneName == TEXT("lowerarm_r"))
	{
		SpawnPartMesh(GetMesh(), TEXT("lowerarm_r"), ArmRightMesh, FVector(1, 0, 0), 1000);
	}
	else if (ParentBoneName == TEXT("thigh_l"))
	{
		SpawnPartMesh(GetMesh(), TEXT("thigh_l"), LegLeftMesh, FVector(1, 0, 0), 1000);
	}
	else if (ParentBoneName == TEXT("thigh_r"))
	{
		SpawnPartMesh(GetMesh(), TEXT("thigh_r"), LegRightMesh, FVector(1, 0, 0), 1000);
	}
}

void ACommonZombie::OnDamaged(float Damage)
{
	Super::OnDamaged(Damage);
}

void ACommonZombie::OnDie()
{
	Super::OnDie();
	GetCharacterMovement()->bUseRVOAvoidance = false;
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
	GetMesh()->SetCollisionProfileName("NoCollision");
	ZombieFSM->HandleDie();
}

void ACommonZombie::ForceDie()
{
	OnDie();
}

float ACommonZombie::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	const float Ret = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 죽었고, 죽은 원인이 PointDamage일 때만 해당 부위 사지분해 실시
	if (Hp <= 0)
	{
		if (const FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)&DamageEvent)
		{
			HandleDismemberment(PointDamageEvent);
		}
	}

	return Ret;
}