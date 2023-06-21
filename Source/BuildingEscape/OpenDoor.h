// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);

private:

	FRotator InitialRotation;
	FRotator CurrentRotation;
	FRotator DoorOpening;
	float DoorLastOpen = 0.f;

	UPROPERTY(EditAnywhere)
	float TargetYawRotation = 90.f;
	UPROPERTY(EditAnywhere)
	float DoorOpeningSpeedMultiplier = 1.f;
	UPROPERTY(EditAnywhere)
	float DoorClosingSpeedMultiplier = 1.f;
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 1.f;
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;
	UPROPERTY(EditAnywhere)
	AActor* ActorThatOpensDoor;

		
};
