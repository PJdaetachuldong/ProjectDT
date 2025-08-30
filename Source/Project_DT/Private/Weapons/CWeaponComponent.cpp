// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CWeaponComponent.h"
#include "GameFramework/Character.h"
#include "Weapons/CWeaponAsset.h"
#include "Utilities/CHelper.h"
#include "Component/CStateComponent.h"
#include "Weapons/CEquipment.h"
#include "Global.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CSubAction.h"
#include "Weapons/SubActions/CSubAction_Skill.h"
#include "Widget/CPlayerWidget.h"

// Sets default values for this component's properties
UCWeaponComponent::UCWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter> ( GetOwner ( ) );
	for ( int32 i = 0; i < (int32)EWeaponType::Max; i++ ) {
		if ( !!DataAssets[i] )
			DataAssets[i]->BeginPlay ( OwnerCharacter );
	}
	State = CHelpers::GetComponent<UCStateComponent> ( OwnerCharacter );
}

void UCWeaponComponent::TickComponent ( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent ( DeltaTime , TickType , ThisTickFunction );

	if ( !!GetSubAction ( ) )
		GetSubAction ( )->Tick ( DeltaTime );
	if ( !!GetSubAction_Skill ( ) )
		GetSubAction_Skill ( )->Tick ( DeltaTime );

	if (State->GetStateType()==EStateType::Idle){
		CombatStateTime = +DeltaTime;
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

class UCEquipment* UCWeaponComponent::GetEquipment ( )
{
	CheckTrueResult ( IsUnarmedMode ( ) , nullptr );
	CheckFalseResult ( !!DataAssets[(int32)Type] , nullptr );

	return DataAssets[(int32)Type]->GetEquipment ( );
}

class UCDoAction* UCWeaponComponent::GetDoAction ( )
{
	CheckTrueResult ( IsUnarmedMode ( ) , nullptr );
	CheckFalseResult ( !!DataAssets[(int32)Type] , nullptr );
	return DataAssets[(int32)Type]->GetDoAction ();
}

class UCSubAction* UCWeaponComponent::GetSubAction ( )
{
	CheckTrueResult ( IsUnarmedMode ( ) , nullptr );
	CheckFalseResult ( !!DataAssets[(int32)Type] , nullptr );



	return DataAssets[(int32)Type]->GetSubAction ( );

}

class UCSubAction_Skill* UCWeaponComponent::GetSubAction_Skill ( )
{
	CheckTrueResult ( IsUnarmedMode ( ) , nullptr );
	CheckFalseResult ( !!DataAssets[(int32)Type] , nullptr );


	return DataAssets[(int32)Type]->GetSubAction_Skill ( );
}

void UCWeaponComponent::SetUnarmedMode ( )
{
	CheckFalse ( IsIdleMode ( ) );

	GetEquipment ( )->Unequip ( );

	ChangeType ( EWeaponType::Max );


}

void UCWeaponComponent::SetFistMode ( )
{
	CheckFalse ( IsIdleMode ( ) );
	SetMode ( EWeaponType::Fist );

}

void UCWeaponComponent::SetKatanaMode ( )
{
		CheckFalse ( IsIdleMode ( ) );
		SetMode ( EWeaponType::Katana );
}

void UCWeaponComponent::SetSwordMode ( )
{
		CheckFalse ( IsIdleMode ( ) );
		SetMode ( EWeaponType::Sword );
}

void UCWeaponComponent::SetGreatSwordMode ( )
{

		CheckFalse ( IsIdleMode ( ) );
		SetMode ( EWeaponType::GreatSword );
}

void UCWeaponComponent::DoAction ( )
{
	if ( !!GetDoAction ( ) ){
		GetDoAction ()->DoAction ( );
		GetDoAction ()->NormalAttack ( );
	}

}

void UCWeaponComponent::DoHeavyAction ( )
{
	if ( !!GetDoAction ( ) ) {
		GetDoAction ( )->DoHeavyAction ( );
		GetDoAction ( )->HeavyAttack ( );
	}

}

void UCWeaponComponent::SubAction_Pressed()
{
	//if ( !bCanParry ) return;
	// State = CHelpers::GetComponent<UCStateComponent> ( OwnerCharacter );
	//
	// //CheckFalse ( State->IsIdleMode ( ) );
	//
	// if (!!GetSubAction())
	// 	GetSubAction()->Pressed();
	//
	// UCParryComponent* parry = CHelpers::GetComponent<UCParryComponent>(OwnerCharacter);
	// CheckNull(parry);

	State->SetGuardMode();



}

void UCWeaponComponent::SubAction_Released ( )
{
	State->SetIdleMode();

	// if ( !!GetSubAction ( ) )
	// 	GetSubAction ( )->Released ( );
	//
	// UCParryComponent* parry = CHelpers::GetComponent<UCParryComponent> ( OwnerCharacter );
	// CheckNull ( parry );
	//parry->OffParryCollision ( );

}

void UCWeaponComponent::SubAction_Skill_Pressed ( )
{
	if ( !!GetSubAction_Skill ( ) ){
		GetSubAction_Skill ( )->Pressed ( );
	}
}

void UCWeaponComponent::SubAction_Skill_Released ( )
{
	if ( !!GetSubAction_Skill ( ) )
		GetSubAction_Skill ( )->Released ( );
}

void UCWeaponComponent::OnParry ( EParryState ParryState )
{
	if ( !!GetDoAction())
		GetDoAction ( )->DoActionParry ( ParryState );
}

void UCWeaponComponent::SetMode ( EWeaponType InType )
{
	if ( Type == InType )
	{
		SetUnarmedMode ( );

		return;
	}
	else if ( IsUnarmedMode ( ) == false )
		GetEquipment ( )->Unequip ( );
	if ( !!DataAssets[( int32 )InType]) {
		DataAssets[(int32)InType]->GetEquipment ( )->Equip ( );
		ChangeType ( InType );
	}
}

void UCWeaponComponent::ChangeType ( EWeaponType InType )
{
	EWeaponType prevType = Type;
	Type = InType;


	if ( OnWeaponTypeChange.IsBound ( ) )
		OnWeaponTypeChange.Broadcast ( prevType , InType );
}