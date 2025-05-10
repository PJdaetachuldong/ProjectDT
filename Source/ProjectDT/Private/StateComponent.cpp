// Fill out your copyright notice in the Description page of Project Settings.


#include "StateComponent.h"

// Sets default values for this component's properties
UStateComponent::UStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStateComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);

}

void UStateComponent::SetBackStepMode()
{
	ChangeType(EStateType::BackStep);

}

void UStateComponent::SetEquipMode()
{
	ChangeType(EStateType::Equip);

}

void UStateComponent::SetHittedMode()
{
	ChangeType(EStateType::Hitted);

}

void UStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);

}

void UStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);

}

void UStateComponent::OnSubActionMode()
{
	bInSubActionMode = true;

}

void UStateComponent::OffSubActionMode()
{
	bInSubActionMode = false;

}

void UStateComponent::ChangeType(EStateType InType)
{
	EStateType prevType = Type;
	Type = InType;

	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(prevType, Type);
}

