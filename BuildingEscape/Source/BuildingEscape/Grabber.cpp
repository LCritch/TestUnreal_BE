// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
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
	//linetrace try to reach actors with physBody collision channel
	auto hitResult = GetFirstPhysBodyInReach();
	auto compToGrab = hitResult.GetComponent();
	auto actorHit = hitResult.GetActor();

	///if hit then attach phys handle
	if (actorHit) 
	{
		if (!physHandle) { return; }
		physHandle->GrabComponentAtLocationWithRotation(
			compToGrab,
			NAME_None,
			compToGrab->GetOwner()->GetActorLocation(),
			compToGrab->GetOwner()->GetActorRotation()
		);
	}
}

void UGrabber::Release()
{
	if (!physHandle) { return; }
	physHandle->ReleaseComponent();
}

void UGrabber::FindPhysicsHandle()
{
	physHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (physHandle == nullptr)
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


	//if physhandle attached, move object that is held
	if (!physHandle) { return; }
	if (physHandle->GrabbedComponent)
	{
		physHandle->SetTargetLocation(GetReachLineEnd());
	}
}

FVector UGrabber::GetReachLineEnd()
{
	FVector viewLocation;
	FRotator viewRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT viewLocation, OUT viewRotation);

	return viewLocation + (viewRotation.Vector() * reach);

}

FVector UGrabber::GetReachLineStart()
{
	FVector viewLocation;
	FRotator viewRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT viewLocation, OUT viewRotation);

	return viewLocation;

}

const FHitResult UGrabber::GetFirstPhysBodyInReach()
{
	/// setup line trace parameters
	FHitResult Hit;
	FCollisionObjectQueryParams physBodyQuery;
	FCollisionQueryParams collQuery;
	physBodyQuery = FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody);
	collQuery = FCollisionQueryParams(TEXT(""), false, GetOwner());


	///raycast front of viewport via reach distance
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetReachLineStart(),
		GetReachLineEnd(),
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

