// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CEquipment.h"
#include "Utilities/CHelper.h"
#include "Component/CMovementComponent.h"
#include "Component/CStateComponent.h"

void UCEquipment::BeginPlay ( class ACharacter* InOwner , TArray<FEquipmentData>& InData )
{
	OwnerCharacter = InOwner;
	Data = InData;

	Movement = CHelpers::GetComponent<UCMovementComponent> ( InOwner );
	State = CHelpers::GetComponent<UCStateComponent> ( InOwner );
}

void UCEquipment::Equip_Implementation ( )
{
	State->SetEquipMode ( );

	if ( Data[0].bCanMove == false )
		Movement->Stop ( );

	if ( !!Data[0].Montage )
		OwnerCharacter->PlayAnimMontage ( Data[0].Montage , Data[0].PlayRate );
	else {
		Begin_Equip ( );
		End_Equip ( );
	}
	if ( Data[0].bUseControlRotation )
		Movement->EnableControlRotation ( );
}



void UCEquipment::End_Equip_Implementation ( )
{
	bBeginEquip = true;
	State->SetIdleMode();
	
}

void UCEquipment::Begin_Equip_Implementation ( )
{
	bBeginEquip = false;
	bEquipped = true;
	if ( OnEquipmentBeginEquip.IsBound ( ) )
		OnEquipmentBeginEquip.Broadcast ( );
	Movement->Move ( );
	State->SetActionMode ( );

}

void UCEquipment::Unequip_Implementation ( )
{
	bEquipped = false;
	Movement->DisableControlRotation ( );

	if ( !!Data[1].Montage )
		OwnerCharacter->PlayAnimMontage ( Data[1].Montage , Data[1].PlayRate );

	if ( OnEquipmentUnequip.IsBound() )
	{
		OnEquipmentUnequip.Broadcast ( );
	}
}