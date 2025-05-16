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

void FDoHeavyActionData::DoHeavyAction ( class ACharacter* InOwner )
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

void FHitData::PlayHitStop ( UWorld* InWorld )
{
	CheckTrue ( FMath::IsNearlyZero ( StopTime ) );

	TArray<APawn*>pawns;
	for ( AActor* actor : InWorld->GetCurrentLevel()->Actors ) {
		APawn* pawn = Cast<ACharacter> ( actor );

		if ( !!pawn ) {
			pawn->CustomTimeDilation = 1e-3f;
			pawns.Add ( pawn );
		}
	}
	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda ( [=]( ) {
		for ( APawn* pawn : pawns )
			pawn->CustomTimeDilation = 1;
	} );
	FTimerHandle hanlde;
	InWorld->GetTimerManager ( ).SetTimer ( hanlde , timerDelegate , StopTime , false );
}
void FHitData::PlaySoundWave ( ACharacter* InOwner )
{
	CheckNull ( Sound );

	UWorld* world = InOwner->GetWorld ( );
	FVector location = InOwner->GetActorLocation ( );

	UGameplayStatics::SpawnSoundAtLocation ( world , Sound , location );
}

void FHitData::PlayEffect ( UWorld* InWorld , const FVector& InLocation )
{
	CheckNull ( Effect );

	FTransform transform;
	transform.SetLocation ( EffectLocation );
	transform.SetScale3D ( EffectScale );
	transform.AddToTranslation ( InLocation );

	CHelpers::PlayEffect ( InWorld , Effect , transform );
}

void FHitData::PlayEffect ( UWorld* InWorld , const FVector& InLocation , const FRotator& InRotation )
{
	CheckNull ( Effect );

	FTransform transform;
	transform.SetLocation ( InLocation + InRotation.RotateVector ( EffectLocation ) );
	transform.SetScale3D ( EffectScale );

	CHelpers::PlayEffect ( InWorld , Effect , transform );
}

