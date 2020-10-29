#include "OpenWorldStreamingActorFactory.h"
#include "OpenWorldStreamingActor.h"

UOpenWorldStreamingActorFactory::UOpenWorldStreamingActorFactory(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
	DisplayName = NSLOCTEXT("OpenWorldStreamingActor", "OpenWorldStreamingActorDisplayName", "Open World Streaming Actor");
	NewActorClass = AOpenWorldStreamingActor::StaticClass();
}