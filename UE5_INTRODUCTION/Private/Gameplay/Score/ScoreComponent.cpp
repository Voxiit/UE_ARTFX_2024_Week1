// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Score/ScoreComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/Goal/Goal.h"

UScoreComponent::UScoreComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UScoreComponent::BeginPlay()
{
	Super::BeginPlay();

	// Exercice 3
	// Bind function to each goal
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGoal::StaticClass(), GoalArray);
	for (AActor* Goal : GoalArray)
	{
		AGoal* GoalCasted = Cast<AGoal>(Goal);
		if (GoalCasted)
		{
			GoalCasted->OnSendGoal.AddUniqueDynamic(this, &UScoreComponent::OnRecieveScore);

			// Exercice 4
			GoalCasted->OnSendTeamGoaled.AddUniqueDynamic(this, &UScoreComponent::RecieveGoalTeam);
		}
	}

	// Exercice 4
	// Make team static array
	for (const FTeamStruct& TeamStruct : TeamArray)
	{
		TeamStaticArray[(uint8)TeamStruct.Team] = TeamStruct;
	}
}

void UScoreComponent::OnRecieveScore(unsigned int Score)
{
	UE_LOG(LogTemp, Log, TEXT("SCORE RECIEVED IS % d"), Score);
}

void UScoreComponent::RecieveGoalTeam(ETeamEnum TeamGoaled)
{
	if (TeamGoaled != ETeamEnum::None)
	{
		TeamStaticArray[(uint8)TeamGoaled].Score++;
	}
}

void UScoreComponent::DisplayScore()
{
	for (const FTeamStruct& TeamStruct : TeamStaticArray)
	{
		if (TeamStruct.Team != ETeamEnum::None)
		{
			FString EnumName;
			UEnum::GetValueAsString(TeamStruct.Team, EnumName);
			UE_LOG(LogTemp, Log, TEXT("TEAM %s HAS TAKEN %d GOAL"), *EnumName, TeamStruct.Score);
		}
	}
}
