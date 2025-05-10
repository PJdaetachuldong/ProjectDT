// Fill out your copyright notice in the Description page of Project Settings.


#include "MovemontComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UMovemontComponent::UMovemontComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMovemontComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UMovemontComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMovemontComponent::SetSpeed(ESpeedType InType)
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed[(int32)InType];
}

void UMovemontComponent::OnSprint()
{
	SetSpeed(ESpeedType::Sprint);
}

void UMovemontComponent::OnRun()
{
	SetSpeed(ESpeedType::Run);
}

void UMovemontComponent::OnWalk()
{
	SetSpeed(ESpeedType::Walk);
}

void UMovemontComponent::EnableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
}

void UMovemontComponent::DisableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}

void UMovemontComponent::OnMoveForward(float InAxis)
{
	if (!bCanMove)return;


	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	if (bTopViewCamera)
		direction = FVector::XAxisVector;

	OwnerCharacter->AddMovementInput(direction, InAxis);
}

void UMovemontComponent::OnMoveRight(float InAxis)
{
	if (!bCanMove)return;


	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	if (bTopViewCamera)
		direction = FVector::YAxisVector;

	OwnerCharacter->AddMovementInput(direction, InAxis);
}

void UMovemontComponent::OnHorizontalLook(float InAxis)
{
	if (!bFixedCamera)return;

	OwnerCharacter->AddControllerYawInput(InAxis * HorizontalLook * GetWorld()->GetDeltaSeconds());
}

void UMovemontComponent::OnVerticalLook(float InAxis)
{
	if (!bFixedCamera)return;


	OwnerCharacter->AddControllerPitchInput(InAxis * VerticalLook * GetWorld()->GetDeltaSeconds());
}

void UMovemontComponent::Move()
{
	bCanMove = true;
}

void UMovemontComponent::Stop()
{
	bCanMove = false;
}

