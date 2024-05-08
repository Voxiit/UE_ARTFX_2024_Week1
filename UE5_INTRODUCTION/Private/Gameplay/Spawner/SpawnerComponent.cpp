// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Spawner/SpawnerComponent.h"
#include "Gameplay/GravityGunComponent.h"
#include "Player/MainCharacter.h"
#include "Kismet/GameplayStatics.h"

static FName PickUpActorTag = "PICKUP";

USpawnerComponent::USpawnerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void USpawnerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Get Refs
	Character = Cast<AMainCharacter>(GetOwner());
	CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if (Character.IsValid())
	{
		GravityGunComponent = Character->FindComponentByClass<UGravityGunComponent>();
	}

	// Fill our static array
	for (const FSpawnerPickUpStruct& PickUpStruct : PickUpArray)
	{
		if (PickUpStruct.PickUp)
		{
			PickUpStaticArray[(uint8)PickUpStruct.PickUpType] = PickUpStruct;
		}
	}

	// Get all pick up on the map
	TArray<AActor*> PickUps;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), PickUpActorTag, PickUps);
	for (AActor* PickUp : PickUps)
	{
		UPickUpComponent* PickUpComp = PickUp->FindComponentByClass<UPickUpComponent>();
		if (PickUpComp)
		{
			PickUpStaticArray[(uint8)PickUpComp->GetPickUpType()].CurrentPickUpOfThisType++;
			CurrentPickUpNumber++;

			// Bind on the destruction for the count
			PickUpComp->OnPickUpDestroy.AddUniqueDynamic(this, &USpawnerComponent::OnPickupDestroyed);
		}
	}
}

void USpawnerComponent::SpawnPickup(EPickUpType PickUpType)
{
	// Checks
	if (!bCanSpawn)
	{
		UE_LOG(LogTemp, Log, TEXT("YOU CAN'T SPAWN A PICK UP RIGHT NOW"));
		return;
	}

	if (CurrentPickUpNumber >= MaxPickUpNumber)
	{
		UE_LOG(LogTemp, Log, TEXT("TOO MANY PICK UP ON THE MAP !"));
		return;
	}

	FString PickUpName = UEnum::GetValueAsString(PickUpType);

	if (!IsValid(PickUpStaticArray[(uint8)PickUpType].PickUp))
	{
		UE_LOG(LogTemp, Log, TEXT("NO GIVEN PICK UP OF THIS TYPE : %s !"), *PickUpName);
		return;
	}

	if (PickUpStaticArray[(uint8)PickUpType].CurrentPickUpOfThisType >= PickUpStaticArray[(uint8)PickUpType].MaxPickUpOfThisType)
	{
		UE_LOG(LogTemp, Log, TEXT("TO MANY PICK UP OF THIS TYPE : %s !"), *PickUpName);
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("SPAWNING PICK UP OF THIS TYPE : %s !"), *PickUpName);

	// Spawn Pick Up
	AActor* SpawnedPickUp = GetWorld()->SpawnActor(PickUpStaticArray[(uint8)PickUpType].PickUp);
	if (!SpawnedPickUp)
	{
		return;
	}

	// Bind on Destroy Pick Up
	UPickUpComponent* PickUpComp = SpawnedPickUp->FindComponentByClass<UPickUpComponent>();
	if (PickUpComp)
	{
		PickUpComp->OnPickUpDestroy.AddUniqueDynamic(this, &USpawnerComponent::OnPickupDestroyed);
	}

	// Place Pick Up
	if (GravityGunComponent.IsValid() && !GravityGunComponent->HasPickUpInHand())
	{
		GravityGunComponent->PlacePickUpInHand(SpawnedPickUp);
	}
	else if (CameraManager.IsValid())
	{
		FVector SpawnLocation = Character->GetActorLocation() + (CameraManager->GetActorForwardVector() * PickupSpawnOffsetFromPlayer);
		FQuat SpawnRotation = CameraManager->GetActorQuat();
		SpawnedPickUp->SetActorLocationAndRotation(SpawnLocation, SpawnRotation);
	}

	// Count the PickUp
	PickUpStaticArray[(uint8)PickUpType].CurrentPickUpOfThisType++;
	CurrentPickUpNumber++;

	// Prepare Timer
	bCanSpawn = false;
	ClearTimer();
	FTimerManager& TimerManager = Character->GetWorldTimerManager();
	TimerManager.SetTimer(PickUpSpawnTimerHandle, this, &USpawnerComponent::ResetTimer, DelayBetweenTwoSpawn, false);
}

void USpawnerComponent::PrintPickupInfos()
{
	UE_LOG(LogTemp, Log, TEXT("---------------------------"));
	UE_LOG(LogTemp, Log, TEXT("PICK UP ON THE MAP : %d !"), CurrentPickUpNumber);
	UE_LOG(LogTemp, Log, TEXT("NORMAL PICK UP : %d !"), PickUpStaticArray[(uint8)EPickUpType::Normal].CurrentPickUpOfThisType);
	UE_LOG(LogTemp, Log, TEXT("DESTROY AFTER PICK UP : %d !"), PickUpStaticArray[(uint8)EPickUpType::DestroyAfterPickUp].CurrentPickUpOfThisType);
	UE_LOG(LogTemp, Log, TEXT("DESTROY AFTER THROW : %d !"), PickUpStaticArray[(uint8)EPickUpType::DestroyAfterThrow].CurrentPickUpOfThisType);
	UE_LOG(LogTemp, Log, TEXT("---------------------------"));
}

void USpawnerComponent::OnPickupDestroyed(EPickUpType PickUpType)
{
	PickUpStaticArray[(uint8)PickUpType].CurrentPickUpOfThisType--;
	CurrentPickUpNumber--;
}

void USpawnerComponent::ResetTimer()
{
	bCanSpawn = true;
}


void USpawnerComponent::ClearTimer()
{
	FTimerManager& TimerManager = Character->GetWorldTimerManager();
	TimerManager.ClearTimer(PickUpSpawnTimerHandle);
}

