// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AIController/CMeleeAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "GameFramework/Character.h"
#include "Global.h"

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

void ACMeleeAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACMeleeAIController::SetRotation(ACharacter* Player, ACharacter* Enemy)
{
	if (!GetWorld ( ) )
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "SetRotation: World is null" ) );
		return;
	}

	// 플레이어 방향 계산
	FVector PlayerLocation = Player->GetActorLocation();
	FVector PawnLocation = Enemy->GetActorLocation();
	FVector DirectionToPlayer = (PlayerLocation - PawnLocation).GetSafeNormal();

	// Yaw 회전만 조정 (Pitch는 유지)
	FRotator TargetRotation = FRotationMatrix::MakeFromX(DirectionToPlayer).Rotator();
	TargetRotation.Pitch = 0.0f; // Pitch를 0으로 설정해 수평 회전만 적용
	TargetRotation.Roll = 0.0f;

	// 부드러운 회전을 위해 Interp 사용 (선택 사항)
	FRotator CurrentRotation = Enemy->GetActorRotation();
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 20.0f);

	// Pawn 회전 설정
	Enemy->SetActorRotation (NewRotation);
}
