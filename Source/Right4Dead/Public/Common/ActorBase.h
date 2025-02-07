#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActorBase.generated.h"

UINTERFACE(MinimalAPI)
class UActorBase : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RIGHT4DEAD_API IActorBase
{
	GENERATED_BODY()

public:
	virtual void OnDamaged(float Damage) = 0;
	virtual void OnDie() = 0;
};