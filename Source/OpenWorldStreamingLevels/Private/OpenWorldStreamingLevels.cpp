// Copyright Epic Games, Inc. All Rights Reserved.

#include "OpenWorldStreamingLevels.h"
#include "OpenWorldStreamingLevelsStyle.h"
#include "OpenWorldStreamingLevelsCommands.h"

#include "OpenWorldStreamingActor.h"
#include "ActorFactories/OpenWorldStreamingActorFactory.h"

#include "LevelEditor.h"
#include "EditorLevelUtils.h"
#include "ToolMenus.h"
#include "Editor.h"
#include "Engine/World.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"

static const FName OpenWorldStreamingLevelsTabName("OpenWorldStreamingLevels");

#define LOCTEXT_NAMESPACE "FOpenWorldStreamingLevelsModule"

void FOpenWorldStreamingLevelsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FOpenWorldStreamingLevelsStyle::Initialize();
	FOpenWorldStreamingLevelsStyle::ReloadTextures();

	FOpenWorldStreamingLevelsCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FOpenWorldStreamingLevelsCommands::Get().CreateInitialActor,
		FExecuteAction::CreateRaw(this, &FOpenWorldStreamingLevelsModule::CreateButtonClicked),
		FCanExecuteAction());
	PluginCommands->MapAction(
		FOpenWorldStreamingLevelsCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FOpenWorldStreamingLevelsModule::EditorButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FOpenWorldStreamingLevelsModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(OpenWorldStreamingLevelsTabName, FOnSpawnTab::CreateRaw(this, &FOpenWorldStreamingLevelsModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FOpenWorldStreamingLevelsTabTitle", "OpenWorldStreamingLevels"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	RegisterFactories();
}

void FOpenWorldStreamingLevelsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FOpenWorldStreamingLevelsStyle::Shutdown();

	FOpenWorldStreamingLevelsCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(OpenWorldStreamingLevelsTabName);
}

TSharedRef<SDockTab> FOpenWorldStreamingLevelsModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FOpenWorldStreamingLevelsModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("OpenWorldStreamingLevels.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FOpenWorldStreamingLevelsModule::AddMenuExtension(FMenuBuilder & MenuBuilder)
{
	MenuBuilder.BeginSection("OpenWorldStreamingLevels", LOCTEXT("OpenWorldStreamingLevelsLabel", "Open World Streaming Levels"));
	MenuBuilder.AddMenuEntry(FOpenWorldStreamingLevelsCommands::Get().CreateInitialActor);
	MenuBuilder.AddMenuEntry(FOpenWorldStreamingLevelsCommands::Get().OpenPluginWindow);
	MenuBuilder.EndSection();
}

void FOpenWorldStreamingLevelsModule::CreateButtonClicked()
{
	if (InitOpenWorldStreamingActor())
	{
		FNotificationInfo Info(LOCTEXT("Successed_Notification", "Create OpenWorldStreamingActor successed!"));
		Info.bUseLargeFont = false;
		Info.bFireAndForget = true;
		Info.bUseThrobber = false;
		Info.ExpireDuration = 5.0f;
		Info.Image = FOpenWorldStreamingLevelsStyle::Get().GetBrush(TEXT("Notification.Success"));
		FSlateNotificationManager::Get().AddNotification(Info);
	}
	else
	{
		FNotificationInfo Info(LOCTEXT("Failed_Notification", "OpenWorldStreamingActor already exist!"));
		Info.bUseLargeFont = false;
		Info.bFireAndForget = true;
		Info.bUseThrobber = false;
		Info.ExpireDuration = 5.0f;
		Info.Image = FOpenWorldStreamingLevelsStyle::Get().GetBrush(TEXT("Notification.Warning"));
		FSlateNotificationManager::Get().AddNotification(Info);
	}
}

void FOpenWorldStreamingLevelsModule::EditorButtonClicked()
{
	InitOpenWorldStreamingActor();
	FGlobalTabmanager::Get()->TryInvokeTab(OpenWorldStreamingLevelsTabName);
}

bool FOpenWorldStreamingLevelsModule::InitOpenWorldStreamingActor()
{
	UWorld * CurrentWorld = GEditor->GetEditorWorldContext().World();
	ULevel* CurrentLevel = CurrentWorld->PersistentLevel;
	for (TObjectIterator<AOpenWorldStreamingActor> Iter; Iter; ++Iter)
	{
		AOpenWorldStreamingActor* Actor = *Iter;
		if (Actor && !Actor->IsPendingKill() && Actor->IsValidLowLevel() && Actor->GetWorld() == CurrentWorld)
		{
			return false;
		}
	}
	AActor* NewActor = GEditor->AddActor(CurrentLevel, AOpenWorldStreamingActor::StaticClass(), FTransform::Identity);
	AOpenWorldStreamingActor* OpenWorldStreamingActor = Cast<AOpenWorldStreamingActor>(NewActor);
	return true;
}

void FOpenWorldStreamingLevelsModule::RegisterMenus()
{
	MainMenuExtender = MakeShareable(new FExtender);

	// Extend File menu, we will add Houdini section.
	MainMenuExtender->AddMenuExtension(
		"WindowGlobalTabSpawners",
		EExtensionHook::After,
		PluginCommands,
		FMenuExtensionDelegate::CreateRaw(this, &FOpenWorldStreamingLevelsModule::AddMenuExtension));

	// Add our menu extender
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MainMenuExtender);
}

void FOpenWorldStreamingLevelsModule::RegisterFactories()
{
	// Register actor factory.
	UActorFactory* OpenWorldStreamingActorFactory = NewObject<UOpenWorldStreamingActorFactory>(GetTransientPackage(), UOpenWorldStreamingActorFactory::StaticClass());
	if (GEditor)
	{
		GEditor->ActorFactories.Add(OpenWorldStreamingActorFactory);
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FOpenWorldStreamingLevelsModule, OpenWorldStreamingLevels)