// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpawnerController.generated.h"


UCLASS(Abstract, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5_INTRODUCTION_API USpawnerController : public UActorComponent
{
	GENERATED_BODY()

public:	
	USpawnerController();

	void SetupInputComponentSpawnerController(TObjectPtr<class UInputComponent> InputComponent, class AMainCharacter* InCharacter);

protected:
	TWeakObjectPtr<class AMainCharacter> Character = nullptr;
	TWeakObjectPtr<class USpawnerComponent> SpawnerComponent = nullptr;
	
// Inputs
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Spawner|Inputs")
	class UInputAction* InputActionSpawnPickUp;
	UPROPERTY(EditDefaultsOnly, Category = "Spawner|Inputs")
	class UInputAction* InputActionPickUpInfos;

protected:
	void OnRecieveSpawnInput(const struct FInputActionValue& Value);

// End of Inputs


// Debug
protected:
	void PrintPickUpInfos();

// End of Debug
};
