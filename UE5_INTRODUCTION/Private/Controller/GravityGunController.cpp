// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/GravityGunController.h"

#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"

#include "Gameplay/GravityGunComponent.h"
#include "Player/MainCharacter.h"

UGravityGunController::UGravityGunController()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UGravityGunController::BeginPlay()
{
	Super::BeginPlay();

}

void UGravityGunController::SetupInputComponentGravityGun(TObjectPtr<class UInputComponent> InputComponent, AMainCharacter* InCharacter)
{
	// Get Character and comp
	if (InCharacter)
	{
		Character = InCharacter;
		GravityGunComponent = Character->GetComponentByClass<UGravityGunComponent>();
	}
	
	// Cast the Input Component to use it as EnhancedInputComponent
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);

	// Bind actions
	EnhancedInput->BindAction(TakeObjectInputAction, ETriggerEvent::Triggered, this, &UGravityGunController::OnTakeObjectInputPressed);
	EnhancedInput->BindAction(ThrowObjectInputAction, ETriggerEvent::Triggered, this, &UGravityGunController::OnThrowObjectInputTriggered);

	// Exercice 1
	EnhancedInput->BindAction(IncreaseRaycastSizeInputAction, ETriggerEvent::Triggered, this, &UGravityGunController::OnIncreaseRaycastSizeInputTriggered);
}

void UGravityGunController::OnTakeObjectInputPressed()
{
	if (GravityGunComponent.IsValid())
	{
		GravityGunComponent->OnTakeObjectInputPressed();
	}
}

void UGravityGunController::OnThrowObjectInputTriggered(const FInputActionValue& Value)
{
	// Check if valid
	if (!GravityGunComponent.IsValid())
	{
		return;
	}

	// If the value is postive we're pressing the input
	const float CountValue = Value.Get<float>();
	if (CountValue > 0.f)
	{
		GravityGunComponent->OnThrowObjectInputPressed();
	}
	// Else, we're releasing the input
	else
	{
		GravityGunComponent->OnThrowObjectInputReleased();
	}
}

void UGravityGunController::OnIncreaseRaycastSizeInputTriggered(const FInputActionValue& Value)
{
	// Check if valid
	if (!GravityGunComponent.IsValid())
	{
		return;
	}

	// If the value is postive we're increasing the size
	const float CountValue = Value.Get<float>();
	if (CountValue > 0.f)
	{
		GravityGunComponent->OnIncreaseRaycastSize();
	}
	// Else we're decreasing
	else
	{
		GravityGunComponent->OnDecreaseRaycastSize();
	}
}
