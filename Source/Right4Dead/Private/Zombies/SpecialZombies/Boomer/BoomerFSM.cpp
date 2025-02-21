#include "BoomerFSM.h"

#include "Boomer.h"

UBoomerFSM::UBoomerFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UBoomerFSM::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<ABoomer>(GetOwner());
}
void UBoomerFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}