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
	Health += ( InAmount * -1.0f );
	Health = FMath::Clamp ( Health , 0.0f , MaxHealth );
	return Health;
}

void UCStatusComponent::Heal ( float InAmount )
{
	Health += InAmount;
	Health = FMath::Clamp ( Health , 0.0f , MaxHealth );
	OnHeal.Broadcast();
}

void UCStatusComponent::UseMana ( float InAmount )
{
	Mana -= InAmount;
	Mana = FMath::Clamp ( Mana , 0.0f , MaxMana );
}

void UCStatusComponent::RecoverMana ( float InAmount )
{
	Mana += InAmount;
	Mana = FMath::Clamp ( Mana , 0.0f , MaxMana );
}