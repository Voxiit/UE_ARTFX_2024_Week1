// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SpawnerController.h"
#include "Player/MainCharacter.h"
#include "Gameplay/Spawner/SpawnerComponent.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"


USpawnerController::USpawnerController()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USpawnerController::SetupInputComponentSpawnerController(TObjectPtr<class UInputComponent> InputComponent, AMainCharacter* InCharacter)
{
	// Get Character and comp
	if (InCharacter)
	{
		Character = InCharacter;
		SpawnerComponent = Character->GetComponentByClass<USpawnerComponent>();
	}

	// Cast the Input Component to use it as EnhancedInputComponent
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);

	// Bind actions
	EnhancedInput->BindAction(InputActionSpawnPickUp, ETriggerEvent::Triggered, this, &USpawnerController::OnRecieveSpawnInput);
	EnhancedInput->BindAction(InputActionPickUpInfos, ETriggerEvent::Triggered, this, &USpawnerController::PrintPickUpInfos);
}


void USpawnerController::OnRecieveSpawnInput(const FInputActionValue& Value)
{
	// Check Spawner comp
	if (!SpawnerComponent.IsValid())
	{
		return;
	}

	// Get spawn value
	const FVector2D MoveValue = Value.Get<FVector2D>();

	// X positive = normal pick up
	if (MoveValue.X > 0)
	{
		SpawnerComponent->SpawnPickup(EPickUpType::Normal);
	}

	// X negative = destroy after pick up
	else if (MoveValue.X < 0)
	{
		SpawnerComponent->SpawnPickup(EPickUpType::DestroyAfterPickUp);
	}

	// Y = destroy after throw
	else if (MoveValue.Y != 0)
	{
		SpawnerComponent->SpawnPickup(EPickUpType::DestroyAfterThrow);
	}
}

void USpawnerController::PrintPickUpInfos()
{
	// Check Spawner comp
	if (!SpawnerComponent.IsValid())
	{
		return;
	}

	SpawnerComponent->PrintPickupInfos();
}
