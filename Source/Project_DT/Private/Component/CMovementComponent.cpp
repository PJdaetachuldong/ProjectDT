// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/CWeaponComponent.h"
#include "Global.h"

// Sets default values for this component's properties
UCMovementComponent::UCMovementComponent()
{

}


// Called when the game starts
void UCMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter> ( GetOwner ( ) );
	Weapon = CHelpers::GetComponent<UCWeaponComponent> ( OwnerCharacter );

	// ...

}

void UCMovementComponent::SetSpeed ( ESpeedType InType )
{
	OwnerCharacter->GetCharacterMovement ( )->MaxWalkSpeed = Speed[(int32)InType];
}

void UCMovementComponent::OnSprint ( )
{
	CheckFalse ( Weapon->GetWeaponType ( ) == EWeaponType::Max );
	SetSpeed ( ESpeedType::Sprint );

}

void UCMovementComponent::OnRun ( )
{
	SetSpeed ( ESpeedType::Run );

}

void UCMovementComponent::OnWalk ( )
{
	SetSpeed ( ESpeedType::Walk );

}

void UCMovementComponent::EnableControlRotation ( )
{
	OwnerCharacter->bUseControllerRotationYaw = true;

	OwnerCharacter->GetCharacterMovement ( )->bOrientRotationToMovement = false;
	OwnerCharacter->GetCharacterMovement ( )->bUseControllerDesiredRotation = true;
}

void UCMovementComponent::DisableControlRotation ( )
{
	OwnerCharacter->bUseControllerRotationYaw = false;

	OwnerCharacter->GetCharacterMovement ( )->bOrientRotationToMovement = true;
	OwnerCharacter->GetCharacterMovement ( )->bUseControllerDesiredRotation = false;
}

void UCMovementComponent::OnMoveForward ( const FInputActionValue& Value )
{
	if ( !bCanMove )return;

	float Scale = Value.Get<float> ( );

	FRotator rotator = FRotator ( 0 , OwnerCharacter->GetControlRotation ( ).Yaw , 0 );
	FVector direction = FQuat ( rotator ).GetForwardVector ( );

	if ( bTopViewCamera )
		direction = FVector::XAxisVector;

	OwnerCharacter->AddMovementInput ( direction , Scale);
}

void UCMovementComponent::OnMoveRight ( const FInputActionValue& Value )
{
	if ( !bCanMove )return;
	float Scale = Value.Get<float> ( );


	FRotator rotator = FRotator ( 0 , OwnerCharacter->GetControlRotation ( ).Yaw , 0 );
	FVector direction = FQuat ( rotator ).GetRightVector ( );

	if ( bTopViewCamera )
		direction = FVector::YAxisVector;

	OwnerCharacter->AddMovementInput ( direction , Scale);
}

void UCMovementComponent::OnHorizontalLook ( const FInputActionValue& Value )
{
	float Scale = Value.Get<float> ( );


	OwnerCharacter->AddControllerYawInput ( Scale * HorizontalLook * GetWorld ( )->GetDeltaSeconds ( ) );
}

void UCMovementComponent::OnVerticalLook ( const FInputActionValue& Value )
{
	float Scale = Value.Get<float> ( );


	OwnerCharacter->AddControllerPitchInput ( Scale * VerticalLook * GetWorld ( )->GetDeltaSeconds ( ) );
}

void UCMovementComponent::Move ( )
{
	bCanMove = true;

}

void UCMovementComponent::Stop ( )
{
	bCanMove = false;
}

