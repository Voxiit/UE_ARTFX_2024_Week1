// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GravityGunComponent.generated.h"


UCLASS(Abstract,Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UE5_INTRODUCTION_API UGravityGunComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGravityGunComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OnTakeObjectInputPressed();
	void OnThrowObjectInputPressed();
	void OnThrowObjectInputReleased();

protected:
	TWeakObjectPtr<class AMainCharacter> Character = nullptr;
	TWeakObjectPtr<class APlayerCameraManager> CameraManager = nullptr;


// Collision
protected:
	UPROPERTY(EditAnywhere, Category = "GravityGun|Collision")
	TEnumAsByte<ETraceTypeQuery> SimpleCollisionTraceChannel;
	ECollisionChannel GravityGunCollisionChannel;

	UPROPERTY(EditAnywhere, Category = "GravityGun|Collision", meta = (ClampMin = "0.0", ClampMax = "3000.0"))
	float RaycastSize = 500.f;
	float InitialRaycastSize = 0.f;

// End of Collision	


// Pick Up
protected:
	class AActor* CurrentPickUp = nullptr;
	class UPickUpComponent* CurrentPickUpComponent = nullptr;

// End of Pick Up


// Exercice 1
protected:
	UPROPERTY(EditAnywhere, Category = "GravityGun|Collision", meta = (ClampMin = "500.0", ClampMax = "3000.0"))
	float RaycastMaxSize = 2000.f;
	UPROPERTY(EditAnywhere, Category = "GravityGun|Collision", meta = (ClampMin = "100.0", ClampMax = "3000.0"))
	float RaycastVariationByInput = 100.f;

public:
	void OnIncreaseRaycastSize();
	void OnDecreaseRaycastSize();

// End of Exercice 1


// Debug
protected:
	UPROPERTY(EditAnywhere, Category = "GravityGun|Debug")
	bool bDrawDebugRaycast = false;
	UPROPERTY(EditAnywhere, Category = "GravityGun|Debug", meta = (ClampMin = "0.1", ClampMax = "30.0"))
	float TimerDebugRaycast = 5.f;

// End of Debug

};
