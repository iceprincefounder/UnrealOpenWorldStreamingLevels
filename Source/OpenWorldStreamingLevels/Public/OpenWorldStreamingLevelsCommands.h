// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "OpenWorldStreamingLevelsStyle.h"

class FOpenWorldStreamingLevelsCommands : public TCommands<FOpenWorldStreamingLevelsCommands>
{
public:

	FOpenWorldStreamingLevelsCommands()
		: TCommands<FOpenWorldStreamingLevelsCommands>(TEXT("OpenWorldStreamingLevels"), NSLOCTEXT("Contexts", "OpenWorldStreamingLevels", "OpenWorldStreamingLevels Plugin"), NAME_None, FOpenWorldStreamingLevelsStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > CreateInitialActor;
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};