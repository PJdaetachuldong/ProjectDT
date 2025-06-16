// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CMointageComponent.h"
#include "GameFramework/Character.h"
#include "Global.h"
#include "LHW_GameModeBase.h"
#include "Component/CProductionComponent.h"
#include "Component/CStateComponent.h"
#include "Component/CStatusComponent.h"
#include "Component/CTargetingComponent.h"
#include "Dummy/CTargetActor.h"
#include "GameFramework/GameModeBase.h"
#include "Widget/CGameOverWidget.h"

// Sets default values for this component's properties
UCMointageComponent::UCMointageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	CHelpers::GetClass(&GameOverClass,AssetPaths::GameOverWidget);
	

	// ...
}
void UCMointageComponent::BeginPlay ( )
{
	Super::BeginPlay ( );

	if ( DataTable == nullptr )
	{
		GLog->Log ( ELogVerbosity::Error , "DataTable is not selected" );

		return;
	}
	OwnerCharacter = Cast<ACharacter> ( GetOwner ( ) );
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);
	Target = CHelpers::GetComponent<UCTargetingComponent>(OwnerCharacter);
	Production = CHelpers::GetComponent<UCProductionComponent>(OwnerCharacter);
	GameOver=CreateWidget<UCGameOverWidget>(GetWorld(),GameOverClass);


	TArray<FMontagesData*> datas;
	DataTable->GetAllRows<FMontagesData> ( "" , datas );

	for ( int32 i = 0; i < (int32)EActState::Max; i++ )
	{
		for ( FMontagesData* data : datas )
		{
			if ( (EActState)i == data->Type )
			{
				Datas[i] = data;

				continue;
			}
		}
	}
}


void UCMointageComponent::TickComponent ( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent ( DeltaTime , TickType , ThisTickFunction );
}

void UCMointageComponent::PlayBackStepMode (EActState InType)
{
	PlayAnimMontage (InType);
}

void UCMointageComponent::PlayDeadMode ( )
{
	PlayAnimMontage ( EActState::Dead );

	OwnerCharacter->DisableInput(nullptr);
	isDead=true;
	DieDelegate.Broadcast();
	GameOver->AddToViewport();
	GameOver->PlayGameOverAnimation();
	
	
}

void UCMointageComponent::PlayEquipMode ( )
{
}

void UCMointageComponent::PlayHealingMode ( )
{
	PlayAnimMontage ( EActState::Healing );

}

void UCMointageComponent::PlayPerfectDodgeL()
{
	PlayAnimMontage(EActState::PDodgeL);

}

void UCMointageComponent::PlayPerfectDodgeR()
{
	PlayAnimMontage(EActState::PDodgeR);
}

void UCMointageComponent::PlayPerfectDodge()
{
	PlayAnimMontage(EActState::PDodge);
}

void UCMointageComponent::PlayIntro()
{
	PlayAnimMontage(EActState::Intro);
}

void UCMointageComponent::Dead()
{
	PlayAnimMontage(EActState::Dead);
}

void UCMointageComponent::Respawn()
{
	if(OwnerCharacter&&State&& Status){
		Target->ResetLockOn();
	OwnerCharacter->SetActorLocation(RespawnPosition);
	Status->Heal(100);
	Status->UseMana(100);
	isDead=false;
		GetWorld()->GetTimerManager().SetTimer(handler,[this]()
			{
				OwnerCharacter->EnableInput(nullptr);
				PlayAnimMontage(EActState::Getup);
				Production->SetCameraLookAtNearestTarget();
			},2.0f,false,2.0f);
	}
}

void UCMointageComponent::PlayAnimMontage ( EActState InType )
{
	CheckNull ( OwnerCharacter );
	GLog->Log(ELogVerbosity::Error, "montages data");

	FMontagesData* data = Datas[(int32)InType];
	if ( data == nullptr || data->Montage == nullptr )
	{
		GLog->Log ( ELogVerbosity::Error , "None montages data" );
		return;
	}
	OwnerCharacter->PlayAnimMontage ( data->Montage , data->PlayRate );
}
