// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/CWeaponComponent.h"
#include "Global.h"
#include "Component/CTargetingComponent.h"

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
	TargetComp = CHelpers::GetComponent<UCTargetingComponent> ( OwnerCharacter );


	// ...

}

void UCMovementComponent::SetSpeed ( ESpeedType InType )
{
	OwnerCharacter->GetCharacterMovement ( )->MaxWalkSpeed = Speed[(int32)InType];
}

void UCMovementComponent::OnSprint ( )
{
	//bCancelLockOn = true;
	if ( !OwnerCharacter->GetMesh ( )->GetAnimInstance ( )->IsAnyMontagePlaying () )
		TargetComp->ResetLockOn ( );
	SetSpeed ( ESpeedType::Sprint );
}

void UCMovementComponent::OnRun ( )
{
	bCancelLockOn = false;
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
	if ( !bCanMove ) return;

	ForwardScale = Value.Get<float> ( );

	FRotator rotator = FRotator ( 0 , OwnerCharacter->GetControlRotation ( ).Yaw , 0 );
	FVector ForwardVector = FQuat ( rotator ).GetForwardVector ( );
	FVector RightVector = FQuat ( rotator ).GetRightVector ( );

	if ( bTopViewCamera )
	{
		ForwardVector = FVector::XAxisVector;
		RightVector = FVector::YAxisVector;
	}

	// 최종 입력 방향 계산
	LastInputDirection = ( ForwardVector * ForwardScale + RightVector * RightScale ).GetSafeNormal ( );

	// 이동 적용
	OwnerCharacter->AddMovementInput ( ForwardVector , ForwardScale );
}

void UCMovementComponent::OnMoveRight ( const FInputActionValue& Value )
{
	if ( !bCanMove ) return;

	RightScale = Value.Get<float> ( );

	FRotator rotator = FRotator ( 0 , OwnerCharacter->GetControlRotation ( ).Yaw , 0 );
	FVector ForwardVector = FQuat ( rotator ).GetForwardVector ( );
	FVector RightVector = FQuat ( rotator ).GetRightVector ( );

	if ( bTopViewCamera )
	{
		ForwardVector = FVector::XAxisVector;
		RightVector = FVector::YAxisVector;
	}

	// 최종 입력 방향 계산
	LastInputDirection = ( ForwardVector * ForwardScale + RightVector * RightScale ).GetSafeNormal ( );

	// 이동 적용
	OwnerCharacter->AddMovementInput ( RightVector , RightScale );
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

