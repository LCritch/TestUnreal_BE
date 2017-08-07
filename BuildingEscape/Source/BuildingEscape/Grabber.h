// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	//how far is the distance the player can reach
	float reach = 100.f;

	UPhysicsHandleComponent* physHandle = nullptr;
	UInputComponent* inputComp = nullptr;
	
	///raycast and grab what is within reach
	void Grab();
	void Release();

	//phys handle event
	void FindPhysicsHandle();

	//inputComp find event
	void FindInputComp();

	//return hit for first physBody in reach
	const FHitResult GetFirstPhysBodyInReach();

	///return start and end of line trace
	FVector GetReachLineStart();
	FVector GetReachLineEnd();

};
