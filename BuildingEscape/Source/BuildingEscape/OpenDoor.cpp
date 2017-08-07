// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	if (!pressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing pressure plate"), *GetOwner()->GetName());
	}
}




// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	//if total mass of actors on pressure plate > something then allow the door open
	//poll trigger volume
	if (GetTotalMassOnPlate() > triggerMass)
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}


}

float UOpenDoor::GetTotalMassOnPlate()
{
	float totalMass = 0.f;

	//find all the overlapping actors
	TArray<AActor*> overlappingActors;
	if (!pressurePlate) { return totalMass; }
	pressurePlate->GetOverlappingActors(OUT overlappingActors);

	for (const auto* actor : overlappingActors)
	{
		UPrimitiveComponent * act = Cast<UPrimitiveComponent>(actor->GetRootComponent());
		totalMass += act->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *actor->GetName());
	}


	return totalMass;
}

