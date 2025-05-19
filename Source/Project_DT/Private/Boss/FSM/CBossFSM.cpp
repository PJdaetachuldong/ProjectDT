// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/FSM/CBossFSM.h"

// Sets default values for this component's properties
UCBossFSM::UCBossFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCBossFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UCBossFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch ( State )
	{
	case EBossState::IDLE: { IDLEState ( ); } break;
		case EBossState::CHASE: { CHASEState ( ); } break;
		case EBossState::ATTACK: { ATTACKState ( ); } break;
		case EBossState::DAMAGE: { DAMAGEState ( ); } break;
		case EBossState::DIE: { DIEState ( ); }	break;
	}

	if ( State == EBossState::ATTACK )
	{
		switch ( AttackState )
		{
			  case EBossATTACKState::NONE: { NONEState(); } break;
			  case EBossATTACKState::RANGEDATTACK: {RANGEDATTACKState();} break;
			  case EBossATTACKState::DASHATTACK: {DASHATTACKState();} break;
			  case EBossATTACKState::COMBOATTACK: {COMBOATTACKState();} break;
			  case EBossATTACKState::COUNTERATTACK: {COUNTERATTACKState();} break;
			  case EBossATTACKState::SPATTACK: {SPATTACKState();} break;
		}
	}
}

void UCBossFSM::IDLEState()
{

}

void UCBossFSM::CHASEState()
{

}

void UCBossFSM::ATTACKState()
{

}

void UCBossFSM::DAMAGEState()
{

}

void UCBossFSM::DIEState()
{

}

void UCBossFSM::NONEState()
{

}

void UCBossFSM::RANGEDATTACKState()
{

}

void UCBossFSM::DASHATTACKState()
{

}

void UCBossFSM::COMBOATTACKState()
{

}

void UCBossFSM::COUNTERATTACKState()
{

}

void UCBossFSM::SPATTACKState()
{

}

