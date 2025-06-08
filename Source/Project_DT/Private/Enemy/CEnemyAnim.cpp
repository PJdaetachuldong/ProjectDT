// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/CEnemyAnim.h"

void UCEnemyAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACMeleeEnemy>(TryGetPawnOwner());
}

void UCEnemyAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Owner)
	{
		return;
	}
}
