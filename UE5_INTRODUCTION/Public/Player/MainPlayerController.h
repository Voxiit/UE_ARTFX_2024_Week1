// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

UCLASS()
class UE5_INTRODUCTION_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;
	virtual void SetPawn(APawn* InPawn) override;

// Character
protected:
	class AMainCharacter* Character = nullptr;
	class UGravityGunController* GravityGunController = nullptr;

// Inputs
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input|Movement")
	class UInputAction* InputActionMove;
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input|Mouse")
	class UInputAction* InputActionLook;

protected:
	void MovePlayer(const struct FInputActionValue& Value);
	void Look(const struct FInputActionValue& Value);

// End of Inputs

// Mouse sensitivity
protected:
	UPROPERTY(EditAnywhere, Category = "Input|Mouse", meta = (ClampMin = "0.1", ClampMax = "2.0"))
	float MouseSensitivityX = 1.f;
	UPROPERTY(EditAnywhere, Category = "Input|Mouse", meta = (ClampMin = "0.1", ClampMax = "2.0"))
	float MouseSensitivityY = 1.f;

public:
	virtual void AddPitchInput(float Val) override;
	virtual void AddYawInput(float Val) override;

// End of Mouse sensitivity


// Jump
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input|Jump")
	class UInputAction* InputActionJump;

protected:
	void Jump();

// End of Jump
};
