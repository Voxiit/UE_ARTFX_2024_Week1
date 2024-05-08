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

	UpdatePickUpLocation();
	UpdateThrowforceTimer(DeltaTime);
}

void UGravityGunComponent::OnTakeObjectInputPressed()
{
	if (!CameraManager.IsValid())
	{
		return;
	}

	// Check if pick up is present
	if (CurrentPickUp)
	{
		ReleasePickUp();
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
	PlacePickUpInHand(RaycastHit.GetActor());
}

void UGravityGunComponent::OnThrowObjectInputPressed()
{
	// Check for static mesh
	if (!CurrentPickUpMesh)
	{
		return;
	}

	bUpdateThrowForceTimer = true;
	CurrentTimetoReachMaxThrowForce = 0.f;
}

void UGravityGunComponent::OnThrowObjectInputReleased()
{
	if (!CurrentPickUp)
	{
		return;
	}

	ReleasePickUp(true);

	// Stop Updating
	CurrentTimetoReachMaxThrowForce = 0.f;
	bUpdateThrowForceTimer = false;
}

void UGravityGunComponent::UpdatePickUpLocation()
{
	if (!CurrentPickUp)
	{
		return;
	}

	FVector NewPickUpLocation = CameraManager->GetCameraLocation() + CameraManager->GetActorForwardVector() * PickUpDistanceFromPlayer;
	CurrentPickUp->SetActorLocationAndRotation(NewPickUpLocation, CameraManager->GetActorQuat());
}

void UGravityGunComponent::ReleasePickUp(bool bThrow)
{
	// Unbind Event
	if (CurrentPickUpComponent->GetPickUpType() == EPickUpType::DestroyAfterPickUp)
	{
		CurrentPickUpComponent->OnPickUpDestroy.RemoveDynamic(this, &UGravityGunComponent::OnHoldPickUpDestroyed);
	}


	// Set back physic
	CurrentPickUpMesh->SetSimulatePhysics(true);

	// Set back coll profile
	CurrentPickUpMesh->SetCollisionProfileName(PreviousCollisionProfile);

	// If Throw pick up
	if (bThrow)
	{
		// Compute force timer
		float ThrowForceAlpha = FMath::Clamp(CurrentTimetoReachMaxThrowForce / TimetoReachMaxThrowForce, 0.f, 1.f);
		float ThrowForce = FMath::Lerp(PickUpThrowForce, PickUpMaxThrowForce, ThrowForceAlpha);
		UE_LOG(LogTemp, Log, TEXT("THROW FORCE ALPHA %f - THROW FORCE %f"), ThrowForceAlpha, ThrowForce);

		FVector Impulse = CameraManager->GetActorForwardVector() * ThrowForce;
		CurrentPickUpMesh->AddImpulse(Impulse);
		FVector AngularImpulse = FVector(FMath::RandRange(.0, PickUpAngularForce.X), FMath::RandRange(.0, PickUpAngularForce.Y), FMath::RandRange(.0, PickUpAngularForce.Z));
		CurrentPickUpMesh->AddAngularImpulseInDegrees(AngularImpulse);
	}

	// Check if destruction timer required
	if (CurrentPickUpComponent->GetPickUpType() == EPickUpType::DestroyAfterThrow)
	{
		CurrentPickUpComponent->StartPickUpDetonationTimer();
	}

	// Clean refs
	CurrentPickUp = nullptr;
	CurrentPickUpComponent = nullptr;
	CurrentPickUpMesh = nullptr;
}

void UGravityGunComponent::OnIncreaseRaycastSize()
{
	RaycastSize = FMath::Clamp(RaycastSize + RaycastVariationByInput, InitialRaycastSize, RaycastMaxSize);
}

void UGravityGunComponent::OnDecreaseRaycastSize()
{
	RaycastSize = FMath::Clamp(RaycastSize - RaycastVariationByInput, InitialRaycastSize, RaycastMaxSize);
}

void UGravityGunComponent::UpdateThrowforceTimer(float DeltaTime)
{
	if (!bUpdateThrowForceTimer)
	{
		return;
	}

	CurrentTimetoReachMaxThrowForce += DeltaTime;
}

void UGravityGunComponent::OnHoldPickUpDestroyed(EPickUpType PickUpType)
{
	CurrentPickUpComponent->OnPickUpDestroy.RemoveDynamic(this, &UGravityGunComponent::OnHoldPickUpDestroyed);

	ReleasePickUp();
}

bool UGravityGunComponent::HasPickUpInHand()
{
	return IsValid(CurrentPickUp);
}

void UGravityGunComponent::PlacePickUpInHand(AActor* PickUp)
{
	CurrentPickUp = PickUp;
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

	// Get mesh
	CurrentPickUpMesh = CurrentPickUp->GetComponentByClass<UStaticMeshComponent>();
	if (!CurrentPickUpMesh)
	{
		return;
	}

	// Update collision profile
	PreviousCollisionProfile = CurrentPickUpMesh->GetCollisionProfileName();
	CurrentPickUpMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	// Disable physic
	CurrentPickUpMesh->SetSimulatePhysics(false);

	// Reset timer if required
	if (CurrentPickUpComponent->GetPickUpType() != EPickUpType::Normal)
	{
		CurrentPickUpComponent->ClearTimer();
	}

	// Check if destruction timer required
	if (CurrentPickUpComponent->GetPickUpType() == EPickUpType::DestroyAfterPickUp)
	{
		CurrentPickUpComponent->StartPickUpDetonationTimer();
		CurrentPickUpComponent->OnPickUpDestroy.AddUniqueDynamic(this, &UGravityGunComponent::OnHoldPickUpDestroyed);
	}
}

void UGravityGunComponent::OnDestroyPickUpInHand()
{
	if (!CurrentPickUpComponent)
	{
		return;
	}

	UPickUpComponent* PickUpToDestroy = CurrentPickUpComponent;
	ReleasePickUp();
	PickUpToDestroy->DestroyPickUp();
}

