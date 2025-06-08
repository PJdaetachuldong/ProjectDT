// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/FSM/CMeleeEnemyFSM.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"
#include "Enemy/AIController/CMeleeAIController.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/CEnemyAnim.h"

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

	GetOwnerEnemy();
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
		case EMeleeEnemyState::BREAK: { BREAKState(); } break;
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
	/*LookAtTarget();*/
	if (MyEnemy->Target, MyEnemy)
	{
		AI->SetRotation(MyEnemy->Target, MyEnemy);
	}

	//플레이어와 거리를 계산해서 거리에 따라 어떠한 이동 로직을 갖을지 정함
	float DistPlayer = FVector::Dist (MyEnemy->GetActorLocation(), MyEnemy->Target->GetActorLocation());

	//만약 플레이어와 거리가 너무 멀면 그냥 플레이어만 쫓음
	if ( DistPlayer >= MyEnemy->ChaseDistance )
	{
		/*IsSetRandomLocation = false;*/
		AI->MoveToLocation(MyEnemy->Target->GetActorLocation());

		Cast<UCEnemyAnim>(MyEnemy->AnimInstance)->State = EMeleeEnemyState::CHASE;
	}
}

void UCMeleeEnemyFSM::WANDERState()
{
// 	if (MyEnemy->GetCharacterMovement()->bOrientRotationToMovement == true)
// 	{
// 		MyEnemy->GetCharacterMovement()->bOrientRotationToMovement = false;
// 		MyEnemy->GetCharacterMovement()->bUseControllerDesiredRotation = false;
// 	}

	if ( !IsWanderMoveSet )
	{
		/*LookAtTarget();*/

		//만약 플레이어가 자신에게 다가오면 뒤로 물러나게 만듦
// 		if (FVector::Dist(MyEnemy->GetActorLocation(), MyEnemy->Target->GetActorLocation()) <= WanderRadius)
// 		{
// 			FVector RetreatPosition = BackstepPosition();
// 
// 			UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
// 			if ( NavSys )
// 			{
// 				FNavLocation ProjectedLocation;
// 				// NavMesh에 목표 위치 투영
// 				if (NavSys->ProjectPointToNavigation(RetreatPosition, ProjectedLocation))
// 				{
// 					AI->MoveToLocation( RetreatPosition , 10.0f);
// 					IsWanderMoveSet = true;
// 				}
// 			}
// 		}

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

				MyEnemy->bUseControllerRotationYaw = true; 
				MyEnemy->GetCharacterMovement()->bOrientRotationToMovement = true;

				Cast<UCEnemyAnim>(MyEnemy->AnimInstance)->State = EMeleeEnemyState::IDLE;
			}
		}
		else
		{
			
		}
	}

	LookAtTarget();

	AI->SetRotation(MyEnemy->Target, MyEnemy);
}

void UCMeleeEnemyFSM::ATTACKState()
{
// 	if (MyEnemy->GetCharacterMovement()->bOrientRotationToMovement == true)
// 	{
// 		MyEnemy->GetCharacterMovement()->bOrientRotationToMovement = false;
// 		MyEnemy->GetCharacterMovement()->bUseControllerDesiredRotation = false;
// 	}

	if (IsSetAttackRandomLocation)
	{
		AI->MoveToLocation(AttackRandomLocation);
		DrawDebugSphere (GetWorld(), AttackRandomLocation, 20.0f, 15, FColor::Red, false, 0.1f ); // 디버그용

		//자신과 공격랜덤 위치가 120이하일 경우
		if (FVector::Dist(MyEnemy->GetActorLocation(), AttackRandomLocation) <= 120.0f)
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

					Cast<UCEnemyAnim>(MyEnemy->AnimInstance)->State = EMeleeEnemyState::IDLE;
				}
			}
		}
	}
	else
	{
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

			Cast<UCEnemyAnim>(MyEnemy->AnimInstance)->State = EMeleeEnemyState::CHASE;
		}
		else
		{

		}
	}

	/*LookAtTarget();*/
	AI->SetRotation(MyEnemy->Target, MyEnemy );
}

void UCMeleeEnemyFSM::DMAGEState()
{

}

void UCMeleeEnemyFSM::BREAKState()
{
	//AI 움직임 멈춤
	AI->StopMovement();

	//만약 브레이크 애니메이션이 재생중이지 않으면 재생하게 만듦
	if (!MyEnemy->AnimInstance->Montage_IsPlaying(MyEnemy->AM_Break))
	{
		MyEnemy->AnimInstance->Montage_Play(MyEnemy->AM_Break);
	}
}

void UCMeleeEnemyFSM::DIEState()
{
	if (Cast<UCEnemyAnim>(MyEnemy->AnimInstance)->State != EMeleeEnemyState::DIE)
	{
		//모든 몽타주 재생을 멈춤
		MyEnemy->AnimInstance->StopAllMontages(0.4f);

		Cast<UCEnemyAnim>(MyEnemy->AnimInstance)->State = EMeleeEnemyState::DIE;
		MyEnemy->GetCapsuleComponent()->SetCollisionProfileName(FName("BlockAll"));
	}
}

void UCMeleeEnemyFSM::SetWanderMoveLocation()
{
	MyEnemy->bUseControllerRotationYaw = false;
	MyEnemy->GetCharacterMovement()->bOrientRotationToMovement = false;

	FVector PlayerLocation = MyEnemy->Target->GetActorLocation();
	FVector EnemyLocation = MyEnemy->GetActorLocation();
	FVector DirectionToPlayer = (EnemyLocation - PlayerLocation).GetSafeNormal2D();

	MoveClockwise = FMath::RandBool();

	float Angle = GetWorld()->GetTimeSeconds() * WanderSpeed * (MoveClockwise ? 1.0f : -1.0f);
	FVector WanderOffset = FVector (FMath::Cos(Angle) * WanderRadius, FMath::Sin(Angle) * WanderRadius, 0.0f);

	WanderMoveLocation = PlayerLocation + WanderOffset;

	//위치로 정한곳이 NavMesh로 이동할 수 있는지 체크

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
		FNavLocation NavPoint;

// 	if ( NavSys->ProjectPointToNavigation(WanderMoveLocation, NavPoint))
// 	{
		IsWanderMoveSet = true;
		CurWanderDelayTime = 0.0f;

		Cast<UCEnemyAnim>(MyEnemy->AnimInstance)->State = EMeleeEnemyState::WANDER;
// 	}
// 
// 	else
// 	{
// 		SetWanderMoveLocation();
// 	}

// 	IsWanderMoveSet = true;
// 	CurWanderDelayTime = 0.0f;
	
}

void UCMeleeEnemyFSM::GetOwnerEnemy()
{
	MyEnemy = Cast<ACMeleeEnemy>(GetOwner());

	AI = Cast<ACMeleeAIController>(MyEnemy->GetController());
}

void UCMeleeEnemyFSM::LookAtTarget()
{
	// 플레이어 위치
	FVector PlayerLocation = MyEnemy->Target->GetActorLocation();
	// 현재 AI 위치
	FVector AILocation = MyEnemy->GetActorLocation();
	// 플레이어를 향하는 방향 벡터 계산
	FVector DirectionToPlayer = (PlayerLocation - AILocation).GetSafeNormal();
	// 방향 벡터를 회전으로 변환 (Yaw만 고려)
	FRotator LookAtRotation = FRotationMatrix::MakeFromX(DirectionToPlayer).Rotator();
	// Z축 회전(Yaw)만 적용하여 캐릭터가 플레이어를 향하도록 설정
	FRotator NewRotation = FRotator(0.0f, LookAtRotation.Yaw, 0.0f);
	// 플레이어를 바라보게 고정함
	MyEnemy->SetActorRotation(NewRotation);

// 	FVector DirectionToPlayer = (MyEnemy->Target->GetActorLocation() - MyEnemy->GetActorLocation()).GetSafeNormal2D();
// 	FRotator TargetRotation = DirectionToPlayer.Rotation();
// 	FRotator CurrentRotation = MyEnemy->GetActorRotation();
// 	float DeltaYaw = FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, TargetRotation.Yaw);
// 	TargetRotation.Yaw = CurrentRotation.Yaw + DeltaYaw;
// 
// 	MyEnemy->SetActorRotation(FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 10.0f));
// 
// 	// 플레이어 방향 계산
// 	FVector PlayerLocation = MyEnemy->Target->GetActorLocation ( );
// 	FVector PawnLocation = MyEnemy->GetActorLocation ( );
// 	FVector DirectionToPlayer = ( PlayerLocation - PawnLocation ).GetSafeNormal ( );
// 
// 	// Yaw 회전만 조정 (Pitch는 유지)
// 	FRotator TargetRotation = FRotationMatrix::MakeFromX ( DirectionToPlayer ).Rotator ( );
// 	TargetRotation.Pitch = 0.0f; // Pitch를 0으로 설정해 수평 회전만 적용
// 	TargetRotation.Roll = 0.0f;
// 
// 	// 부드러운 회전을 위해 Interp 사용 (선택 사항)
// 	FRotator CurrentRotation = MyEnemy->GetActorRotation ( );
// 	FRotator NewRotation = FMath::RInterpTo ( CurrentRotation , TargetRotation , GetWorld()->GetDeltaSeconds(),20.0f);
// 
// 	// Pawn 회전 설정
// 	MyEnemy->SetActorRotation ( NewRotation );
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