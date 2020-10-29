// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FOpenWorldStreamingLevelsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command : CreateInitialActor*/
	void CreateButtonClicked();

	/** This function will be bound to Command : OpenPluginWindow*/
	void EditorButtonClicked();
	
	bool InitOpenWorldStreamingActor();
private:

	void RegisterMenus();

	void RegisterFactories();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

	void AddMenuExtension(FMenuBuilder & MenuBuilder);

private:
	TSharedPtr<class FUICommandList> PluginCommands;

	// The extender to pass to the level editor to extend it's window menu.
	TSharedPtr<FExtender> MainMenuExtender;
};
