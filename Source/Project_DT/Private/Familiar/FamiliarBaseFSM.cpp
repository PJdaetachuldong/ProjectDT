// Fill out your copyright notice in the Description page of Project Settings.


#include "Familiar/FamiliarBaseFSM.h"

// Sets default values for this component's properties
UFamiliarBaseFSM::UFamiliarBaseFSM()
{
	PrimaryComponentTick.bCanEverTick = true;


}


// Called when the game starts
void UFamiliarBaseFSM::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UFamiliarBaseFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

