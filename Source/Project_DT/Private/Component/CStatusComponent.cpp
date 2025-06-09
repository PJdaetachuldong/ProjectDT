// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CStatusComponent.h"
#include "GameFramework/Character.h"

UCStatusComponent::UCStatusComponent ( )
{

}

void UCStatusComponent::BeginPlay ( )
{
	Super::BeginPlay ( );

	OwnerCharacter = Cast<ACharacter> ( GetOwner ( ) );

	Health = MaxHealth;
	Mana = MaxMana;
}

float UCStatusComponent::Damage ( float InAmount )
{
	SetHP(-InAmount);
	return Health;
}

void UCStatusComponent::Heal ( float InAmount )
{
	SetHP(InAmount);
	Health += InAmount;
	Health = FMath::Clamp ( Health , 0.0f , MaxHealth );
	OnHeal.Broadcast();
}

void UCStatusComponent::UseMana ( float InAmount )
{
	SetMana(-InAmount);
}

void UCStatusComponent::RecoverMana ( float InAmount )
{
	SetMana(InAmount);
}

void UCStatusComponent::SetHP(float HP)
{
	Health += HP;
	Health = FMath::Clamp ( Health , 0.0f , MaxHealth );
	OnDelegateHP.Broadcast(Health);
}

void UCStatusComponent::SetMana(float SetMana)
{
	Mana+=SetMana;
	Mana = FMath::Clamp ( Mana , 0.0f , MaxMana );
	OnDelegateMana.Broadcast(Mana);
}
