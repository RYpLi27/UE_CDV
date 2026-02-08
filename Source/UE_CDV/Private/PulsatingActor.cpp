// Fill out your copyright notice in the Description page of Project Settings.


#include "PulsatingActor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APulsatingActor::APulsatingActor()
{
 	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	// Setup mesh component as root
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	// Default values
	PulseIntensity = 0.5f;
	PulseSpeed = 2.0f;
}

// Called when the game starts or when spawned
void APulsatingActor::BeginPlay()
{
	Super::BeginPlay();

	// Capture the initial scale
	BaseScale = GetActorScale3D();
}

// Called every frame
void APulsatingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Formula that oscillates Alpha between 0.0 and 1.0
	// 1.5707f (PI/2) shift ensures we start at 0.0 value (BaseScale)
	float SinValue = FMath::Sin(GetWorld()->GetTimeSeconds() * PulseSpeed - 1.5707f);
	float Alpha = (SinValue + 1.0f) * 0.5f;

	// Calculate Target Scale based on Mode
	FVector TargetScale = bShrinkMode ? 
		(BaseScale * (1.0f - PulseIntensity)) : 
		(BaseScale * (1.0f + PulseIntensity));

	// Perfectly interpolate between Base and Target
	FVector NewScale = FMath::Lerp(BaseScale, TargetScale, Alpha);

	// Apply the new scale
	SetActorScale3D(NewScale);
}
