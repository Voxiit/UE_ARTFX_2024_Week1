// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/PickUpComponent.h"

UPickUpComponent::UPickUpComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UPickUpComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


void UPickUpComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

