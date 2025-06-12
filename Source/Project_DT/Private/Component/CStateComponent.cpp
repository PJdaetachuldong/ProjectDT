// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CStateComponent.h"

// Sets default values for this component's properties
UCStateComponent::UCStateComponent()
{

}


// Called when the game starts
void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();


}

void UCStateComponent::TickComponent ( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction )
{

}
void UCStateComponent::SetIdleMode ( )
{
	ChangeType ( EStateType::Idle );
}

void UCStateComponent::SetBackStepMode ( )
{
	ChangeType ( EStateType::BackStep );
}

void UCStateComponent::SetEquipMode ( )
{
	ChangeType ( EStateType::Equip );

}

void UCStateComponent::SetHittedMode ( )
{
	ChangeType ( EStateType::Hitted );
}

void UCStateComponent::SetDeadMode ( )
{
	ChangeType ( EStateType::Dead );
}

void UCStateComponent::SetActionMode ( )
{
	ChangeType ( EStateType::Action );

}

void UCStateComponent::SetCancelMode()
{
	ChangeType(EStateType::Cancel);
}

void UCStateComponent::SetGuardMode()
{
	ChangeType(EStateType::Guard);
	
}

void UCStateComponent::SetHealMode()
{
	ChangeType(EStateType::Heal);
}

void UCStateComponent::SetParryMode()
{
	ChangeType(EStateType::Parry);
	
}

void UCStateComponent::SetCounterMode()
{
	ChangeType(EStateType::Counter);
	
}

void UCStateComponent::OnSubActionMode ( )
{
	bInSubActionMode = true;
}

void UCStateComponent::OffSubActionMode ( )
{
	bInSubActionMode = false;

}

void UCStateComponent::ChangeType ( EStateType InType )
{
	EStateType prevType = Type;
	Type = InType;

	if ( OnStateTypeChanged.IsBound ( ) )
		OnStateTypeChanged.Broadcast ( prevType , Type );
}

