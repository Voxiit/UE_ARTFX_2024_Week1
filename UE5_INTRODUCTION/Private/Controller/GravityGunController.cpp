// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/GravityGunController.h"

#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"

UGravityGunController::UGravityGunController()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UGravityGunController::BeginPlay()
{
	Super::BeginPlay();
	
}

void UGravityGunController::SetupInputComponentGravityGun(TObjectPtr<class UInputComponent> InputComponent)
{
	// Cast the Input Component to use it as EnhancedInputComponent
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);

	// Bind actions
	EnhancedInput->BindAction(TakeObjectInputAction, ETriggerEvent::Triggered, this, &UGravityGunController::OnTakeObjectInputPressed);
	EnhancedInput->BindAction(ThrowObjectInputAction, ETriggerEvent::Triggered, this, &UGravityGunController::OnThrowObjectInputTriggered);

}

void UGravityGunController::OnTakeObjectInputPressed()
{
}

void UGravityGunController::OnThrowObjectInputTriggered(const FInputActionValue& Value)
{
}
