// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GravityGunController.generated.h"


UCLASS(Abstract, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5_INTRODUCTION_API UGravityGunController : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGravityGunController();

protected:
	virtual void BeginPlay() override;

protected:
	TWeakObjectPtr<class AMainCharacter> Character = nullptr;
	TWeakObjectPtr<class UGravityGunComponent> GravityGunComponent = nullptr;
		
// Inputs
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	class UInputAction* TakeObjectInputAction;
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	class UInputAction* ThrowObjectInputAction;

public:
	void SetupInputComponentGravityGun(TObjectPtr<class UInputComponent> InputComponent, class AMainCharacter* InCharacter);

protected:
	void OnTakeObjectInputPressed();
	void OnThrowObjectInputTriggered(const struct FInputActionValue& Value);

// End of Inputs


// Exercice 1
protected:
	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	class UInputAction* IncreaseRaycastSizeInputAction;

protected:
	void OnIncreaseRaycastSizeInputTriggered(const struct FInputActionValue& Value);

// End of Exercice 1
};
