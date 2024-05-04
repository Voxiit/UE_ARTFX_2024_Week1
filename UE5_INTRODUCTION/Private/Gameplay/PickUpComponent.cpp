// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/PickUpComponent.h"

UPickUpComponent::UPickUpComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

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

	OnPickUpDestroy.Broadcast();

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
