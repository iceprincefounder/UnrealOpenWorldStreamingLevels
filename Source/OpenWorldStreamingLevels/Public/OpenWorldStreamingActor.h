// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OpenWorldStreamingActor.generated.h"

UCLASS(NotBlueprintable, HideCategories = (Replication, Collision, Input, LOD, Rendering, Cooking))
class OPENWORLDSTREAMINGLEVELS_API AOpenWorldStreamingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOpenWorldStreamingActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StreamingLevels)
	FIntPoint NumberOfTiles = FIntPoint(2, 2);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StreamingLevels)
	FIntPoint Resolution = FIntPoint(505, 505);

	UFUNCTION(BlueprintCallable, CallInEditor, Category = StreamingLevels)
	virtual void CreateStreamingLevels();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = StreamingLevels)
	virtual void UpdateStreamingSetting();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
