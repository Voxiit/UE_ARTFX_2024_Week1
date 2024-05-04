// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MainPlayerController.h"

#include "Player/MainCharacter.h"
#include "Controller/GravityGunController.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"

void AMainPlayerController::MovePlayer(const FInputActionValue& Value)
{
	// No need to compute if no character
	if (!Character)
		return;

	// Get movement value and player's rotation
	const FVector2D MoveValue = Value.Get<FVector2D>();

	// Check if the player wants to move in forward/backward direction
	if (MoveValue.Y != 0.f)
	{
		Character->AddMovementInput(Character->GetActorForwardVector(), MoveValue.Y);
	}

	// Check if the player wants to move in Right/Left direction
	if (MoveValue.X != 0.f)
	{
		Character->AddMovementInput(Character->GetActorRightVector(), MoveValue.X);
	}
}

void AMainPlayerController::Look(const FInputActionValue& Value)
{
	// No need to compute if no character
	if (!Character)
	{
		return;
	}

	const FVector2D LookValue = Value.Get<FVector2D>();

	if (LookValue.X != 0.f)
	{
		Character->AddControllerYawInput(LookValue.X);
	}

	if (LookValue.Y != 0.f)
	{
		Character->AddControllerPitchInput(-LookValue.Y);
	}
}

void AMainPlayerController::AddPitchInput(float Val)
{
	float Multiplier = Val * MouseSensitivityY;
	Super::AddPitchInput(Multiplier);
}

void AMainPlayerController::AddYawInput(float Val)
{
	float Multiplier = Val * MouseSensitivityX;
	Super::AddYawInput(Multiplier);
}

void AMainPlayerController::Jump()
{
	if (Character)
	{
		Character->Jump();
	}
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Get the local player subsystem
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	// Clear out existing mapping, and add our mapping
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	// Cast the Input Component to use it as EnhancedInputComponent
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
	EnhancedInput->ClearActionBindings();

	// Bind Movements Inputs
	EnhancedInput->BindAction(InputActionMove, ETriggerEvent::Triggered, this, &AMainPlayerController::MovePlayer);
	EnhancedInput->BindAction(InputActionLook, ETriggerEvent::Triggered, this, &AMainPlayerController::Look);
	EnhancedInput->BindAction(InputActionJump, ETriggerEvent::Triggered, this, &AMainPlayerController::Jump);

}

void AMainPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	// Uknow bug, for a reason, SetPawn is called multiple times...
	// Small fix
	if (Character)
	{
		return;
	}

	Character = Cast<AMainCharacter>(InPawn);
	if (Character)
	{
		GravityGunController = GetComponentByClass<UGravityGunController>();
		if (GravityGunController)
		{
			GravityGunController->SetupInputComponentGravityGun(InputComponent, Character);
		}
	}
}
