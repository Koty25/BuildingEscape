// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Matinee/InterpTrackInst.h"
#include "Engine/LocalPlayer.h" 

#define OUT // Macro to highlight OUT parameters.

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialRotation = GetOwner()->GetActorRotation();

	TargetYawRotation += InitialRotation.Yaw;

	DoorOpening = InitialRotation;
	DoorOpening.Yaw = TargetYawRotation;

	FindPressurePlate();
	FindAudioComponent();
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Audio component is not a part of %s!"), *GetOwner()->GetName()); //Error, no Audio component on the Actor.
	}
	
}

void UOpenDoor::FindPressurePlate()
{
	if(!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the door component on it, but no PressurePlate set!"), *GetOwner()->GetName())
	}	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassActor() > TotalMassToOpenDoor)
	{
		OpenDoor(DeltaTime);

		DoorLastOpen = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpen > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
		
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentRotation = GetOwner()->GetActorRotation();
	
	DoorOpening.Yaw = FMath::Lerp(CurrentRotation.Yaw, TargetYawRotation, DeltaTime * DoorOpeningSpeedMultiplier);

	GetOwner()->SetActorRotation(DoorOpening);

	if (OpenDoorSound)
	{
		AudioComponent->Play();
		OpenDoorSound = false;
		CloseDoorSound = true;
	}
	
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentRotation = GetOwner()->GetActorRotation();
	
	DoorOpening.Yaw = FMath::Lerp(CurrentRotation.Yaw, InitialRotation.Yaw, DeltaTime * DoorClosingSpeedMultiplier);

	GetOwner()->SetActorRotation(DoorOpening);

	if (CloseDoorSound)
	{
		AudioComponent->Play();
		OpenDoorSound = true;
		CloseDoorSound = false;
	}
}

float UOpenDoor::TotalMassActor()
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	
	for (AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

