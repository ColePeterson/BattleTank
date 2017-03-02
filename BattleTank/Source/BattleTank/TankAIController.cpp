// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "Tank.h"
#include "TankAIController.h"


void ATankAIController::Tick(float DeltaTime)
{
	if (GetPlayerTank())
	{
		GetControlledTank()->AimAt(GetPlayerTank()->GetActorLocation());
	}
	
}

ATank* ATankAIController::GetControlledTank() const
{

	return Cast<ATank>(GetPawn());

	
}

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();


	auto PlayerTank = GetPlayerTank();

	if (!PlayerTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Cant Find Player Tank!"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Controller Found: %s"), *PlayerTank->GetName());

	}
	

}

ATank * ATankAIController::GetPlayerTank() const
{

	auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!PlayerPawn)
	{
		return nullptr;

	}
	else
		return Cast<ATank>(PlayerPawn);
}
