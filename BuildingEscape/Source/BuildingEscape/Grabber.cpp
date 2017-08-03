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

}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandle();
	FindInputComp();
}


void UGrabber::Grab()
{
	UE_LOG(LogTemp, Error, TEXT("GRAB PRESSED"));

	//linetracetry to reach actors with physBody collision channel
	auto hitResult = GetFirstPhysBodyInReach();
	auto compToGrab = hitResult.GetComponent();
	auto actorHit = hitResult.GetActor();

	//if hit then attach phys handle
	if (actorHit) 
	{
		physHandle->GrabComponentAtLocationWithRotation(
			compToGrab,
			NAME_None,
			compToGrab->GetOwner()->GetActorLocation(),
			compToGrab->GetOwner()->GetActorRotation()
		);
	}
	//TODO grab physics handle
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Error, TEXT("GRAB RELEASED"));
	//TODO Release physics handle
	physHandle->ReleaseComponent();
}

void UGrabber::FindPhysicsHandle()
{
	physHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (physHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("Physics Handle found"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Physics handle on %s not found, missing component? "), *GetOwner()->GetName());
	}
}

void UGrabber::FindInputComp()
{
	inputComp = GetOwner()->FindComponentByClass<UInputComponent>();
	if (inputComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Component Attached"));
		inputComp->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComp->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UinputComponent on %s not found, missing component? "), *GetOwner()->GetName());
	}

}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//get player viewpoint on tick
	FVector viewLocation;
	FRotator viewRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT viewLocation, OUT viewRotation);

	reach = 100.f;
	FVector lineTraceEnd = viewLocation + (viewRotation.Vector() * reach);

	//if physhandle attached, move object that is held

	if (physHandle->GrabbedComponent)
	{
		physHandle->SetTargetLocation(lineTraceEnd);
	}

	//see what is hit
}

const FHitResult UGrabber::GetFirstPhysBodyInReach()
{
	//get player viewpoint on tick
	FVector viewLocation;
	FRotator viewRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT viewLocation, OUT viewRotation);

	reach = 100.f;
	FVector lineTraceEnd = viewLocation + (viewRotation.Vector() * reach);

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
	return Hit;
}

