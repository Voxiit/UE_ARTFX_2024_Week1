// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/GravityGunComponent.h"

UGravityGunComponent::UGravityGunComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UGravityGunComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UGravityGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

