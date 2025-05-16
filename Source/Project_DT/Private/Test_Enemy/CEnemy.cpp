// Fill out your copyright notice in the Description page of Project Settings.


#include "Test_Enemy/CEnemy.h"
#include "Global.h"
#include "Component/CMointageComponent.h"
#include "Component/CStateComponent.h"
#include "Character/CPlayerAnim.h"
#include "Component/CMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/CWeaponStuctures.h"
#include "GameFramework/Character.h"
// Sets default values
ACEnemy::ACEnemy ( )
{
	CHelpers::CreateActorComponent<UCMointageComponent> ( this , &Montages , "Montages" );
	CHelpers::CreateActorComponent<UCMovementComponent> ( this , &Movement , "Movement" );
	CHelpers::CreateActorComponent<UCStateComponent> ( this , &State , "State" );
	//CHelpers::CreateActorComponent<UCStatusComponent> ( this , &Status , "Status" );


	GetMesh ( )->SetRelativeLocation ( FVector ( 0 , 0 , -90 ) );
	GetMesh ( )->SetRelativeRotation ( FRotator ( 0 , -90 , 0 ) );

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh> ( &mesh , AssetPaths::CHARACTER_MESH );
	GetMesh ( )->SetSkeletalMesh ( mesh );

	TSubclassOf<UAnimInstance> Anim;

	CHelpers::GetClass<UAnimInstance> ( &Anim , AssetPaths::CHARACTER_ANIM );
	GetMesh ( )->SetAnimClass ( Anim );

	GetCharacterMovement ( )->RotationRate = FRotator ( 0 , 720 , 0 );
}

void ACEnemy::BeginPlay ( )
{
	Super::BeginPlay ( );

	Movement->OnWalk ( );


	Create_DynamicMeterical ( this );
	Change_Color ( this , OriginColor );

	State->OnStateTypeChanged.AddDynamic ( this , &ACEnemy::OnStateTypeChanged );
}

void ACEnemy::OnStateTypeChanged ( EStateType InPrevType , EStateType InNewType )
{
	switch ( InNewType )
	{
	case EStateType::Hitted: Hitted ( ); break;
	}
}

float ACEnemy::TakeDamage ( float DamageAmount , struct FDamageEvent const& DamageEvent , class AController* EventInstigator , AActor* DamageCauser )
{
	float damage = Super::TakeDamage ( DamageAmount , DamageEvent , EventInstigator , DamageCauser );

	Damage.Power = damage;
	Damage.Character = Cast<ACharacter> ( EventInstigator->GetPawn ( ) );
	Damage.Causer = DamageCauser;
	Damage.Event = (FActionDamageEvent*)&DamageEvent;

	State->SetHittedMode ( );

	return damage;
}

void ACEnemy::Hitted ( )
{
	Damage.Power = 0;

	if ( !!Damage.Event && !!Damage.Event->HitData ){
		FHitData* data = Damage.Event->HitData;
		data->PlayMontage ( this );
	}
	Damage.Character = nullptr;
	Damage.Causer = nullptr;
	Damage.Event = nullptr;
}
