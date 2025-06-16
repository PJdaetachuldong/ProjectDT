// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_EndState.h"
#include "Global.h"
#include "Character/ICharacter.h"
#include "Component/CStateComponent.h"

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
	ACharacter* Owner = Cast<ACharacter> ( Meshcomp->GetOwner ( ) );
	CheckNull ( character );
	CheckNull ( Owner );
	UCStateComponent* State=CHelpers::GetComponent<UCStateComponent> ( Owner );
	StateType=State->GetStateType();
	
	State->OffSubActionMode();
	switch ( StateType )
	{
	case EStateType::Idle:
		break;
	case EStateType::BackStep: { character->End_BackStep (); }
		break;
	case EStateType::Equip:{ State->SetIdleMode(); }
		break;
	case EStateType::Hitted:{ State->SetIdleMode(); }
		break;
	case EStateType::Dead:{ State->SetIdleMode(); }
		break;
	case EStateType::Action:{ State->SetIdleMode(); }
		break;
	case EStateType::Cancel:{ State->SetIdleMode(); }
		break;
	case EStateType::Counter:{ State->SetIdleMode(); }
		break;
	case EStateType::Guard:{ State->SetIdleMode(); }
		break;
	case EStateType::Heal:{ State->SetIdleMode(); }
		break;
	case EStateType::Parry:{ State->SetIdleMode(); }
		break;
	case EStateType::Start:{ State->SetIdleMode(); }
		break;
	case EStateType::Max:
		break;
	default:
		break;
	}

}
