// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenWorldStreamingActor.h"

#include "Engine/World.h"
#include "Engine/WorldComposition.h"
#include "Engine/LevelStreamingAlwaysLoaded.h"
#include "Engine/LevelStreamingDynamic.h"
#include "FileHelpers.h"
#include "AssetRegistryModule.h"
#include "EditorActorFolders.h"
#include "EditorLevelUtils.h"
#include "Landscape.h"
#include "LandscapeInfo.h"
#include "LandscapeStreamingProxy.h"
#include "LandscapeDataAccess.h"

// Sets default values
AOpenWorldStreamingActor::AOpenWorldStreamingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsEditorOnlyActor = true;
}

void AOpenWorldStreamingActor::CreateStreamingLevels()
{
	UWorld* CurrentWorld = GetWorld();
	FString WorldRootPath = FPackageName::LongPackageNameToFilename(CurrentWorld->WorldComposition->GetWorldRoot());
	FName OutlinerRootName = FName("Landscape");

	// Create main landscape actor in persistent level, it will be empty (no components in it)
	// All landscape tiles will go into it's own sub-levels
	FGuid LandscapeGuid = FGuid::NewGuid();
	{
		ALandscape* Landscape = CurrentWorld->SpawnActor<ALandscape>();
		Landscape->SetActorTransform(FTransform(FQuat::Identity, FVector::ZeroVector, FVector(100)));

		// Set landscape configuration
		Landscape->bCanHaveLayersContent = false;
		Landscape->LandscapeMaterial = nullptr;
		Landscape->ComponentSizeQuads = 63;
		Landscape->NumSubsections = 1;
		Landscape->SubsectionSizeQuads = 63;
		Landscape->SetLandscapeGuid(LandscapeGuid);
		//for (const auto& ImportLayerInfo : ImportLayers)
		//{
		//	Landscape->EditorLayerSettings.Add(FLandscapeEditorLayerSettings(ImportLayerInfo.LayerInfo));
		//}
		Landscape->CreateLandscapeInfo();
		// Create folder.
		FActorFolders::Get().CreateFolder(*CurrentWorld, OutlinerRootName);
		Landscape->SetFolderPath(OutlinerRootName);
	}

	for (int32 x = 0; x < NumberOfTiles.X; x ++)
	{
		for (int32 y = 0; y < NumberOfTiles.Y; y++)
		{
			check(LandscapeGuid.IsValid())

			FString TileID = FString::Printf(TEXT("X%d_Y%d"), x, y);
			FString MapFileName = WorldRootPath / FString("StreamingLevels") / FString("Level_") + TileID + FPackageName::GetMapPackageExtension();
			ULevelStreaming* LevelStreaming = UEditorLevelUtils::CreateNewStreamingLevelForWorld(*CurrentWorld, ULevelStreamingDynamic::StaticClass(), MapFileName);
			UWorld* LevelStreamingWorld = LevelStreaming->GetWorld();
			check(LevelStreamingWorld);
			//AActor* LandscapeProxy = LevelStreamingWorld->SpawnActor<AActor>();
			ALandscapeProxy* LandscapeProxy = Cast<ALandscapeStreamingProxy>(LevelStreamingWorld->SpawnActor<ALandscapeStreamingProxy>());

			TArray<uint16> HeightmapData;
			TArray<FLandscapeImportLayerInfo> MaterialLayerData;
			HeightmapData.Init(LandscapeDataAccess::MidValue, 505*505*sizeof(uint16));
			TMap<FGuid, TArray<uint16>> HeightmapDataPerLayers;
			TMap<FGuid, TArray<FLandscapeImportLayerInfo>> MaterialLayerDataPerLayer;

			HeightmapDataPerLayers.Add(FGuid(), HeightmapData);
			MaterialLayerDataPerLayer.Add(FGuid(), MaterialLayerData);

			LandscapeProxy->Import(LandscapeGuid, 0, 0, 505, 505, 1, 63,
				HeightmapDataPerLayers, TEXT("E:/SubLevels.png"), MaterialLayerDataPerLayer, ELandscapeImportAlphamapType::Additive);

			// Calculate this tile offset from world origin
			FIntRect NewLandscapeRect = LandscapeProxy->GetBoundingRect();
			float WidthX = NewLandscapeRect.Width()*100;
			float WidthY = NewLandscapeRect.Height()*100;
			FIntPoint TileOffset = FIntPoint(x*WidthX, y*WidthY);

			if (LandscapeProxy)
			{
				// Calculate new section coordinates for landscape
				FVector DrawScale = LandscapeProxy->GetRootComponent()->GetRelativeScale3D();
				FIntPoint QuadsSpaceOffset;
				QuadsSpaceOffset.X = FMath::RoundToInt(TileOffset.X / DrawScale.X);
				QuadsSpaceOffset.Y = FMath::RoundToInt(TileOffset.Y / DrawScale.Y);
				LandscapeProxy->SetAbsoluteSectionBase(QuadsSpaceOffset + LandscapeProxy->LandscapeSectionOffset);
				ULandscapeInfo::RecreateLandscapeInfo(LevelStreamingWorld, true);
			}
			LandscapeProxy->SetActorLabel(FString("LandscapeStreamingProxy_") + TileID);
			LandscapeProxy->SetFolderPath(FName(OutlinerRootName.ToString()));
		}
	}
}

void AOpenWorldStreamingActor::UpdateStreamingSetting()
{
}

// Called when the game starts or when spawned
void AOpenWorldStreamingActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AOpenWorldStreamingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

