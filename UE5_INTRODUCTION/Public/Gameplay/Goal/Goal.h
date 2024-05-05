// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gameplay/Score/ScoreComponent.h"
#include "Goal.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSendGoalDelegate, unsigned int, Score);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSendTeamGoaledDelegate, ETeamEnum, TeamGoaled);

UCLASS()
class UE5_INTRODUCTION_API AGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	AGoal(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

// Collision Box
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UBoxComponent* CollisionBox = nullptr;
	unsigned int Score = 0;

public:
	UFUNCTION()
	void OnReceiveBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

// End of Collision Box


// Exercice 3
public:
	FSendGoalDelegate OnSendGoal;

// End of Exercice 3


// Score
protected:
	UPROPERTY(EditAnywhere, Category = "Goal|Collision")
	TEnumAsByte<ETraceTypeQuery> GoalCollisionTraceChannel;

public:
	unsigned int CountPickUpInGoal();

// End of Score


// Exercice 4
protected:
	UPROPERTY(EditAnywhere, Category = "Team")
	ETeamEnum GoalTeam = ETeamEnum::None;

public:
	FSendTeamGoaledDelegate OnSendTeamGoaled;

// End of Exercice 4
};
