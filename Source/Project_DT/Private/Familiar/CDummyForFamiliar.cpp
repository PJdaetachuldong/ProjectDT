// Fill out your copyright notice in the Description page of Project Settings.


#include "Familiar/CDummyForFamiliar.h"

// Sets default values
ACDummyForFamiliar::ACDummyForFamiliar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACDummyForFamiliar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACDummyForFamiliar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACDummyForFamiliar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

