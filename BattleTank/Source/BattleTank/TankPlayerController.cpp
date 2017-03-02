// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "Tank.h"
#include "TankPlayerController.h"


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto ControlledTank = GetControlledTank(); // Gets name of tank the player is possesing

	if (!ControlledTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController Not Possesing A Tank!"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Controller Posessing: %s"), *ControlledTank->GetName());

	}

}

ATank* ATankPlayerController::GetControlledTank() const 
{
	return Cast<ATank>(GetPawn()); // returns actual name of possesed tank 
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
}


void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetControlledTank()) { return;  }
		
	FVector HitLocation; // OUT Parameter for getting line trace hit location

	if (GetSightRayHitLocation(HitLocation))
	{
		GetControlledTank()->AimAt(HitLocation);
	}
}


bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
	int32 ViewportSizeX, ViewportSizeY; // Out parameters for getting size of viewport

	GetViewportSize(ViewportSizeX, ViewportSizeY); // sets variables size

	FVector2D ScreenLocation = FVector2D(ViewportSizeX * CrossHairXLocation, ViewportSizeY * CrossHairYLocation); // Sets ScreenLocation to position of crosshair

	FVector LookDirection; // Out parameter for look direction

	if (GetLookDirection(ScreenLocation, LookDirection)) // Look direction gets set here
	{
		GetLookVectorLocation(LookDirection, HitLocation);
	}



	return true;
}

bool ATankPlayerController::GetLookVectorLocation(FVector LookDirection, FVector& HitLocation) const
{
	
	FHitResult HitResult; // Out parameter for where the trace hits

	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility)) // Sets hit location and traces line.
	{
		HitLocation = HitResult.Location;
		if(DrawAimTrace)
			DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor(255, 0, 0), false, -1, 0, 10);

		return true;

	}
	else // doesnt hit anything visible, everything = 0;
	{
		HitLocation = FVector(0.0f);
		return false;
	}
}



bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
	FVector CameraWorldLocation; // placeholder

	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, LookDirection); // returns de projected crosshair location.
	

}