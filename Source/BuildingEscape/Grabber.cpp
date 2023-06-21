// Fill out your copyright notice in the Description page of Project Settings.

 	
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

#define OUT // Macro to highlight OUT parameters.

// Sets default values for this component's properties.
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle(); // Prevents crashes when a Physics Handle is not on the same actor as Grabber.
	SetupInputComponent(); // Setup for the Input Component and to attach it to the grab and release functions.
}

// Setup for the Input Component and to attach it to the grab and release functions.
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab); // Attach InputComponent to the Grab function.
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release); // Attach InputComponent to the Release function.
	}
}

// Prevents crashes when a Physics Handle is not on the same actor as Grabber.
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		//Physics handle is found.
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Physics Handle component is not a part of %s!"), *GetOwner()->GetName()); //Error, no Physics Handle component on the Actor.
	}
}

// Grab action function.
void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Pressed"));

	FHitResult HitResult = GetFirstPhysicsBodyInReach();

	FVector LineTraceEnd = PlayerMaximumReach(); // Vector of how far a Player may Grab an object.

	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	if (HitResult.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, LineTraceEnd); // Grab action.
	}
}

// Release action function.
void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Relesead"));

	PhysicsHandle->ReleaseComponent();

}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector LineTraceEnd = PlayerMaximumReach(); // Vector of how far a Player may Grab an object.

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd); // Set the object location at the farthest point a Player can reach.
	}
	

}

// Get the Physics Body that is in Reach of the Player. Returns FHitResult.
FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	FHitResult Hit;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner()); // Set up Collision parameters.
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, PlayerViewPointLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);

	AActor* ActorHit = Hit.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Actor is hit!"), *(ActorHit->GetName()));
	}
	
	return Hit;

}

// The Player maximum reach. Returns an FVector.
FVector UGrabber::PlayerMaximumReach()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation); // Location and Rotation of the Player Actor.

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach; // Vector of how far a Player may Grab an object.

	return LineTraceEnd;
}
