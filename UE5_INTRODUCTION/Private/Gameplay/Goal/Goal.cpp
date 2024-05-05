// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Goal/Goal.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Gameplay/PickUpComponent.h"

// Sets default values
AGoal::AGoal(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	
	// Create collision Box
	CollisionBox = ObjectInitializer.CreateOptionalDefaultSubobject<UBoxComponent>(this, TEXT("CollisionBox"));
	if (CollisionBox)
	{
		SetRootComponent(CollisionBox);
	}
}

void AGoal::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AGoal::OnReceiveBeginOverlap);
}

void AGoal::OnReceiveBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if we've got a Pick Up
	UPickUpComponent* EnteringPickUp = OtherActor->FindComponentByClass<UPickUpComponent>();
	if (!EnteringPickUp)
		return;

	// Update our score
	Score++;
	OnSendGoal.Broadcast(Score);
	OnSendTeamGoaled.Broadcast(GoalTeam);
	UE_LOG(LogTemp, Log, TEXT("%s HAS A SCORE OF %d"), *GetName(), Score);
}

unsigned int AGoal::CountPickUpInGoal()
{
	// Prepare raycast location
	FVector GoalLocation = GetActorLocation();
	FVector ScaledBoxExtent = CollisionBox->GetScaledBoxExtent();

	// Launch Our Raycast
	const TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> OutHits;
	UKismetSystemLibrary::BoxTraceMulti(GetWorld(), GoalLocation, GoalLocation, ScaledBoxExtent, GetActorRotation(), 
	GoalCollisionTraceChannel, false, ActorsToIgnore, EDrawDebugTrace::None, OutHits, true);

	int NumberOfPickUpInGoal = OutHits.Num();
	return NumberOfPickUpInGoal;
}

