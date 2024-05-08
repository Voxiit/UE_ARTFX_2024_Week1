// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/PickUpComponent.h"
#include "UObject/ObjectSaveContext.h"

// Noted Exercice
static FName PickUpTag = "PICKUP";

UPickUpComponent::UPickUpComponent()
{
	PrimaryComponentTick.bCanEverTick = false;	
}

void UPickUpComponent::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

	// Add tag to a Pick Up to find them more easily
	AActor* Owner = GetOwner();
	if (!Owner->Tags.Contains(PickUpTag))
	{
		Owner->Tags.Add(PickUpTag);
	}
}

void UPickUpComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);

#if WITH_EDITOR
	if (GetWorld() && GetWorld()->IsPlayInEditor())
	{
		AActor* Owner = GetOwner();
		if (Owner->Tags.Contains(PickUpTag))
		{
			Owner->Tags.Remove(PickUpTag);
		}
	}
#endif
}

void UPickUpComponent::StartPickUpDetonationTimer()
{
	float DestructionTime = PickUpStruct.DestructionTimer;

	// Prepare Timer
	FTimerManager& TimerManager = GetOwner()->GetWorldTimerManager();
	TimerManager.ClearTimer(ProjectileDestructionTimerHandle);
	TimerManager.SetTimer(ProjectileDestructionTimerHandle, this, &UPickUpComponent::DestroyPickUp, DestructionTime, false);

}

void UPickUpComponent::DestroyPickUp()
{
	ClearTimer();

	OnPickUpDestroy.Broadcast(PickUpStruct.PickUpType);

	GetOwner()->Destroy();
}

EPickUpType UPickUpComponent::GetPickUpType()
{
	return PickUpStruct.PickUpType;
}

void UPickUpComponent::ClearTimer()
{
	// Clear Timer
	FTimerManager& TimerManager = GetOwner()->GetWorldTimerManager();
	TimerManager.ClearTimer(ProjectileDestructionTimerHandle);
}
