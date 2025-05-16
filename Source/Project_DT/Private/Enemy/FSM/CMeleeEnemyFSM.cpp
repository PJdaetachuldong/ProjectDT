// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/FSM/CMeleeEnemyFSM.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

// Sets default values for this component's properties
UCMeleeEnemyFSM::UCMeleeEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCMeleeEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	GetOnwerEnemy();
}


// Called every frame
void UCMeleeEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch ( State )
	{
		case EMeleeEnemyState::IDLE: { IDLEState(); } break;		
		case EMeleeEnemyState::CHASE: { CHASEState(); } break;
		case EMeleeEnemyState::WANDER: { WANDERState(); } break;
		case EMeleeEnemyState::ATTACK: { ATTACKState(); } break;
		case EMeleeEnemyState::DAMAGE: { DMAGEState(); } break;
		case EMeleeEnemyState::DIE: { DIEState(); } break;
	}
}

void UCMeleeEnemyFSM::IDLEState()
{
	if ( MyEnemy )
	{
		MyEnemy->GetCharacterMovement()->StopMovementImmediately();
	}
}

void UCMeleeEnemyFSM::CHASEState()
{
	LookAtTarget();

	//플레이어와 거리를 계산해서 거리에 따라 어떠한 이동 로직을 갖을지 정함
	float DistPlayer = FVector::Dist (MyEnemy->GetActorLocation(), MyEnemy->Target->GetActorLocation());

	//만약 플레이어와 거리가 너무 멀면 그냥 플레이어만 쫓음
	if ( DistPlayer >= MyEnemy->ChaseDistance )
	{
		/*IsSetRandomLocation = false;*/
		AI->MoveToLocation(MyEnemy->Target->GetActorLocation());
	}
}

void UCMeleeEnemyFSM::WANDERState()
{
	if ( !IsWanderMoveSet )
	{
		/*LookAtTarget();*/

		//만약 플레이어가 자신에게 다가오면 뒤로 물러나게 만듦
		if (FVector::Dist(MyEnemy->GetActorLocation(), MyEnemy->Target->GetActorLocation()) <= WanderRadius)
		{
			FVector RetreatPosition = BackstepPosition();

			UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
			if ( NavSys )
			{
				FNavLocation ProjectedLocation;
				// NavMesh에 목표 위치 투영
				if (NavSys->ProjectPointToNavigation(RetreatPosition, ProjectedLocation, FVector(100.0f)))
				{
					AI->MoveToLocation(ProjectedLocation.Location, 10.0f);
					IsWanderMoveSet = true;
				}
			}
		}
		//특정 시간 안에 불리도록 설정
		//이동해야하는 곳 위치 선정
		CurWanderDelayTime += GetWorld()->GetDeltaSeconds();
		float RandomMoveSetTime = FMath::FRandRange (5.4f, 6.6f);

		if ( CurWanderDelayTime >= RandomMoveSetTime )
		{
			SetWanderMoveLocation();
		}
	}

	else if ( IsWanderMoveSet )
	{
		/*LookAtTarget();*/

		//이동을 하게 해줌
		UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
		FNavLocation NavPoint;

		if ( NavSys->ProjectPointToNavigation(WanderMoveLocation, NavPoint, FVector(100.0f)))
		{
			AI->MoveToLocation(WanderMoveLocation);

			DrawDebugSphere(GetWorld (), WanderMoveLocation, 20.0f, 30, FColor::Yellow, false, 0.1f); // 디버그용

			//목표 위치랑 거리가 좁혀졌다면 다시 위치 설정이 이루어지도록 불 값 변경
			if ( FVector::Dist(WanderMoveLocation, MyEnemy->GetActorLocation()) <= 100.0f)
			{
				IsWanderMoveSet = false;
			}
		}
		else
		{

		}
	}

	LookAtTarget();
}

void UCMeleeEnemyFSM::ATTACKState()
{
	if (MyEnemy->GetCharacterMovement()->bOrientRotationToMovement == true)
	{
		MyEnemy->GetCharacterMovement()->bOrientRotationToMovement = false;
		MyEnemy->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	}


	if (IsSetAttackRandomLocation)
	{
		AI->MoveToLocation(AttackRandomLocation);
		DrawDebugSphere (GetWorld(), AttackRandomLocation, 20.0f, 15, FColor::Red, false, 0.1f ); // 디버그용

		if (FVector::Dist(MyEnemy->GetActorLocation(), AttackRandomLocation) <= 90.0f)
		{
			CurAttackMoveTime += GetWorld()->GetDeltaSeconds();
			if ( CurAttackMoveTime >= 2.4f )
			{
				//만약 이동한 다음 공격 범위안에 플레이어가 있으면 
				if (FVector::Dist(MyEnemy->Target->GetActorLocation(), MyEnemy->GetActorLocation()) <= 250.0f + 50.0f)
				{
					//플레이어를 공격, 공격후 공격 상태는 유지
					GEngine->AddOnScreenDebugMessage( 2 , 1.0f , FColor::Red , TEXT ( "Attack!" ) );

					IsSetAttackRandomLocation = false;
					CurAttackMoveTime = 0.0f;

					//공격권 재설정하는 함수인데 이거는 먼저 주석처리
// 					if (MyEnemy->Manager)
// 					{
// 						MyEnemy->AttackRandomInt();
// 					}
				}

				//이동한 다음 공격 범위안에 플레이어가 없으면
				else
				{
					//초기화만 해준다
					IsSetAttackRandomLocation = false;
					CurAttackMoveTime = 0.0f;
				}
			}
		}
	}
	else
	{
		/*LookAtTarget();*/

// 		//플레이어를 바라보도록 만듦
// 		FVector DirectionToPlayer = (MyEnemy->Target->GetActorLocation() - MyEnemy->GetActorLocation()).GetSafeNormal();
// 		FRotator TargetRotation = DirectionToPlayer.Rotation();
// 		TargetRotation.Pitch = 0;
// 		MyEnemy->SetActorRotation(FMath::RInterpTo(MyEnemy->GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 5.0f));

		FVector DirectionToPlayer = (MyEnemy->Target->GetActorLocation() - MyEnemy->GetActorLocation()).GetSafeNormal();

		int32 Direction = FMath::RandRange(0, 2); // 0: 좌, 1: 우, 2: 뒤,	3: 공격
		FVector Offset;
		switch ( Direction )
		{
		case 0: //좌
			Offset = FVector(0.0f, -250.0f, 0.0f);
			break;
		case 1: //우
			Offset = FVector(0.0f, 250.0f, 0.0f);
			break;
		case 2: //뒤
			Offset = FVector(-250.0f, 0.0f, 0.0f);
			break;
			// 		case 3: //공격
			// 			Offset = FVector(0.0f, -100.0f, 0.0f);
			// 			break;
		}

		FVector LocalOffset = Offset.RotateAngleAxis(DirectionToPlayer.Rotation().Yaw, FVector::UpVector);
		FVector NowTargetLocation = MyEnemy->Target->GetActorLocation() + LocalOffset;

		UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
		FNavLocation NavPoint;

		//목표 위치가 NavMesh 안에 있는지 확인
		if (NavSys->ProjectPointToNavigation(NowTargetLocation, NavPoint, FVector(100.0f)))
		{
			AttackRandomLocation = NavPoint.Location;
			IsSetAttackRandomLocation = true;
		}
		else
		{

		}
	}

	LookAtTarget();
}

void UCMeleeEnemyFSM::DMAGEState()
{

}

void UCMeleeEnemyFSM::DIEState()
{

}

void UCMeleeEnemyFSM::SetWanderMoveLocation()
{
	FVector PlayerLocation = MyEnemy->Target->GetActorLocation();
	FVector EnemyLocation = MyEnemy->GetActorLocation();
	FVector DirectionToPlayer = (EnemyLocation - PlayerLocation).GetSafeNormal2D();

	MoveClockwise = FMath::RandBool();

	float Angle = GetWorld()->GetTimeSeconds() * WanderSpeed * (MoveClockwise ? 1.0f : -1.0f);
	FVector WanderOffset = FVector (FMath::Cos(Angle) * WanderRadius, FMath::Sin(Angle) * WanderRadius, 0.0f);

	WanderMoveLocation = PlayerLocation + WanderOffset;

	IsWanderMoveSet = true;
	CurWanderDelayTime = 0.0f;
}

void UCMeleeEnemyFSM::GetOnwerEnemy()
{
	MyEnemy = Cast<ACMeleeEnemy>(GetOwner());

	AI = Cast<AAIController>(MyEnemy->GetController());
}

void UCMeleeEnemyFSM::LookAtTarget()
{
	FVector DirectionToPlayer = (MyEnemy->Target->GetActorLocation() - MyEnemy->GetActorLocation()).GetSafeNormal2D();
	FRotator TargetRotation = DirectionToPlayer.Rotation();
	FRotator CurrentRotation = MyEnemy->GetActorRotation();
	float DeltaYaw = FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, TargetRotation.Yaw);
	TargetRotation.Yaw = CurrentRotation.Yaw + DeltaYaw;

	MyEnemy->SetActorRotation(FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 10.0f));
}

FVector UCMeleeEnemyFSM::BackstepPosition()
{
	FVector TargetLocation = MyEnemy->Target->GetActorLocation();
	FVector MyLocation = MyEnemy->GetActorLocation();
	FVector ToTarget = (TargetLocation - MyLocation).GetSafeNormal2D();

	// 플레이어 반대 방향으로 목표 위치 계산
	float CurrentDistance = FVector::Dist2D(TargetLocation, MyLocation);
	float DistanceToMove = WanderRadius - CurrentDistance + BackStepDistance; // 추가 후퇴 거리
	FVector RetreatDirection = - ToTarget;
	FVector RetreatPosition = MyLocation + RetreatDirection * DistanceToMove;

	return RetreatPosition;
}