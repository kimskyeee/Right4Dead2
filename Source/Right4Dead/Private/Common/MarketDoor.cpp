// Fill out your copyright notice in the Description page of Project Settings.


#include "MarketDoor.h"


// Sets default values
AMarketDoor::AMarketDoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	LeftHinge = CreateDefaultSubobject<USceneComponent>(TEXT("LeftHinge"));
	RightHinge = CreateDefaultSubobject<USceneComponent>(TEXT("RightHinge"));
	Frame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frame"));
	LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoor"));
	LeftBackGrip = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftBackGrip"));
	LeftFrontGrip = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftFrontGrip"));
	LeftGlass = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftGlass"));
	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoor"));
	RightBackGrip = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightBackGrip"));
	RightFrontGrip = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightFrontGrip"));
	RightGlass = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightGlass"));
	if (const ConstructorHelpers::FObjectFinder<UStaticMesh> FrameMesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Datasmith/final/Geometries/Group_11.Group_11'")); FrameMesh.Succeeded())
	{
		Frame->SetStaticMesh(FrameMesh.Object);
	}
	if (const ConstructorHelpers::FObjectFinder<UStaticMesh> DoorMesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Datasmith/final/Geometries/Group_12.Group_12'")); DoorMesh.Succeeded())
	{
		LeftDoor->SetStaticMesh(DoorMesh.Object);
		RightDoor->SetStaticMesh(DoorMesh.Object);
	}
	if (const ConstructorHelpers::FObjectFinder<UStaticMesh> GripMesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Datasmith/final/Geometries/Group_14.Group_14'")); GripMesh.Succeeded())
	{
		LeftBackGrip->SetStaticMesh(GripMesh.Object);
		LeftFrontGrip->SetStaticMesh(GripMesh.Object);
		RightBackGrip->SetStaticMesh(GripMesh.Object);
		RightFrontGrip->SetStaticMesh(GripMesh.Object);
	}
	if (const ConstructorHelpers::FObjectFinder<UStaticMesh> GlassMesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Datasmith/final/Geometries/Group_13.Group_13'")); GlassMesh.Succeeded())
	{
		LeftGlass->SetStaticMesh(GlassMesh.Object);
		RightGlass->SetStaticMesh(GlassMesh.Object);
	}

	SetRootComponent(Root);
	Frame->SetupAttachment(RootComponent);
	LeftHinge->SetupAttachment(RootComponent);
	RightHinge->SetupAttachment(RootComponent);
	LeftDoor->SetupAttachment(LeftHinge);
	RightDoor->SetupAttachment(RightHinge);
	LeftBackGrip->SetupAttachment(LeftDoor);
	LeftFrontGrip->SetupAttachment(LeftDoor);
	LeftGlass->SetupAttachment(LeftDoor);
	RightBackGrip->SetupAttachment(RightDoor);
	RightFrontGrip->SetupAttachment(RightDoor);
	RightGlass->SetupAttachment(RightDoor);

	Frame->SetRelativeScale3D(FVector(1, 1, 1.136364));
	LeftHinge->SetRelativeLocation(FVector(0, -250, 0));
	RightHinge->SetRelativeLocation(FVector(0, -8, 0));
	LeftDoor->SetRelativeLocation(FVector(0, 125, 0));
	LeftDoor->SetRelativeScale3D(FVector(1, 1, 1.136364f));
	RightDoor->SetRelativeLocation(FVector(0, 0, 0));
	RightDoor->SetRelativeScale3D(FVector(1, 1, 1.136364f));
	LeftBackGrip->SetRelativeLocation(FVector(5, -16, 60));
	LeftBackGrip->SetRelativeRotation(FRotator(0,90,0));
	LeftBackGrip->SetRelativeScale3D(FVector(1, 1, 1.136364f));
	LeftFrontGrip->SetRelativeLocation(FVector(0, -16, 60));
	LeftFrontGrip->SetRelativeRotation(FRotator(0,-90,-180));
	LeftFrontGrip->SetRelativeScale3D(FVector(-1, -1, -1.136364f));
	RightBackGrip->SetRelativeLocation(FVector(5, -101, 60));
	RightBackGrip->SetRelativeRotation(FRotator(0,90,180));
	RightBackGrip->SetRelativeScale3D(FVector(-1, -1, -1.136364f));
	RightFrontGrip->SetRelativeLocation(FVector(0, -101, 60));
	RightFrontGrip->SetRelativeRotation(FRotator(0,-90,0));
	RightFrontGrip->SetRelativeScale3D(FVector(1, 1, 1.136364f));
	LeftGlass->SetRelativeLocation(FVector(0, -10, 10));
	RightGlass->SetRelativeLocation(FVector(0, -10, 10));
}

// Called when the game starts or when spawned
void AMarketDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMarketDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMarketDoor::Interaction()
{
	Super::Interaction();
	InteractionDoor();
}

