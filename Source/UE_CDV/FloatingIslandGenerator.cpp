#include "FloatingIslandGenerator.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Engine/StaticMesh.h"

AFloatingIslandGenerator::AFloatingIslandGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	HISM_Cube = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HISM_Cube"));
	HISM_Cube->SetupAttachment(Root);

	HISM_Ramp = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HISM_Ramp"));
	HISM_Ramp->SetupAttachment(Root);

	HISM_Pyramid = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("HISM_Pyramid"));
	HISM_Pyramid->SetupAttachment(Root);

	// Twoje sciezki
	CubeMesh = TSoftObjectPtr<UStaticMesh>(FSoftObjectPath(TEXT("/Game/LevelPrototyping/Meshes/SM_Cube.SM_Cube")));
	RampMesh = TSoftObjectPtr<UStaticMesh>(FSoftObjectPath(TEXT("/Game/LevelPrototyping/Meshes/SM_Ramp.SM_Ramp")));
	PyramidMesh = nullptr; // ustawisz w Szczegoly aktora, jesli trzeba
}

void AFloatingIslandGenerator::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (AutoGeneruj)
	{
		Generuj();
	}
}

void AFloatingIslandGenerator::LoadMeshes()
{
	if (CubeMesh.IsValid())
		HISM_Cube->SetStaticMesh(CubeMesh.LoadSynchronous());

	if (RampMesh.IsValid())
		HISM_Ramp->SetStaticMesh(RampMesh.LoadSynchronous());

	if (PyramidMesh.IsValid())
		HISM_Pyramid->SetStaticMesh(PyramidMesh.LoadSynchronous());
}

void AFloatingIslandGenerator::Wyczysc()
{
	HISM_Cube->ClearInstances();
	HISM_Ramp->ClearInstances();
	HISM_Pyramid->ClearInstances();
}

void AFloatingIslandGenerator::Generuj()
{
	LoadMeshes();
	Wyczysc();

	if (!HISM_Cube->GetStaticMesh())
	{
		UE_LOG(LogTemp, Error, TEXT("Brak SM_Cube - sprawdz CubeMesh"));
		return;
	}

	BuildHeightMap();
	EmitCubes();
	EmitRamps();
	EmitPyramids();
}

int32 AFloatingIslandGenerator::Idx(int32 X, int32 Y) const
{
	return Y * GridX + X;
}

bool AFloatingIslandGenerator::InBounds(int32 X, int32 Y) const
{
	return X >= 0 && Y >= 0 && X < GridX && Y < GridY;
}

void AFloatingIslandGenerator::BuildHeightMap()
{
	HeightMap.SetNumZeroed(GridX * GridY);

	const FVector2D Center((GridX - 1) * 0.5f, (GridY - 1) * 0.5f);
	const float R = FMath::Min(GridX, GridY) * 0.5f * Radius01;

	for (int32 y = 0; y < GridY; y++)
	{
		for (int32 x = 0; x < GridX; x++)
		{
			float D = FVector2D::Distance(FVector2D(x, y), Center);
			if (D > R) continue;

			float T = 1.f - (D / R);
			int32 H = FMath::Clamp(FMath::RoundToInt(T * MaxHeight), 1, MaxHeight);

			if (TerraceStep > 1)
				H = (H / TerraceStep) * TerraceStep;

			HeightMap[Idx(x, y)] = H;
		}
	}
}

void AFloatingIslandGenerator::EmitCubes()
{
	for (int32 y = 0; y < GridY; y++)
	{
		for (int32 x = 0; x < GridX; x++)
		{
			int32 H = HeightMap[Idx(x, y)];
			for (int32 z = 0; z < H; z++)
			{
				FVector Pos(x * BlockSize, y * BlockSize, -z * BlockSize);
				HISM_Cube->AddInstance(FTransform(Pos));
			}
		}
	}
}

void AFloatingIslandGenerator::EmitRamps()
{
	if (!HISM_Ramp->GetStaticMesh()) return;

	auto H = [&](int32 x, int32 y)
		{
			return InBounds(x, y) ? HeightMap[Idx(x, y)] : 0;
		};

	for (int32 y = 0; y < GridY; y++)
	{
		for (int32 x = 0; x < GridX; x++)
		{
			int32 h = H(x, y);
			if (h <= 0) continue;

			if (h - H(x + 1, y) == 1)
				HISM_Ramp->AddInstance(FTransform(FRotator(0, 180 + RampYawOffset, 0),
					FVector((x + 1) * BlockSize, y * BlockSize, -(h - 1) * BlockSize)));

			if (h - H(x - 1, y) == 1)
				HISM_Ramp->AddInstance(FTransform(FRotator(0, 0 + RampYawOffset, 0),
					FVector((x - 1) * BlockSize, y * BlockSize, -(h - 1) * BlockSize)));

			if (h - H(x, y + 1) == 1)
				HISM_Ramp->AddInstance(FTransform(FRotator(0, -90 + RampYawOffset, 0),
					FVector(x * BlockSize, (y + 1) * BlockSize, -(h - 1) * BlockSize)));

			if (h - H(x, y - 1) == 1)
				HISM_Ramp->AddInstance(FTransform(FRotator(0, 90 + RampYawOffset, 0),
					FVector(x * BlockSize, (y - 1) * BlockSize, -(h - 1) * BlockSize)));
		}
	}
}

void AFloatingIslandGenerator::EmitPyramids()
{
	if (!HISM_Pyramid->GetStaticMesh()) return;

	auto H = [&](int32 x, int32 y)
		{
			return InBounds(x, y) ? HeightMap[Idx(x, y)] : 0;
		};

	for (int32 y = 0; y < GridY; y++)
	{
		for (int32 x = 0; x < GridX; x++)
		{
			int32 h = H(x, y);
			if (h <= 0) continue;

			int32 dE = h - H(x + 1, y);
			int32 dW = h - H(x - 1, y);
			int32 dN = h - H(x, y + 1);
			int32 dS = h - H(x, y - 1);

			FVector Pos(x * BlockSize, y * BlockSize, -(h - 1) * BlockSize);

			if (dE == 1 && dN == 1)
				HISM_Pyramid->AddInstance(FTransform(FRotator(0, 0 + PyramidYawOffset, 0), Pos));
			else if (dE == 1 && dS == 1)
				HISM_Pyramid->AddInstance(FTransform(FRotator(0, 90 + PyramidYawOffset, 0), Pos));
			else if (dW == 1 && dS == 1)
				HISM_Pyramid->AddInstance(FTransform(FRotator(0, 180 + PyramidYawOffset, 0), Pos));
			else if (dW == 1 && dN == 1)
				HISM_Pyramid->AddInstance(FTransform(FRotator(0, 270 + PyramidYawOffset, 0), Pos));
		}
	}
}
