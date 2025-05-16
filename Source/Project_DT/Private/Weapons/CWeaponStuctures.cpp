// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CWeaponStuctures.h"
#include "Global.h"
#include "Component/CMovementComponent.h"

void FDoActionData::DoAction ( class ACharacter* InOwner )
{
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent> ( InOwner );

	if ( !!movement ) {
		if ( bFixedCamera )
			movement->EnableFixedCamera ( );

		if ( bCanMove == false )
			movement->Stop ( );
	}
	if ( !!Montage )
		InOwner->PlayAnimMontage ( Montage , PlayRate );
}
/// ////////////////////////////////////////////////


void FHitData::SendDamage ( class ACharacter* InAttacker , AActor* InAttackCauser , class ACharacter* InOther )
{
	FActionDamageEvent e;
	e.HitData = this;
	InOther->TakeDamage ( 20 , e, InAttacker->GetController ( ) , InAttackCauser );
}

void FHitData::PlayMontage ( class ACharacter* InOwner )
{
	if ( !!Montage )
		InOwner->PlayAnimMontage ( Montage , PlayRate );
}
