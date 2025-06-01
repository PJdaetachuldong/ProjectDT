// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CPlayerAnim.h"
#include "GameFramework/Character.h"
#include "Global.h"
#include "Character/CPlayer.h"

void UCPlayerAnim::NativeBeginPlay ( )
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACPlayer> ( TryGetPawnOwner ( ) );
	CheckNull ( OwnerCharacter );

	Weapon = CHelpers::GetComponent<UCWeaponComponent> ( OwnerCharacter );
	State = CHelpers::GetComponent<UCStateComponent> ( OwnerCharacter );
	if ( !!Weapon )
		Weapon->OnWeaponTypeChange.AddDynamic ( this , &UCPlayerAnim::OnWeaponTypeChanged );
}

void UCPlayerAnim::NativeUpdateAnimation ( float DeltaSeconds )
{
	Super::NativeUpdateAnimation ( DeltaSeconds );

	CheckNull ( OwnerCharacter );
	isMontagePlaying = OwnerCharacter->IsPlayingRootMotion();
	Speed = OwnerCharacter->GetVelocity ( ).Size2D ( );
	auto Owner = Cast<ACPlayer> ( TryGetPawnOwner ( ) );
	Movement = Owner->GetCharacterMovement();
	Trajectory = Owner->Trajectory;
	WeaponType = Weapon->GetWeaponType ( );
	StateType = State->GetStateType ( );
}

void UCPlayerAnim::OnWeaponTypeChanged ( EWeaponType InPrevType , EWeaponType InNewType )
{
	WeaponType = InNewType;
}
