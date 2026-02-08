// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PulsatingActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class UE_CDV_API APulsatingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APulsatingActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Visual mesh for the actor */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	/** Max scale multiplier during pulsation (e.g. 0.5 = 50% increase) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pulsation")
	float PulseIntensity = 0.5f;

	/** Frequency/Speed of the pulsation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pulsation")
	float PulseSpeed = 2.0f;

	/** If true, the actor will shrink and return to base. If false, it will grow and return to base. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pulsation")
	bool bShrinkMode = false;

private:
	/** Original scale saved at BeginPlay */
	FVector BaseScale;
};
