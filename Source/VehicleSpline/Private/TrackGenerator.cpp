// Fill out your copyright notice in the Description page of Project Settings.

#include "VehicleSpline.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "TrackGenerator.h"


// Sets default values
ATrackGenerator::ATrackGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Road-Spline"));
	SplineComponent->SetupAttachment(RootComponent);
}

void ATrackGenerator::OnConstruction(const FTransform& Transform)
{
	// Store number of spline points
	NumberOfSplinePoints = SplineComponent->GetNumberOfSplinePoints();

	// Build RoadData array
	int32 countDataEntries = RoadDataArray.Num();

	while (countDataEntries != NumberOfSplinePoints)
	{
		if (countDataEntries > NumberOfSplinePoints)
		{
			RoadDataArray.RemoveAt(countDataEntries - 1);
			countDataEntries--;
		}
		else if (countDataEntries < NumberOfSplinePoints)
		{
			FRoadData roadData;
			RoadDataArray.Add(roadData);
			countDataEntries++;
		}
	}

	// Construct Track Mesh
	for (int32 i = 0; i <= NumberOfSplinePoints - (LoopingTrack ? 1 : 2); i++)
	{
		BuildTrackElement(i, RoadMesh);

		if (RoadDataArray[i].LeftGuardRail)
			BuildTrackElement(i, LeftGuardMesh);

		if (RoadDataArray[i].RightGuardRail)
			BuildTrackElement(i, RightGuardMesh);
	}


	// DrawPointNumbers
	if (DrawTrackPointNumbers)
	{
		for (int32 i = 0; i < NumberOfSplinePoints; i++)
		{
			FVector location, tangent;
			SplineComponent->GetLocationAndTangentAtSplinePoint(i, location, tangent, ESplineCoordinateSpace::Local);

			UTextRenderComponent* TextRender = NewObject<UTextRenderComponent>(this);
			TextRender->CreationMethod = EComponentCreationMethod::UserConstructionScript;
			TextRender->SetupAttachment(RootComponent);

			TextRender->SetRelativeLocation(location + FVector::UpVector * 50.0f);
			TextRender->SetWorldRotation(FRotator::ZeroRotator);
			TextRender->SetRelativeScale3D(FVector(8.0f, 8.0f, 8.0f));

			TextRender->TextMaterial = TrackPointNumberMaterial;
			TextRender->SetText(FString::FromInt(i));
			TextRender->SetTextRenderColor(FColor(255, 0, 0));
		}
	}

	RegisterAllComponents();
}

void ATrackGenerator::BuildTrackElement(int32 LoopIndex, UStaticMesh * RoadElementMesh)
{
	int32 nextLoopIndex = (LoopIndex + 1) % NumberOfSplinePoints;
	FVector startLocation, startTangent, endLocation, endTangent;

	SplineComponent->GetLocationAndTangentAtSplinePoint(LoopIndex, startLocation, startTangent, ESplineCoordinateSpace::Local);
	SplineComponent->GetLocationAndTangentAtSplinePoint(nextLoopIndex, endLocation, endTangent, ESplineCoordinateSpace::Local);

	USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this);
	SplineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	SplineMesh->SetupAttachment(RootComponent);

	SplineMesh->SetMobility(EComponentMobility::Movable);

	SplineMesh->SetStaticMesh(RoadElementMesh);
	
	SplineMesh->SetStartAndEnd(startLocation, startTangent, endLocation, endTangent);

	SplineMesh->SetStartRoll(RoadDataArray[LoopIndex].TrackBank);
	SplineMesh->SetEndRoll(RoadDataArray[nextLoopIndex].TrackBank);

	FVector2D startScale = FVector2D(RoadDataArray[LoopIndex].TrackWidth, RoadDataArray[LoopIndex].TrackThickness);
	FVector2D endScale = FVector2D(RoadDataArray[nextLoopIndex].TrackWidth, RoadDataArray[nextLoopIndex].TrackThickness);
	SplineMesh->SetStartScale(startScale);
	SplineMesh->SetEndScale(endScale);

	SplineMesh->SetCollisionEnabled(Collisions ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
}