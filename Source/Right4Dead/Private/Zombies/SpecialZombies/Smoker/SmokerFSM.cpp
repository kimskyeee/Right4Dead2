#include "SmokerFSM.h"

#include "Smoker.h"

USmokerFSM::USmokerFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void USmokerFSM::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<ASmoker>(GetOwner());
}
void USmokerFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

