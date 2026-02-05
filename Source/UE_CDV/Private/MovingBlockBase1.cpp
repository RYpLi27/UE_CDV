#include "MovingBlockBase1.h"

AMovingBlockBase1::AMovingBlockBase1()
{
	// To pozwala na ruch w ka¿dej klatce (Tick)
	PrimaryActorTick.bCanEverTick = true;
}

void AMovingBlockBase1::BeginPlay()
{
	Super::BeginPlay();

	// Zapamiêtujemy gdzie postawiliœmy klocka na mapie
	InitialLocation = GetActorLocation();
}

void AMovingBlockBase1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Matematyka ruchu: Sinus(Czas * Szybkoœæ) * Dystans
	float Time = GetWorld()->GetTimeSeconds();
	float MovementOffset = FMath::Sin(Time * Speed) * Distance;

	FVector NewLocation = InitialLocation;

	// Sprawdzamy, które osie u¿ytkownik zaznaczy³ w edytorze
	if (bMoveX) NewLocation.X += MovementOffset;
	if (bMoveY) NewLocation.Y += MovementOffset;
	if (bMoveZ) NewLocation.Z += MovementOffset;

	// Przesuwamy obiekt
	SetActorLocation(NewLocation);
}