// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotatingActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class UE_CDV_API ARotatingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotatingActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Visual mesh for the actor */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	/** Rotation speed per axis in degrees per second */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation Settings")
	FRotator RotationSpeed;

	/** Delay in seconds before rotation starts */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation Settings")
	float StartDelay = 0.0f;

	/** Flag controlling if the object can rotate */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation Settings")
	bool bCanRotate = false;

private:
	/** Function called by timer to enable rotation */
	UFUNCTION()
	void OnStartDelayComplete();

	/** Handle for the start delay timer */
	FTimerHandle StartRotationTimerHandle;
};
