// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TrackGenerator.generated.h"

USTRUCT(BlueprintType)
struct FRoadData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool LeftGuardRail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool RightGuardRail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TrackBank;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TrackWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TrackThickness;

	FRoadData()
	{
		LeftGuardRail = false;
		RightGuardRail = false;
		TrackBank = 0.f;
		TrackWidth = 1.f;
		TrackThickness = 1.f;
	}
};

UCLASS()
class VEHICLESPLINE_API ATrackGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USplineComponent* SplineComponent;

	// Sets default values for this actor's properties
	ATrackGenerator();

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	TArray<FRoadData> RoadDataArray;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	bool LoopingTrack = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Meshes")
	UStaticMesh* RoadMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Meshes")
	UStaticMesh* LeftGuardMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Meshes")
	UStaticMesh* RightGuardMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool Collisions = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool DrawTrackPointNumbers = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Debug")
	UMaterialInterface* TrackPointNumberMaterial = nullptr;

protected:
	virtual void BuildTrackElement(int32 LoopIndex, UStaticMesh* RoadElementMesh);

private:
	int32 NumberOfSplinePoints = 0;
};
