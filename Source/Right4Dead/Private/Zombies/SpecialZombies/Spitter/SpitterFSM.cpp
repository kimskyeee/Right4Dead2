#include "SpitterFSM.h"

#include "Spitter.h"

USpitterFSM::USpitterFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void USpitterFSM::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<ASpitter>(GetOwner());
}
void USpitterFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}