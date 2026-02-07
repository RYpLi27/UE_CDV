#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingBlockBase1.generated.h"

UCLASS()
class UE_CDV_API AMovingBlockBase1 : public AActor
{
	GENERATED_BODY()

public:
	AMovingBlockBase1();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// --- KONFIGURACJA TRASY ---

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Sequence")
	float XDistance = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Sequence")
	float YDistance = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Sequence")
	float ZDistance = 500.0f; // Nowa oœ Z

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Sequence")
	float Speed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Sequence")
	float StartDelay = 0.0f; // Czas oczekiwania przed pierwszym ruchem

private:
	FVector StartLocation;
	TArray<FVector> Targets;
	int32 CurrentTargetIndex = 0;
	float DelayTimer = 0.0f;
	bool bCanMove = false;

	void SetupRoute();
};