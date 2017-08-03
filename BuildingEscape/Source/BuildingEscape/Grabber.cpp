// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber Active"));
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//get player viewpoint on tick
	FVector viewLocation;
	FRotator viewRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT viewLocation, OUT viewRotation);

	UE_LOG(LogTemp, Warning, TEXT("Location: %s  Rotation: %s"), *viewLocation.ToString(), *viewRotation.ToString());

	///debugLine front of viewport via reach distance
	reach = 100.f;
	FVector lineTraceEnd = viewLocation + (viewRotation.Vector() * reach);

	DrawDebugLine(
		GetWorld(),
		viewLocation,
		lineTraceEnd,
		FColor(255, 0, 0),
		false, 
		0.f, 
		0.f,
		10.f
	);

	/// setup line trace parameters
	FHitResult Hit;
	FCollisionObjectQueryParams physBodyQuery;
	FCollisionQueryParams collQuery;
	physBodyQuery = FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody);
	collQuery = FCollisionQueryParams(TEXT(""), false, GetOwner());


	///raycast front of viewport via reach distance
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		viewLocation,
		lineTraceEnd,
		physBodyQuery,
		collQuery
	);

	AActor* hitActor;
	hitActor = Hit.GetActor();
	if (hitActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("HitActor: %s"), *(hitActor->GetName()));
	}

	//see what is hit
}

