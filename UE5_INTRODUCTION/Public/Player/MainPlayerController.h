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
};
