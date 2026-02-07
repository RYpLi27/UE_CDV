#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/SoftObjectPtr.h"
#include "FloatingIslandGenerator.generated.h"

class UHierarchicalInstancedStaticMeshComponent;

UCLASS()
class UE_CDV_API AFloatingIslandGenerator : public AActor
{
	GENERATED_BODY()

public:
	AFloatingIslandGenerator();

	// Przyciski w panelu Szczegoly
	UFUNCTION(CallInEditor, Category = "Wyspa")
	void Generuj();

	UFUNCTION(CallInEditor, Category = "Wyspa")
	void Wyczysc();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UHierarchicalInstancedStaticMeshComponent* HISM_Cube;

	UPROPERTY(VisibleAnywhere)
	UHierarchicalInstancedStaticMeshComponent* HISM_Ramp;

	UPROPERTY(VisibleAnywhere)
	UHierarchicalInstancedStaticMeshComponent* HISM_Pyramid;

public:
	UPROPERTY(EditAnywhere, Category = "Wyspa|Modele")
	TSoftObjectPtr<UStaticMesh> CubeMesh;

	UPROPERTY(EditAnywhere, Category = "Wyspa|Modele")
	TSoftObjectPtr<UStaticMesh> RampMesh;

	UPROPERTY(EditAnywhere, Category = "Wyspa|Modele")
	TSoftObjectPtr<UStaticMesh> PyramidMesh;

	UPROPERTY(EditAnywhere, Category = "Wyspa|Parametry")
	bool AutoGeneruj = true;

	UPROPERTY(EditAnywhere, Category = "Wyspa|Parametry")
	int32 Seed = 1337;

	UPROPERTY(EditAnywhere, Category = "Wyspa|Siatka")
	int32 GridX = 48;

	UPROPERTY(EditAnywhere, Category = "Wyspa|Siatka")
	int32 GridY = 48;

	UPROPERTY(EditAnywhere, Category = "Wyspa|Siatka")
	float BlockSize = 100.f;

	UPROPERTY(EditAnywhere, Category = "Wyspa|Ksztalt")
	int32 MaxHeight = 14;

	UPROPERTY(EditAnywhere, Category = "Wyspa|Ksztalt")
	float Radius01 = 0.85f;

	UPROPERTY(EditAnywhere, Category = "Wyspa|Wygladzanie")
	int32 TerraceStep = 2;

	UPROPERTY(EditAnywhere, Category = "Wyspa|Korekty")
	float RampYawOffset = 0.f;

	UPROPERTY(EditAnywhere, Category = "Wyspa|Korekty")
	float PyramidYawOffset = 0.f;

private:
	TArray<int32> HeightMap;

	int32 Idx(int32 X, int32 Y) const;
	bool InBounds(int32 X, int32 Y) const;

	void LoadMeshes();
	void BuildHeightMap();
	void EmitCubes();
	void EmitRamps();
	void EmitPyramids();
};
