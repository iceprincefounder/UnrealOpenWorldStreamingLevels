// Copyright Epic Games, Inc. All Rights Reserved.

#include "OpenWorldStreamingLevelsCommands.h"

#define LOCTEXT_NAMESPACE "FOpenWorldStreamingLevelsModule"

void FOpenWorldStreamingLevelsCommands::RegisterCommands()
{
	UI_COMMAND(CreateInitialActor, "Create Initial Actor", "Create initial OpenWorldStreamingActor", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(OpenPluginWindow, "Open Streaming Editor", "Bring up open world editor window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
