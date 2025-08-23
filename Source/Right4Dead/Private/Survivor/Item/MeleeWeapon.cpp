// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MeleeWeapon.h"

#include "MeleeDamageType.h"
#include "Survivor.h"
#include "UIAttackZombie.h"
#include "ZombieBase.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMeleeWeapon::AMeleeWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	InitBoneMap();
	// 도끼 쿨타임 초기화 (시작해도 바로 공격할 수 있게)
	NextReadyTime = -1;
}

// Called when the game starts or when spawned
void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMeleeWeapon::MeleeAttack()
{
	// 스윕을 하면 사운드를 재생하고 싶다 (맞든 안맞든 재생할거임)
	UGameplayStatics::PlaySound2D(this, Char->SwingMiss, 1, 1);
	
	// 충돌을 위한 가상의 박스 생성
    auto BoxShape = FCollisionShape::MakeBox(FVector(100, 100, 5));
   
    // 충돌결과 저장을 위한 배열 선언
    // SweepMultiByChannel이 수행되면 여기에 HitResult 구조체(충돌과 관련된 정보들이 들어있음)들이 쌓인다
    TArray<struct FHitResult> HitResults;
  
    // 시작과 끝점 (박스의 중심), 현재의 80은 캐릭터의 머리위치 정도인듯
    // 시작 지점과 끝 지점은 같도록 하면 된다 (Z축 좌표를 모니터 정 중앙 위치를 기준으로)

	FVector Start,End;
	FRotator CameraRotation;
	FVector BoxLocation;
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		int32 ViewportX, ViewportY;
		PC->GetViewportSize(ViewportX, ViewportY);

		float CenterX = ViewportX / 2.f;
		float CenterY = ViewportY / 2.f;

		FVector WorldLocation;
		FVector WorldDirection;
		bool bDeprojectSuccess = PC->DeprojectScreenPositionToWorld(CenterX, CenterY, WorldLocation, WorldDirection);

		if (bDeprojectSuccess)
		{
			// 뷰포트 중심을 기준으로 시작점과 끝점 설정
			Start = WorldLocation;
			End = Start + (WorldDirection * 500.f); // 적당한 거리로 설정

			// 박스의 위치를 뷰포트 중심으로 설정
			BoxLocation = Start;
			CameraRotation = PC->PlayerCameraManager->GetCameraRotation();
		}
	}
	
    // 나 자신은 제외하고 싶다 (필요하면 무기 액터 같은 것들도 제외하면 된다)
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    // SweepMultiByChannel 수행
    const bool bHit = GetWorld()->SweepMultiByChannel(HitResults, Start, End, CameraRotation.Quaternion(), ECC_GameTraceChannel10, BoxShape, Params);
	// ->HitResults 배열에 충돌 결과들이 저장된다
	
	//5. 충돌결과 처리
    // 만약 가상의 박스 안에 뭔가가 있었다면?
    if (bHit)
    {
    	//좀비 사운드 재생하자
    	UGameplayStatics::PlaySound2D(this, Char->SwingHitZombie, 1, 1);
    	
    	//공격을 맞췄다는 변수 true
    	Char->bIsAttacked = true;
    	Char->AttackZombieUI->PlayAnimationByName(Char);
    	
		// 한 액터(좀비)의 여러 부위(왼쪽팔, 머리, 오른쪽팔)가 박스 영역 안에 동시에 들어왔을때 분류 (한 번만 타격!)
        TMap<AActor*, TArray<FName>> HitMap;

        for (auto HitResult : HitResults)
        {
			// 만약 BoneName이 None이라면 SkeletalMesh가 아니라는 뜻이다. 스킵하자.
            FName BoneName = HitResult.BoneName;
            if (HitResult.BoneName.IsNone())
			{
				continue;
            }
        	
            AActor* Actor = Cast<AZombieBase>(HitResult.GetActor());
            if (nullptr == Actor)
            {
                continue;
            }
        	
			// Actor의 BoneName 부위가 피격당했다
            // 해당 Actor가 아직 없다면 새로운 배열(TArray))을 만들고 피격부위(BoneName)를 추가한 후 HitMap에 저장
            if (false == HitMap.Contains(Actor))
            {
               TArray<FName> BoneArray;
               BoneArray.Add(BoneName);
               HitMap.Add(Actor, BoneArray);
            }
            // 이미 존재한다면 기존의 TArray를 가져와서 BoneName을 추가한다.
            else
            {
               HitMap[Actor].Add(BoneName);
            }
		}

        TArray<AActor*> Actors;
        HitMap.GetKeys(Actors);

        for (auto Actor : Actors)
        {
			int HighPriority = INT_MAX;
            FName HighPriorityBoneName;

			for (auto BoneName : HitMap[Actor])
			{
		        int Priority = BoneMap[BoneName];
		        if (Priority < HighPriority)
				{
					HighPriority = Priority;
		            HighPriorityBoneName = BoneName;
		        }
		    }

       		// 어떤 부위들을 피격 당했는지 알았으니 우선순위가 가장 높은 Bone에 맞았다고 하고 데미지 주기
	        FHitResult HR;
	        HR.BoneName = HighPriorityBoneName;
	        UGameplayStatics::ApplyPointDamage(Actor, 9999, GetActorRightVector() * -1.0f, HR, Char->GetController(), this, UMeleeDamageType::StaticClass());
        }
   }
}

void AMeleeWeapon::InitBoneMap()
{
	// BoneMap 초기화 (뼈의 이름과 우선순위 매핑))
	
	// 머리
	BoneMap.Add(TEXT("head"), 0);
	BoneMap.Add(TEXT("neck_01"), 0);

	// 가슴
	BoneMap.Add(TEXT("spine_02"), 1);

	// 배
	BoneMap.Add(TEXT("pelvis"), 2);
	BoneMap.Add(TEXT("spine_01"), 2);

	// 팔
	BoneMap.Add(TEXT("upperarm_l"), 3);
	BoneMap.Add(TEXT("lowerarm_l"), 3);
	BoneMap.Add(TEXT("hand_l"), 3);
	BoneMap.Add(TEXT("upperarm_r"), 3);
	BoneMap.Add(TEXT("lowerarm_r"), 3);
	BoneMap.Add(TEXT("hand_r"), 3);

	// 다리
	BoneMap.Add(TEXT("thigh_l"), 3);
	BoneMap.Add(TEXT("calf_l"), 3);
	BoneMap.Add(TEXT("foot_l"), 3);
	BoneMap.Add(TEXT("ball_l"), 3);
	BoneMap.Add(TEXT("thigh_r"), 3);
	BoneMap.Add(TEXT("calf_r"), 3);
	BoneMap.Add(TEXT("foot_r"), 3);
	BoneMap.Add(TEXT("ball_r"), 3);
}

void AMeleeWeapon::OnTap(float Elapsed)
{
	const float Now = GetWorld()->GetTimeSeconds();
	
	if (Now < NextReadyTime)
	{
		return;
	}

	// 공격 실행
	MeleeAttack();

	// 몽타주(중복 재생 방지)
	PlayMontageOnce(Montage_Use_Tap);

	// 다음 사용 가능 시각 갱신
	NextReadyTime = Now + CooldownSec;
}

void AMeleeWeapon::OnHoldBegan()
{
	
}

void AMeleeWeapon::OnHoldTick(float Elapsed)
{
	const float Now = GetWorld()->GetTimeSeconds();
	if (Now >= NextReadyTime)
	{
		MeleeAttack();
		EnsureLoopMontage(Montage_Use_Tap);
		NextReadyTime = Now + CooldownSec;
	}
}