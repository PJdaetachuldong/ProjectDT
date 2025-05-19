// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AIController/CMeleeAIController.h"
#include "Navigation/CrowdFollowingComponent.h"

void ACMeleeAIController::BeginPlay()
{
	Super::BeginPlay();

	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		//CrowdComp->SetCrowdAvoidance(true);
		CrowdComp->SetCrowdSeparation(true);
		CrowdComp->SetCrowdSeparationWeight(3.0f); // 에너미 간 거리 유지 강도
		CrowdComp->SetCrowdAvoidanceRangeMultiplier(1.5f); // 회피 범위 증가
	}
}
