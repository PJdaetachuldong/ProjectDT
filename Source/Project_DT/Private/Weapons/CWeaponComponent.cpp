// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CWeaponComponent.h"
#include "GameFramework/Character.h"
#include "Weapons/CWeaponAsset.h"
#include "Utilities/CHelper.h"
#include "Component/CStateComponent.h"

// Sets default values for this component's properties
UCWeaponComponent::UCWeaponComponent()
{
}


void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter> ( GetOwner ( ) );
	for ( int32 i = 0; i < (int32)EWeaponType::Max; i++ ) {
		if ( !!DataAssets[i] )
			DataAssets[i]->BeginPlay ( OwnerCharacter );
	}
}

bool UCWeaponComponent::IsIdleMode ( )
{
	return CHelpers::GetComponent<UCStateComponent> ( OwnerCharacter )->IsIdleMode();
}

class ACAttachment* UCWeaponComponent::GetAttachment ( )
{
	CheckTrueResult ( IsUnarmedMode ( ) , nullptr );
	CheckFalseResult ( !!DataAssets[(int32)Type],nullptr);

	return DataAssets[(int32)Type]->GetAttachment();
}

//class UCEquipment* UCWeaponComponent::GetEquipment ( )
//{
//	CheckTrueResult ( IsUnarmedMode ( ) , nullptr );
//	CheckFalseResult ( !!DataAssets[(int32)Type] , nullptr );
//
//	return DataAssets[(int32)Type]->GetEquipment ();
//}

void UCWeaponComponent::SetUnarmedMode ( )
{
	//GetEquipment ( )->Unequip ( );
	ChangeType ( EWeaponType::Max );
}

void UCWeaponComponent::SetFistMode ( )
{

}

void UCWeaponComponent::SetKatanaMode ( )
{

}

void UCWeaponComponent::SetSwordMode ( )
{

}

void UCWeaponComponent::SetMode ( EWeaponType InType )
{
	if ( Type == InType )
	{
		SetUnarmedMode ( );

		return;
	}
}

void UCWeaponComponent::ChangeType ( EWeaponType InType )
{
	EWeaponType prevType = Type;
	Type = InType;

	if ( OnWeaponTypeChange.IsBound ( ) )
		OnWeaponTypeChange.Broadcast ( prevType , InType );
}