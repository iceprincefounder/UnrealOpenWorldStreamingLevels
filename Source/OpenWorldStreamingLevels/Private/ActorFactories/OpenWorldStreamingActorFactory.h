#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "ActorFactories/ActorFactory.h"
#include "OpenWorldStreamingActorFactory.generated.h"

struct FAssetData;

UCLASS(config = Editor)
class UOpenWorldStreamingActorFactory : public UActorFactory
{
	GENERATED_UCLASS_BODY()
};