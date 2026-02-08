// Fill out your copyright notice in the Description page of Project Settings.


#include "RotatingActor.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
ARotatingActor::ARotatingActor()
{
 	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	// Setup mesh component as root
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	// Initialize variables
	RotationSpeed = FRotator(0.0f, 0.0f, 0.0f);
	StartDelay = 0.0f;
	bCanRotate = false;
}

// Called when the game starts or when spawned
void ARotatingActor::BeginPlay()
{
	Super::BeginPlay();

	if (StartDelay > 0.0f)
	{
		// Use timer to enable rotation after StartDelay
		GetWorldTimerManager().SetTimer(StartRotationTimerHandle, this, &ARotatingActor::OnStartDelayComplete, StartDelay, false);
	}
	else
	{
		// No delay, enable immediately
		bCanRotate = true;
	}
}

// Called every frame
void ARotatingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotate only if the flag is true
	if (bCanRotate)
	{
		AddActorLocalRotation(RotationSpeed * DeltaTime, false, nullptr, ETeleportType::None);
	}
}

void ARotatingActor::OnStartDelayComplete()
{
	bCanRotate = true;
}
