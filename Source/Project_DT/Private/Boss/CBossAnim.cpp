// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/CBossAnim.h"
#include "GameFramework/Character.h"

void UCBossAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(TryGetPawnOwner());
}

void UCBossAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if ( !Owner )
	{
		return;
	}

	FVector Velocity = Owner->GetVelocity();
	FVector ForwardVector = Owner->GetActorForwardVector();

	Speed = FVector::DotProduct(Velocity, ForwardVector);

	FVector RightVector = Owner->GetActorRightVector();
	Direction = FVector::DotProduct(Velocity, RightVector);
}
