#include "MovingBlockBase1.h"

AMovingBlockBase1::AMovingBlockBase1()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMovingBlockBase1::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation();

	// Ustawiamy timer i sprawdzamy czy mamy startowaæ od razu
	DelayTimer = StartDelay;
	bCanMove = (StartDelay <= 0.0f);

	SetupRoute();
}

void AMovingBlockBase1::SetupRoute()
{
	Targets.Empty();
	// Sekwencja: X -> Y -> Z -> Powrót do Startu
	Targets.Add(StartLocation + FVector(XDistance, 0.0f, 0.0f));                      // Krok 1: X
	Targets.Add(StartLocation + FVector(XDistance, YDistance, 0.0f));                 // Krok 2: X + Y
	Targets.Add(StartLocation + FVector(XDistance, YDistance, ZDistance));            // Krok 3: X + Y + Z
	Targets.Add(StartLocation);                                                       // Krok 4: Powrót do bazy
}

void AMovingBlockBase1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Obs³uga opóŸnienia startu
	if (!bCanMove)
	{
		DelayTimer -= DeltaTime;
		if (DelayTimer <= 0.0f)
		{
			DelayTimer = 0.0f;
			bCanMove = true;
		}
		return; // Przerywamy Tick, dopóki timer nie doliczy do zera
	}

	if (Targets.Num() == 0) return;

	// Bezpieczne zapewnienie indeksu w zakresie
	if (CurrentTargetIndex < 0 || CurrentTargetIndex >= Targets.Num())
	{
		CurrentTargetIndex = 0;
	}

	FVector CurrentLocation = GetActorLocation();
	FVector TargetLocation = Targets[CurrentTargetIndex];

	// Ruch sta³y (FMath::VInterpConstantTo zapewnia, ¿e nie zwalnia przed celem)
	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
	SetActorLocation(NewLocation);

	// Sprawdzanie celu
	const float AcceptanceRadius = 1.0f;
	if (FVector::Dist(NewLocation, TargetLocation) < AcceptanceRadius)
	{
		CurrentTargetIndex++;

		if (CurrentTargetIndex >= Targets.Num())
		{
			CurrentTargetIndex = 0;
		}
	}
}