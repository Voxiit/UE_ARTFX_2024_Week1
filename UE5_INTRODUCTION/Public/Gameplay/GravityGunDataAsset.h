// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GravityGunDataAsset.generated.h"

UCLASS()
class UE5_INTRODUCTION_API UGravityGunDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Gravity Gun")
	float MinThrowForce = 500.f;
	UPROPERTY(EditAnywhere, Category = "Gravity Gun")
	FVector ThrowAngularForce = FVector(500.f, 500.f, 500.f);
	UPROPERTY(EditAnywhere, Category = "Gravity Gun")
	float MaxThrowForce = 5000.f;
	UPROPERTY(EditAnywhere, Category = "Gravity Gun")
	float TimeToReachMaxThrowForce = 3.f;
};
