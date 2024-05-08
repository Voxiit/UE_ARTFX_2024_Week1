// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "Gameplay/PickUpComponent.h"

#include "SpawnerComponent.generated.h"

USTRUCT(BlueprintType)
struct FSpawnerPickUpStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> PickUp = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPickUpType PickUpType = EPickUpType::Normal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "1.0", ClampMax = "20.0"))
	int MaxPickUpOfThisType = 10;
	int CurrentPickUpOfThisType = 0;
};

UCLASS(Abstract, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5_INTRODUCTION_API USpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USpawnerComponent();

protected:
	virtual void BeginPlay() override;

protected:
	TWeakObjectPtr<class AMainCharacter> Character = nullptr;

// Pick Up
public:
	void SpawnPickup(EPickUpType PickupType);
	void PrintPickupInfos();

protected:
	UFUNCTION()
	void OnPickupDestroyed(EPickUpType PickupType);

protected:
	UPROPERTY(EditAnywhere, Category = "Spawner|Pickup")
	TArray<FSpawnerPickUpStruct> PickUpArray;
	TStaticArray<FSpawnerPickUpStruct, (uint8)EPickUpType::Max> PickUpStaticArray;

	UPROPERTY(EditAnywhere, Category = "Spawner|Spawn")
	float PickupSpawnOffsetFromPlayer = 200.f;
	UPROPERTY(EditAnywhere, Category = "Spawner|Spawn", meta = (ClampMin = "0.0", ClampMax = "10.0"))
	float DelayBetweenTwoSpawn = 1.f;

	UPROPERTY(EditAnywhere, Category = "Spawner|Pickup", meta = (ClampMin = "0.0", ClampMax = "30.0"))
	unsigned int MaxPickUpNumber = 20;
	unsigned int CurrentPickUpNumber = 0;

// End of Pick Up


// Spawn Pick Up
protected:
	TWeakObjectPtr<class UGravityGunComponent> GravityGunComponent = nullptr;
	TWeakObjectPtr<class APlayerCameraManager> CameraManager = nullptr;

	FTimerHandle PickUpSpawnTimerHandle;
	bool bCanSpawn = true;

protected:
	void ResetTimer();
	void ClearTimer();

// End of Spawn Pick Up
};
