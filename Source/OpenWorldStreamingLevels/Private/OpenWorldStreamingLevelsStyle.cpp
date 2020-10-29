// Copyright Epic Games, Inc. All Rights Reserved.

#include "OpenWorldStreamingLevelsStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"

TSharedPtr< FSlateStyleSet > FOpenWorldStreamingLevelsStyle::StyleInstance = NULL;

void FOpenWorldStreamingLevelsStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FOpenWorldStreamingLevelsStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FOpenWorldStreamingLevelsStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("OpenWorldStreamingLevelsStyle"));
	return StyleSetName;
}

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".ttf") ), __VA_ARGS__ )
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".otf") ), __VA_ARGS__ )

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);
const FVector2D Icon64x64(64.0f, 64.0f);

TSharedRef< FSlateStyleSet > FOpenWorldStreamingLevelsStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("OpenWorldStreamingLevelsStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("OpenWorldStreamingLevels")->GetBaseDir() / TEXT("Resources"));

	Style->Set("OpenWorldStreamingLevels.CreateInitialActor", new IMAGE_BRUSH(TEXT("IconPlanet_32x"), Icon40x40));
	Style->Set("OpenWorldStreamingLevels.OpenPluginWindow", new IMAGE_BRUSH(TEXT("IconWorldEditor_32x"), Icon40x40));

	Style->Set("Notification.Success", new IMAGE_BRUSH(TEXT("NotificationSuccess_32x"), Icon20x20));
	Style->Set("Notification.Warning", new IMAGE_BRUSH(TEXT("NotificationWarning_32x"), Icon20x20));

	Style->Set("ClassIcon.OpenWorldStreamingActor", new IMAGE_BRUSH(TEXT("IconPlanet_16x"), Icon16x16));
	Style->Set("ClassThumbnail.OpenWorldStreamingActor", new IMAGE_BRUSH(TEXT("IconPlanet_64x"), Icon64x64));

	return Style;
}

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT

void FOpenWorldStreamingLevelsStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FOpenWorldStreamingLevelsStyle::Get()
{
	return *StyleInstance;
}
