// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScoreComponent.generated.h"

UENUM()
enum class ETeamEnum : uint8
{
	None,
	Blue,
	Red,
	Yellow,
	Green,
	Team1,
	Team2,
	Max UMETA(Hidden),
};

USTRUCT(BlueprintType)
struct FTeamStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETeamEnum Team = ETeamEnum::None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int Score = 0;

};

UCLASS(Abstract, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5_INTRODUCTION_API UScoreComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UScoreComponent();

protected:
	virtual void BeginPlay() override;


// Exercice 3
protected:
	TArray<AActor*> GoalArray;
	
protected:
	UFUNCTION()
	void OnRecieveScore(unsigned int Score);

// End of Exercice 3

// Exercice 4
protected:
	UPROPERTY(EditAnywhere, Category = "Team")
	TArray<FTeamStruct> TeamArray;
	TStaticArray<FTeamStruct, (uint8)ETeamEnum::Max> TeamStaticArray;

protected:
	UFUNCTION()
	void RecieveGoalTeam(ETeamEnum TeamGoaled);

public:
	void DisplayScore();

// End of Exercice 4
};
