// Fill out your copyright notice in the Description page of Project Settings.


#include "Familiar/CFamiliarWolf.h"
#include "Familiar/CWolfFSM.h"

ACFamiliarWolf::ACFamiliarWolf ( )
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACFamiliarWolf::BeginPlay ( )
{
	Super::BeginPlay ( );

}

void ACFamiliarWolf::Tick ( float DeltaTime )
{
	Super::Tick ( DeltaTime );

}

void ACFamiliarWolf::Landed ( const FHitResult& Hit )
{
	WolfFSM->mUpState = EUpperState::Idle;
}
