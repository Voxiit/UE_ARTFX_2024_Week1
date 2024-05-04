// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/GravityGunComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MainCharacter.h"
#include "Gameplay/PickUpComponent.h"

UGravityGunComponent::UGravityGunComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UGravityGunComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AMainCharacter>(GetOwner());
	CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	GravityGunCollisionChannel = UEngineTypes::ConvertToCollisionChannel(SimpleCollisionTraceChannel);

	// Exercice 1
	InitialRaycastSize = RaycastSize;
}


void UGravityGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UGravityGunComponent::OnTakeObjectInputPressed()
{
	if (!CameraManager.IsValid())
	{
		return;
	}

	// Raycast Location
	FVector RaycastStart = CameraManager->GetCameraLocation();
	FVector RaycastEnd = RaycastStart + CameraManager->GetActorForwardVector() * RaycastSize;

	// Params
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character.Get());
	FHitResult RaycastHit;

	bool bHit = GetWorld()->LineTraceSingleByChannel(RaycastHit, RaycastStart, RaycastEnd, GravityGunCollisionChannel,Params);
#if !UE_BUILD_SHIPPING
	if (bDrawDebugRaycast)
		DrawDebugLine(GetWorld(), RaycastStart, RaycastEnd, FColor::Red, false, TimerDebugRaycast, 0, 3.5f);
#endif

	if (!bHit)
		UE_LOG(LogTemp, Log, TEXT("WE HIT NOTHING"));

	// Get Pick Up
	CurrentPickUp = RaycastHit.GetActor();
	if (!CurrentPickUp)
	{
		return;
	}

	// Check for Pick Up comp
	CurrentPickUpComponent = CurrentPickUp->GetComponentByClass<UPickUpComponent>();
	if (!CurrentPickUpComponent)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("WE HIT %s"), *CurrentPickUp->GetName());
}

void UGravityGunComponent::OnThrowObjectInputPressed()
{
	UE_LOG(LogTemp, Log, TEXT("OnThrowObjectInputPressed"));
}

void UGravityGunComponent::OnThrowObjectInputReleased()
{
	UE_LOG(LogTemp, Log, TEXT("OnThrowObjectInputReleased"));
}

void UGravityGunComponent::OnIncreaseRaycastSize()
{
	RaycastSize = FMath::Clamp(RaycastSize + RaycastVariationByInput, InitialRaycastSize, RaycastMaxSize);
	UE_LOG(LogTemp, Log, TEXT("NEW SIZE %f"), RaycastSize);
}

void UGravityGunComponent::OnDecreaseRaycastSize()
{
	RaycastSize = FMath::Clamp(RaycastSize - RaycastVariationByInput, InitialRaycastSize, RaycastMaxSize);
	UE_LOG(LogTemp, Log, TEXT("NEW SIZE %f"), RaycastSize);
}

