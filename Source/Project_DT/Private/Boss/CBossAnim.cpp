// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/CBossAnim.h"
#include "Boss/CBossEnemy.h"

void UCBossAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACBossEnemy>(TryGetPawnOwner());
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

	if(IsBacksteping)
	Speed = -(Speed);

	/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Speed: %f"), Speed));*/

	if (!Owner->FSMComponent->IsSideMoveSetting)
	{
		FVector RightVector = Owner->GetActorRightVector();
		MoveDirection = FVector::DotProduct(Velocity, RightVector);
	}
}
