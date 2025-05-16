// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_EndState.h"
#include "Utilities/CHelper.h"
#include "Character/ICharacter.h"

FString UCAnimNotify_EndState::GetNotifyName_Implementation ( ) const
{
	return "EndState";
}

void UCAnimNotify_EndState::Notify ( USkeletalMeshComponent* Meshcomp , UAnimSequenceBase* Animation )
{
	Super::Notify ( Meshcomp , Animation );
	CheckNull ( Meshcomp );
	CheckNull ( Meshcomp->GetOwner ( ) );

	IICharacter* character = Cast<IICharacter> ( Meshcomp->GetOwner ( ) );
	CheckNull ( character );

	switch ( StateType )
	{
	case EStateType::Idle:
		break;
	case EStateType::BackStep: { character->End_BackStep (); }
		break;
	case EStateType::Equip:
		break;
	case EStateType::Hitted:
		break;
	case EStateType::Dead:
		break;
	case EStateType::Action:
		break;
	case EStateType::Max:
		break;
	default:
		break;
	}

}
