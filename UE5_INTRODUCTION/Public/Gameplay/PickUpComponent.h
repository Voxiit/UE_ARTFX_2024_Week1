// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PickUpComponent.generated.h"

UENUM()
enum class EPickUpType : uint8
{
	Normal,
	DestroyAfterThrow,
	DestroyAfterPickUp,
	Max UMETA(Hidden),
};

USTRUCT(BlueprintType)
struct FPickUpStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPickUpType PickUpType = EPickUpType::Normal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "1.0", ClampMax = "10.0", EditCondition = "PickUpType != EPickUpType::Normal", EditConditionHides))
	float DestructionTimer = 5.f;

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPickUpDestroyDelegate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5_INTRODUCTION_API UPickUpComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPickUpComponent();

protected:
	UPROPERTY(EditAnywhere, Category = "Pick Up")
	FPickUpStruct PickUpStruct;
		

// Detonation
protected:
	FTimerHandle ProjectileDestructionTimerHandle;

public:
	void StartPickUpDetonationTimer();
	void DestroyPickUp();
	EPickUpType GetPickUpType();
	void ClearTimer();

	FOnPickUpDestroyDelegate OnPickUpDestroy;

// End of Detonation
};
