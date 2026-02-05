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

	// --- USTAWIENIA DLA CIEBIE ---

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bMoveX = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bMoveY = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bMoveZ = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Distance = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Speed = 2.0f;

private:
	FVector InitialLocation;
};