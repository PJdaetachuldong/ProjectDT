// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss/FSM/CBossFSM.h"
#include "Boss/CBossEnemy.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Boss/RangedAttackObject/CRangeAttack.h"
#include "Character/CPlayer.h"
#include "Boss/CBossAnim.h"
#include "NavigationSystem.h"
#include "Components/CapsuleComponent.h"
#include "Widget/BossWidget.h"
#include "Boss/CBossWeapon.h"
#include "Components/ArrowComponent.h"

UCBossFSM::UCBossFSM()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCBossFSM::BeginPlay()
{
	Super::BeginPlay();

	GetOwnerEnemy();
}

void UCBossFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!MyBoss->BossStart) return;

	//플레이어와의 거리 체크는 후 공격을 정하던가, 후속타 여부를 위해 체크가 지속적으로 일어나야함
	//아니면 함수로 빼서 특정 상황일때만 체크를 해서 여부를 체크하는 것도 나쁘지 않을것 같음

	if ( State != EBossState::IDLE )
	{
		TargetDist = FVector::Dist(MyBoss->Target->GetActorLocation(), MyBoss->GetActorLocation());
	}

	switch ( State )
	{
		case EBossState::IDLE: { IDLEState(); } break;
		case EBossState::ATTACK: { ATTACKState ( ); } break;
		case EBossState::DAMAGE: { DAMAGEState ( ); } break;
		case EBossState::BREAK: { BREAKState ( ); } break;
		case EBossState::DIE: { DIEState ( ); }	break;
	}
}

void UCBossFSM::IDLEState()
{
	//일정 시간이 지나면 IDLE에서 CHASE상태가 되게, 나중에 애니메이션 넣어서 노티파이로 변환되게 해주기
	if ( Cast<UCBossAnim>(MyBoss->AnimInstance)->IsPlayingIdle )
	{
		State = EBossState::ATTACK;

		//천천히 걸어온 느낌을 주기 위해 걷는 속도를 조절
		MyBoss->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	}
	//일정 시간이 지나면 IDLE에서 CHASE상태가 되게, 나중에 애니메이션 넣어서 노티파이로 변환되게 해주기
}

void UCBossFSM::ATTACKState()
{
	//공격 상태에서 또다시 여러가지 공격 패턴으로 나뉘어짐
	switch ( AttackState )
	{
	case EBossATTACKState::NONE: { NONEState ( ); } break;
	case EBossATTACKState::SETATK: { SETATKState(); } break;
	case EBossATTACKState::RANGEDATTACK: { RANGEDATTACKState ( ); } break;
	case EBossATTACKState::DASHATTACK: { DASHATTACKState ( ); } break;
	case EBossATTACKState::COMBOATTACK: { COMBOATTACKState ( ); } break;
	case EBossATTACKState::COUNTERATTACK: { COUNTERATTACKState ( ); } break;
	case EBossATTACKState::SPATTACK: { SPATTACKState ( ); } break;
	}
}

void UCBossFSM::DAMAGEState()
{
	
}

void UCBossFSM::BREAKState()
{
	//AI 움직임 멈춤
	AI->StopMovement();

	//만약 브레이크 애니메이션이 재생중이지 않으면 재생하게 만듦
	if ( !MyBoss->AnimInstance->Montage_IsPlaying ( MyBoss->AM_Break ) )
	{
		MyBoss->AnimInstance->Montage_Play ( MyBoss->AM_Break );
	}

// 	//브레이크 상태가 되면 브레이크의 지속시간을 체크함
// 	CurBreakTime +=GetWorld()->GetDeltaSeconds();
// 
// 	//지속시간이 끝났다면
// 	if ( CurBreakTime >= BreakLimitTime )
// 	{
// 		//일어선 다음
// 
// 		//CHASE상태로 변환? ATTACK상태로 변환? 나중에 정하기
// 		State = EBossState::ATTACK;
// 	}
}

void UCBossFSM::DIEState()
{
	if(Cast<UCBossAnim>(MyBoss->AnimInstance)->State != EBossState::DIE)
	{ 
		if (MyBoss->BossUI->IsInViewport())
		{
			MyBoss->BossUI->RemoveFromParent();
			
			MyBoss->SpawnWeapon->SetActorEnableCollision(ECollisionEnabled::NoCollision);
		}

		//모든 몽타주 재생을 멈춤
		MyBoss->AnimInstance->StopAllMontages(0.4f);

		Cast<UCBossAnim>(MyBoss->AnimInstance)->State = EBossState::DIE;

		//AI 움직임 멈춤
		AI->StopMovement();
		
		State = EBossState::DIE;
		AttackState = EBossATTACKState::NONE;
		MyBoss->GetCapsuleComponent()->SetCollisionProfileName(FName("BlockAll"));
	}
}

void UCBossFSM::NONEState()
{
	//쉴드가 까진 상태면 밑에 실행 안함
	if(MyBoss->CurShieldAmount <=0.0f) return;

	if (Cast<UCBossAnim>(MyBoss->AnimInstance)->MoveDirection == 100.0f || Cast<UCBossAnim>(MyBoss->AnimInstance)->MoveDirection == -100.0f)
	{
		Cast<UCBossAnim>(MyBoss->AnimInstance)->MoveDirection = 0.0f;
	}

// 	if (!MyBoss->bUseControllerRotationYaw)
// 	{
// 		MyBoss->bUseControllerRotationYaw = true;; // AI 컨트롤러 회전 비활성화
// 		MyBoss->GetCharacterMovement()->bOrientRotationToMovement = true; // 이동 방향 회전 비활성화
// 	}

// 	//일정 거리까지는 플레이어를 향해 움직이게 만듦
// 	if (TargetDist >= 350.0f)
// 	{
// 		//플레이어를 향해서 움직이게 만듦
// 		AI->MoveToLocation(MyBoss->Target->GetActorLocation());
// 	}
// 
// 	else if (TargetDist > 200.0f && TargetDist < 350.0f)
// 	{
// 
// 	}
// 
// 	//플레이어가 가까우면 뒤로 움직이게 만듦
// 	else if (TargetDist <= 200.0f)
// 	{
// 		FVector RetreatPosition = BackstepPosition();
// 
// 		UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
// 		if (NavSys)
// 		{
// 			FNavLocation ProjectedLocation;
// 			// NavMesh에 목표 위치 투영
// 			if (NavSys->ProjectPointToNavigation(RetreatPosition, ProjectedLocation))
// 			{
// 				AI->MoveToLocation(RetreatPosition, 10.0f);
// 			}
// 		}
// 	}

	if (TargetDist >= 200.0f)
	{
		//움직인 거리가 600이상일 경우
		if (TotalMoveDistance >= 600.0f)
		{
			int32 SideMoveCheckInt = 0;

			if (!IsSideMoveSetting)
			{
				SideMoveCheckInt = FMath::RandRange(1, 10);
			}

			//랜덤값이 4 이하
			if (SideMoveCheckInt <= 4)
			{
				//좌우 이동하게 만듦
				SideMove();
				return;
			}

			//랜덤값이 5이상
			else if (SideMoveCheckInt >= 5)
			{
				//움직인 거리값 초기화
				TotalMoveDistance = 0.0f;
			}
		}

		//아닐 경우엔 그냥 플레이어를 향해 움직이도록
		else
		{
			// 현재 위치
			FVector CurrentLocation = MyBoss->GetActorLocation();

			// 이전 위치와의 거리 계산
			float DistanceThisFrame = FVector::Distance(CurrentLocation, LastLocation);

			// 총 이동 거리에 추가
			TotalMoveDistance += DistanceThisFrame;

			// 현재 위치를 이전 위치로 업데이트
			LastLocation = CurrentLocation;

			//플레이어를 향해서 움직이게 만듦
			AI->MoveToLocation(MyBoss->Target->GetActorLocation());

			GEngine->AddOnScreenDebugMessage(160, 1.0f, FColor::White, TEXT("State: NONE, TargetDIst >= 200"));
		}
	}

	//임시로 약간 떨어트리는 코드
	else if (TargetDist <= 300.0f)
	{
		//이동을 멈추고 
		AI->StopMovement();

		GEngine->AddOnScreenDebugMessage(165, 1.0f, FColor::White, TEXT("State: NONE, TargetDIst <= 300"));

		//좌우 이동이나 뒷 걸음질, 그리고 공격 조건을 만족하는지 체크한다
		AttackState = EBossATTACKState::SETATK;

// 		// 플레이어 위치
// 		FVector PlayerLocation = MyBoss->Target->GetActorLocation();
// 		// 현재 AI 위치
// 		FVector AILocation = MyBoss->GetActorLocation();
// 		// 플레이어를 향하는 방향 벡터 계산
// 		FVector DirectionToPlayer = (PlayerLocation - AILocation).GetSafeNormal();
// 		// 방향 벡터를 회전으로 변환 (Yaw만 고려)
// 		FRotator LookAtRotation = FRotationMatrix::MakeFromX(DirectionToPlayer).Rotator();
// 		// Z축 회전(Yaw)만 적용하여 캐릭터가 플레이어를 향하도록 설정
// 		FRotator NewRotation = FRotator(0.0f, LookAtRotation.Yaw, 0.0f);
// 		FRotator CurrentRotation = MyBoss->GetActorRotation();
// 		FRotator SetRotation = FMath::RInterpTo(CurrentRotation, NewRotation, GetWorld()->GetDeltaSeconds(), 3.0f);
// 		// 플레이어를 바라보게 고정함
// 		MyBoss->SetActorRotation(SetRotation);
// 
// 		/*Cast<UCBossAnim>(MyBoss->AnimInstance)->IsTurnIDLE = true;*/
// 
// 		if (Cast<UCBossAnim>(MyBoss->AnimInstance)->MoveDirection >= 0.1f
// 		|| Cast<UCBossAnim>(MyBoss->AnimInstance)->MoveDirection <= 0.1f)
// 		{
// 			Cast<UCBossAnim>(MyBoss->AnimInstance)->MoveDirection = 0;
// 		}
	}
	//임시로 약간 떨어트리는 코드

	

	//피격 상태면 아래가 진행되지않게
	if(MyBoss->AnimInstance->Montage_IsPlaying(MyBoss->AM_ShieldHit)) 
	{
		AI->StopMovement();
		return;
	}

	//공격 패턴 우선 순위
	//SPATTACK(필살기) > RANGEDATTACK(원거리 공격) > DASHATTACK(대쉬 공격)
	//COUNTERATTACK(가드 공격) > COMBOATTACK (콤보 공격)
	
	//임의로 되는 필살기 패턴 테스트
	/*TestCurSPTime += GetWorld()->GetDeltaSeconds();*/
	//임의로 되는 필살기 패턴 테스트

	//필살기 사용 부분
	//체력이 반 이하로 내려갔을 경우, 필살기를 한번 안하였거나 일정 횟수 공격을 했을 경우
	if ( MyBoss->CurHP<= MyBoss->MaxHP / 2 && !IsFirstSPAttack || CurAttackStack >= SPAttackStack)
	{
		if (!IsFirstSPAttack)
		{
			IsFirstSPAttack= true;
		}
		//필살기 상태로 전환
		AttackState = EBossATTACKState::SPATTACK;

		CurAttackStack = 0;

		//공격 스택 랜덤으로 해줌
		SPAttackStack = FMath::RandRange(17, 23);

		//뒤에 코드 실행 안되게 막음
		return;
	}
	//필살기 사용 부분

	//공격 NONE상태에서 거리를 체크해서 거리가 멀 경우
	if (TargetDist >= LongDist)
	{
		//거리가 먼 상태가 얼마나 지속되었는지 체크
		CurChaseTime += GetWorld()->GetDeltaSeconds();

		//거리가 먼 상태에서 플레이어가 물약을 마시는 모션을 할 경우
		//좌우 이동은 안 하고 있어야함
		if (!IsSideMoveSetting && CurChaseTime >= 25.0f)
		{
			//현재 체력이 50퍼 이하면
			if (IsFirstSPAttack)
			{
				//공격 스택을 하나 더 추가
				CurAttackStack++;
			}
			
			//바로 원거리 공격하게 변환
			AttackState = EBossATTACKState::RANGEDATTACK;
			//원거리 공격이 바로 실행되게 이곳에서 한번 실행
			/*SpawnRangedActor();*/
			//뒤에 코드 안불리게 리턴
			return;
		}

		//거리가 먼 상태가 일정 시간 누적될 경우
		//좌우 이동은 안 하고 있어야함
		if (!IsSideMoveSetting && CurChaseTime >= DashAttackCooltime)
		{
			//현재 체력이 50퍼 이하면
			if (IsFirstSPAttack)
			{
				//공격 스택을 하나 더 추가
				CurAttackStack++;
			}

			//그리고 대쉬 공격이 이루어지도록 공격상태도 변환
			AttackState = EBossATTACKState::DASHATTACK;

			//빠르게 달리는 느낌을 주기 위하여 이동속도 바꾸기
			MyBoss->GetCharacterMovement()->MaxWalkSpeed = 1100.0f;

			//뒤에 코드 실행 안되게 리턴
			return;
		}
	}

	//타겟과의 거리가 공격 범위 보다 적을 경우
	else if (TargetDist <= MyBoss->AttackRange)
	{
		//가드 게이지를 채워줌
		MyBoss->GuardGage += GetWorld()->GetDeltaSeconds();
		//가드 게이지를 채워줌

		//콤보 공격 타임을 더함
		CurComboAttackTime += GetWorld()->GetDeltaSeconds();

		//만약 가드 조건이 충족되었을 경우
		if (MyBoss->GuardGage >= MyBoss->GuardPlaying /*4.0f*/)
		{
			//현재 체력이 50퍼 이하면
			if (IsFirstSPAttack)
			{
				//공격 스택을 하나 더 추가
				CurAttackStack++;
			}

			//가드 상태로 변화
			AttackState = EBossATTACKState::COUNTERATTACK;

			//밑에 코드 안 일어나게 리턴
			return;
		}

		//콤보 공격 쿨타임이 되었을 경우
		else if (CurComboAttackTime >= ComboCooltime)
		{
			//현재 체력이 50퍼 이하면
			if (IsFirstSPAttack)
			{
				//공격 스택을 하나 더 추가
				CurAttackStack++;
			}
			
			//랜덤 공격이 실행
			SetCOMBOATTACKState(FMath::RandRange(0, 1));
		}
	}

// 	//공격 NONE상태에서 거리를 체크해서 거리가 멀 경우
// 	if ( TargetDist >= LongDist )
// 	{
// 		//거리가 먼 상태가 얼마나 지속되었는지 체크한다
// 		CurChaseTime += GetWorld()->GetDeltaSeconds();
// 
// 		//거리가 먼 상태인데 플레이어가 물약 마시는 모션을 할 경우
// 		if (/*임시로 다른 조건 넣었음*/ CurChaseTime >= 10.0f)
// 		{
// 			//임시로 넣은거 나중에 지워야함 제발!
// 			CurChaseTime = 0.0f;
// 
// 			//바로 원거리 공격하게 변환
// 			AttackState = EBossATTACKState::RANGEDATTACK;
// 			//원거리 공격이 바로 실행되게 이곳에서 한번 실행
// 			SpawnRangedActor((MyBoss->Target->GetActorLocation() - MyBoss->GetActorLocation()).GetSafeNormal2D());
// 			//뒤에 코드 안불리게 리턴
// 			return;
// 		}
// 		//거리가 먼 상태인데 플레이어가 물약 마시는 모션을 할 경우
// 
// 		//만약 거리가 먼 상태가 일정 시간 유지되었다면
// 		if ( CurChaseTime >= DashAttackCooltime )
// 		{
// 			//그리고 대쉬 공격이 이루어지도록 공격상태도 변환
// 			AttackState = EBossATTACKState::DASHATTACK;
// 
// 			//빠르게 달리는 느낌을 주기 위하여 이동속도 바꾸기
// 			MyBoss->GetCharacterMovement()->MaxWalkSpeed = 1100.0f;
// 
// 			//뒤에 코드 실행 안되게 리턴
// 			return;
// 		}
// 	}
// 	//공격 NONE상태에서 거리를 체크해서 거리가 멀 경우
// 	
// 	//공격 NONE상태에서 거리를 체크해서 거리가 가까울 경우
// 	if ( TargetDist <= MyBoss->AttackRange )
// 	{
// 		//가드 게이지를 채워줌
// 		MyBoss->GuardGage += GetWorld()->GetDeltaSeconds();
// 		//가드 게이지를 채워줌
// 
// 		//콤보 공격 타임을 더함
// 		CurComboAttackTime += GetWorld()->GetDeltaSeconds();
// 
// 		//만약 가드 조건이 충족되었을 경우
// 		if ( MyBoss->GuardGage >= MyBoss->GuardPlaying /*4.0f*/ )
// 		{
// 			//가드 상태로 변화
// 			AttackState = EBossATTACKState::COUNTERATTACK;
// 
// 			//밑에 코드 안 일어나게 리턴
// 			return;
// 		}
// 
// 		//콤보 공격 쿨타임이 되었을 경우
// 		else if ( CurComboAttackTime >= ComboCooltime )
// 		{	
// 			//랜덤 공격이 실행
// 			SetCOMBOATTACKState(FMath::RandRange(0, 1));
// 		}
// 	}
}

void UCBossFSM::SETATKState()
{
	//피격 상태면 아래가 진행되지않게
	if (MyBoss->AnimInstance->Montage_IsPlaying(MyBoss->AM_ShieldHit))
	{
		AI->StopMovement();
		return;
	}

	if (TargetDist <= 300.0f)
	{
		SetATKState = ESetATKState::BACKSTEP;
	}

	switch (SetATKState)
	{
	case ESetATKState::SETATKNONE: { SETATKNONEState(); } break;
	case ESetATKState::BACKSTEP: { BACKSTEPState(); } break;
	case ESetATKState::SIDEMOVE: { SIDEMOVEState(); } break;
	}

	//공격 패턴 우선 순위
	//SPATTACK(필살기) > RANGEDATTACK(원거리 공격) > DASHATTACK(대쉬 공격)
	//COUNTERATTACK(가드 공격) > COMBOATTACK (콤보 공격)

	//임의로 되는 필살기 패턴 테스트
	/*TestCurSPTime += GetWorld()->GetDeltaSeconds();*/
	//임의로 되는 필살기 패턴 테스트

	//필살기 사용 부분
	//체력이 반 이하로 내려갔을 경우, 필살기를 한번 안하였거나 일정 횟수 공격을 했을 경우
	if (MyBoss->CurHP <= MyBoss->MaxHP / 2 && !IsFirstSPAttack || CurAttackStack >= SPAttackStack)
	{
		if (!IsFirstSPAttack)
		{
			IsFirstSPAttack = true;
		}
		//필살기 상태로 전환
		AttackState = EBossATTACKState::SPATTACK;

		CurAttackStack = 0;

		//공격 스택 랜덤으로 해줌
		SPAttackStack = FMath::RandRange(17, 23);

		//뒤에 코드 실행 안되게 막음
		return;
	}
	//필살기 사용 부분

	//공격 NONE상태에서 거리를 체크해서 거리가 멀 경우
	if (TargetDist >= LongDist)
	{
		//거리가 먼 상태가 얼마나 지속되었는지 체크
		CurChaseTime += GetWorld()->GetDeltaSeconds();

		//거리가 먼 상태에서 플레이어가 물약을 마시는 모션을 할 경우
		//좌우 이동은 안 하고 있어야함
		if (!IsSideMoveSetting && CurChaseTime >= 25.0f)
		{
			//현재 체력이 50퍼 이하면
			if (IsFirstSPAttack)
			{
				//공격 스택을 하나 더 추가
				CurAttackStack++;
			}

			//바로 원거리 공격하게 변환
			AttackState = EBossATTACKState::RANGEDATTACK;
			//원거리 공격이 바로 실행되게 이곳에서 한번 실행
			/*SpawnRangedActor();*/
			//뒤에 코드 안불리게 리턴
			return;
		}

		//거리가 먼 상태가 일정 시간 누적될 경우
		//좌우 이동은 안 하고 있어야함
		if (!IsSideMoveSetting && CurChaseTime >= DashAttackCooltime)
		{
			//현재 체력이 50퍼 이하면
			if (IsFirstSPAttack)
			{
				//공격 스택을 하나 더 추가
				CurAttackStack++;
			}

			//그리고 대쉬 공격이 이루어지도록 공격상태도 변환
			AttackState = EBossATTACKState::DASHATTACK;

			//빠르게 달리는 느낌을 주기 위하여 이동속도 바꾸기
			MyBoss->GetCharacterMovement()->MaxWalkSpeed = 1100.0f;

			//뒤에 코드 실행 안되게 리턴
			return;
		}
	}

	//타겟과의 거리가 공격 범위 보다 적을 경우
	else if (TargetDist <= MyBoss->AttackRange)
	{
		//가드 게이지를 채워줌
		MyBoss->GuardGage += GetWorld()->GetDeltaSeconds();
		//가드 게이지를 채워줌

		//콤보 공격 타임을 더함
		CurComboAttackTime += GetWorld()->GetDeltaSeconds();

		//만약 가드 조건이 충족되었을 경우
		if (MyBoss->GuardGage >= MyBoss->GuardPlaying /*4.0f*/)
		{
			//현재 체력이 50퍼 이하면
			if (IsFirstSPAttack)
			{
				//공격 스택을 하나 더 추가
				CurAttackStack++;
			}

			//가드 상태로 변화
			AttackState = EBossATTACKState::COUNTERATTACK;

			//밑에 코드 안 일어나게 리턴
			return;
		}

		//콤보 공격 쿨타임이 되었을 경우
		else if (CurComboAttackTime >= ComboCooltime)
		{
			//현재 체력이 50퍼 이하면
			if (IsFirstSPAttack)
			{
				//공격 스택을 하나 더 추가
				CurAttackStack++;
			}

			//랜덤 공격이 실행
			SetCOMBOATTACKState(FMath::RandRange(0, 1));
		}
	}
}

void UCBossFSM::RANGEDATTACKState()
{
	//원거리 공격을 할 것이기에 이동은 멈춤
	AI->StopMovement();

	//만약 원거리 공격 애니메이션이 재생중이지 않으면 재생하게 만듦
	if (!MyBoss->AnimInstance->Montage_IsPlaying(MyBoss->AM_RangedAttack))
	{
		MyBoss->AnimInstance->Montage_Play(MyBoss->AM_RangedAttack);
	}

// 	//타겟과 자신의 방향을 체크
// 	FVector TargetLocation = MyBoss->Target->GetActorLocation();
// 	FVector MyLocation = MyBoss->GetActorLocation();
// 	FVector DirectionToTarget = (TargetLocation - MyLocation).GetSafeNormal2D();
// 
// 	//해당 방향으로 원거리 공격 발사
// 	//애니메이션을 넣는다면 해당 애니메이션 노티파이로 원거리 공격 발사되게
// 	CurRandgedTime += GetWorld ( )->GetDeltaSeconds ( );
// 
// 	//여기서 코드를 작성하면 Tick으로 여러번 불리기에 여기에 작성하면 안됨
// 	/*SpawnRangedActor(DirectionToTarget);*/
// 
// 	if ( CurRandgedTime >= 2.5f )
// 	{
// 		SpawnRangedActor ( DirectionToTarget );
// 		AttackState = EBossATTACKState::NONE;
// 		CurRandgedTime = 0.0f;
// 		return;
// 	}
}

void UCBossFSM::SideMove()
{
	if (!IsSideMoveSetting)
	{
		MyBoss->bUseControllerRotationYaw = false; // AI 컨트롤러 회전 비활성화
		MyBoss->GetCharacterMovement()->bOrientRotationToMovement = false; // 이동 방향 회전 비활성화

		// 현재 Pawn의 위치
		FVector CurrentLocation = MyBoss->GetActorLocation();

		// 랜덤으로 좌(-1) 또는 우(1) 방향 선택
		float Direction = FMath::RandBool() ? 1.0f : -1.0f;

		if (Direction >= 0.1f)
		{
			 Cast<UCBossAnim>(MyBoss->AnimInstance)->MoveDirection = 100;
		}

		else if (Direction <= -0.1f)
		{
			Cast<UCBossAnim>(MyBoss->AnimInstance)->MoveDirection = -100;
		}

		// 이동할 거리
		float SideDistance = 550.0f;

		// Pawn의 오른쪽 방향 벡터를 기준으로 좌/우 방향 계산
		FVector SideVector = MyBoss->GetActorRightVector() * Direction * SideDistance;

		// 목표 위치 계산
		TargetSideLocation = CurrentLocation + SideVector;

		// 네비게이션 시스템을 통해 유효한 위치로 보정
		FNavLocation NavLocation;
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		if (NavSystem && NavSystem->ProjectPointToNavigation(TargetSideLocation, NavLocation))
		{
			TargetSideLocation = NavLocation.Location;

			//좌우 이동 셋팅이 끝났음을 알림
			IsSideMoveSetting = true;
		}

		else
		{
			//좌우 이동을 안하고 초기화
			TotalMoveDistance = 0.0f;

			IsSideMoveSetting = false;
		}
	}

	if (IsSideMoveSetting)
	{
		// 목표 위치로 이동
		AI->MoveToLocation(TargetSideLocation);

// 		if (!MyBoss->bUseControllerRotationYaw)
// 		{
// 			MyBoss->bUseControllerRotationYaw = true;
// 		}
	
		//회전 뒤집히는거 방지용 거리가 있을때만 플레이어 바라보게
		if (TargetDist >= 150.0f)
		{
			// 플레이어 위치
			FVector PlayerLocation = MyBoss->Target->GetActorLocation();
			// 현재 AI 위치
			FVector AILocation = MyBoss->GetActorLocation();
			// 플레이어를 향하는 방향 벡터 계산
			FVector DirectionToPlayer = (PlayerLocation - AILocation).GetSafeNormal();
			// 방향 벡터를 회전으로 변환 (Yaw만 고려)
			FRotator LookAtRotation = FRotationMatrix::MakeFromX(DirectionToPlayer).Rotator();
			// Z축 회전(Yaw)만 적용하여 캐릭터가 플레이어를 향하도록 설정
			FRotator NewRotation = FRotator(0.0f, LookAtRotation.Yaw, 0.0f);
			// 플레이어를 바라보게 고정함
			MyBoss->SetActorRotation(NewRotation);
		}

		//만약 이동한 위치랑 얼마 차이가 나지 않으면 다시 플레이어를 향해 움직임
		if (FVector::Dist(TargetSideLocation, MyBoss->GetActorLocation()) <= 150.0f)
		{
			//좌우 이동을 하기전 위치에서 거리값 계산을 막기위해 이동 후 위치를 마지막 위치로 설정
			LastLocation = MyBoss->GetActorLocation();

			//0으로 되돌려서 다시 플레이어를 향해 움직이게 만듦
			TotalMoveDistance = 0.0f;

			IsSideMoveSetting = false;

			MyBoss->bUseControllerRotationYaw = true;
			MyBoss->GetCharacterMovement()->bOrientRotationToMovement = true; 
		}
	}
}

void UCBossFSM::SpawnRangedActor()
{
	++RangedAttackCount;

	bool FindResult = false;

	FVector Direction = (MyBoss->Target->GetActorLocation() - MyBoss->GetActorLocation()).GetSafeNormal2D();

	//발사 위치의 Transform을 가져옴
	FTransform ThrowPos = MyBoss->ThrowPosition->GetComponentTransform();
	
	//스폰할 객체에 대한 스폰 옵션을 설정하는 구조체
	FActorSpawnParameters Params;
	//스폰 과정에 충돌이 생겨도 제자리에서 스폰할 수 있게 만듦
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	if (RangedAttackCount == 1)
	{
		//원거리 공격 오브젝트를 월드에 소환
		ACRangeAttack* SpawnRanged = GetWorld()->SpawnActor<ACRangeAttack>(MyBoss->RangedAttackFactory1, MyBoss->ThrowPosition->GetComponentTransform(), Params);
		SpawnRanged->SetDirectionAndBoss(Direction, MyBoss);
	}

	else if (RangedAttackCount == 2)
	{
		//원거리 공격 오브젝트를 월드에 소환
		ACRangeAttack* SpawnRanged = GetWorld()->SpawnActor<ACRangeAttack>(MyBoss->RangedAttackFactory2, MyBoss->ThrowPosition->GetComponentTransform(), Params);
		SpawnRanged->SetDirectionAndBoss(Direction, MyBoss);
	}

	//원거리 엑터 소환을 2번했으면 더 이상 못하도록 초기화
	if (RangedAttackCount == 2)
	{
		RangedAttackCount = 0;
	}

// 	//오브젝트 풀 리스트를 검사하여 비활성화 된 액터를 찾음
// 	for ( int32 i = 0; i < MyBoss->RangedAttackList.Num(); ++i )
// 	{
// 		//비활성화 된 액터를 찾으면
// 		if ( !MyBoss->RangedAttackList[i]->MeshComp->IsVisible() )
// 		{
// 			FindResult = true;
// 
// 			//활성화를 해주고 방향을 전달
// 			MyBoss->RangedAttackList[i]->SetActive(true, Direction);
// 			//던져지기 시작하는 위치에 배치
// 			MyBoss->RangedAttackList[i]->SetActorTransform(ThrowPos);
// 
// 			//반복 그만하기
// 			break;
// 		}
// 	}
}

void UCBossFSM::DASHATTACKState()
{
	//AI 움직임 멈춤
	AI->StopMovement();

	//만약 대쉬 공격 애니메이션이 재생중이지 않으면 재생하게 만듦
	if ( !MyBoss->AnimInstance->Montage_IsPlaying ( MyBoss->AM_DashAttack ) )
	{
		MyBoss->AnimInstance->Montage_Play ( MyBoss->AM_DashAttack );
	}

	//위치 셋팅이 되어있는 상태이면
	if ( IsSetDashAttackLocation )
	{
// 		if ( IsReadyDashAttack )
// 		{
			//돌진 시간을 업데이트
			DashTimer += GetWorld ( )->GetDeltaSeconds ( ) / DashDuration;

			//만약 시간이 정해진 시간 보다 커지거나, 사거리 안에 들어왔을 경우
			if ( DashTimer >= 1.0f )
			{
				//돌진은 완료했다고 bool값 false로 전환
				IsSetDashAttackLocation = false;
				//Navmesh를 이용하여 다시 이동하게 만듦
				MyBoss->GetCharacterMovement ( )->SetMovementMode ( MOVE_Walking );
				//이동 속도도 다시 원래 추적 속도로 되돌려줌
				MyBoss->GetCharacterMovement ( )->MaxWalkSpeed = 200.0f;
				//추적 시간도 초기화
				CurChaseTime = 0.0f;
				//대쉬 공격 준비 초기화
				IsReadyDashAttack = false;
				//불 초기화
				IsLowDist = false;
				//좌우 이동을 하기전 위치에서 거리값 계산을 막기위해 이동 후 위치를 마지막 위치로 설정
				LastLocation = MyBoss->GetActorLocation();
				//리턴
				return;
			}

			//easeinsine이 적용되도록 계산하게 만듦
			float t = EaseInSine ( DashTimer );
			//Lerp를 사용하여 사이를 보간
			FVector NewLocation = FMath::Lerp ( StartDashLocation , CalculatedTargetLocation , t );
			//Z축 이동 못하게 만듦
			NewLocation.Z = StartDashLocation.Z;
			//이동하게 함
			MyBoss->SetActorLocation ( NewLocation );
		/*}*/	
	}

	//위치 세팅이 안되어있는 상태이면
	else if ( !IsSetDashAttackLocation )
	{
		
	}
}

void UCBossFSM::COMBOATTACKState()
{
	//콤보 공격 실행 중이 아니라면 일단플레이어한테 다가감
	if (!MyBoss->AnimInstance->Montage_IsPlaying(MyBoss->AM_ComboAttack_01) || !MyBoss->AnimInstance->Montage_IsPlaying(MyBoss->AM_ComboAttack_02))
	{
		if (TargetDist >= 150.0f)
		{
			AI->MoveToLocation(MyBoss->Target->GetActorLocation());
		}

		else
		{
			//랜덤값에 따라 콤보 공격이 다르게 나옴
			switch (ComboAttackIndex)
			{
			case 0:
				//콤보 공격이 실행중이지 않을경우 콤보공격 실행
				if (!MyBoss->AnimInstance->Montage_IsPlaying(MyBoss->AM_ComboAttack_01))
				{
					MyBoss->AnimInstance->Montage_Play(MyBoss->AM_ComboAttack_01);

					//시간 초기화
					CurComboAttackTime = 0.0f;
				}

				break;

			case 1:
				//콤보 공격이 실행중이지 않을경우 콤보공격 실행
				if (!MyBoss->AnimInstance->Montage_IsPlaying(MyBoss->AM_ComboAttack_02))
				{
					MyBoss->AnimInstance->Montage_Play(MyBoss->AM_ComboAttack_02);

					//시간 초기화
					CurComboAttackTime = 0.0f;
				}

				break;

			default: break;
			}
		}
	}

// 	//랜덤값에 따라 콤보 공격이 다르게 나옴
// 	switch ( ComboAttackIndex )
// 	{
// 		case 0:
// 			//콤보 공격이 실행중이지 않을경우 콤보공격 실행
// 			if ( !MyBoss->AnimInstance->Montage_IsPlaying ( MyBoss->AM_ComboAttack_01 ) )
// 			{
// 				MyBoss->AnimInstance->Montage_Play ( MyBoss->AM_ComboAttack_01 );
// 
// 				//시간 초기화
// 				CurComboAttackTime = 0.0f;
// 			}
// 
// 		break;
// 		
// 		case 1:
// 			//콤보 공격이 실행중이지 않을경우 콤보공격 실행
// 			if ( !MyBoss->AnimInstance->Montage_IsPlaying ( MyBoss->AM_ComboAttack_02 ) )
// 			{
// 				MyBoss->AnimInstance->Montage_Play ( MyBoss->AM_ComboAttack_02 );
// 
// 				//시간 초기화
// 				CurComboAttackTime = 0.0f;
// 			}
// 
// 		break;
// 		
// 		default: break;
// 	}
}

void UCBossFSM::COUNTERATTACKState()
{
	//가드를 할 것이기에 이동은 멈춤
	AI->StopMovement();

	//먼저 가드 애니메이션이 나오게 만듦
	if ( !MyBoss->AnimInstance->Montage_IsPlaying ( MyBoss->AM_Guard ) )
	{
		MyBoss->AnimInstance->Montage_Play ( MyBoss->AM_Guard );
	}

	//가드 애니메이션이 실행중일때만 캐릭터를 바라보게 회전함
	if ( MyBoss->AnimInstance->Montage_IsPlaying ( MyBoss->AM_Guard ) )
	{
		//가드 상태일때는 플레이어를 바라보도록 만듦
		// 플레이어 방향 계산
		FVector PlayerLocation = MyBoss->Target->GetActorLocation();
		FVector PawnLocation = MyBoss->GetActorLocation();
		FVector DirectionToPlayer = (PlayerLocation - PawnLocation).GetSafeNormal   ();

		// Yaw 회전만 조정 (Pitch는 유지)
		FRotator TargetRotation = FRotationMatrix::MakeFromX	(DirectionToPlayer).Rotator();
		TargetRotation.Pitch = 0.0f; // Pitch를 0으로 설정해 수평 회전만 적용
		TargetRotation.Roll = 0.0f;

		// 부드러운 회전을 위해 Interp 사용 (선택 사항)
		FRotator CurrentRotation = MyBoss->GetActorRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation,	GetWorld()->GetDeltaSeconds(), 0.5f);

		// Pawn 회전 설정
		MyBoss->SetActorRotation (NewRotation);
	}

	

// 	//공격이 현재 에너미가 바라보는 방향에서 어느 각도로 맞았는지 체크하게 만듦
// 	//SphereTrace와 내적을 사용해서 플레이어의 공격 각도를 체크하는 방식
// 	FHitResult Hit;
// 	FCollisionQueryParams Params;
// 	Params.AddIgnoredActor(MyBoss);
// 	bool bHit = GetWorld()->SweepSingleByChannel(Hit, MyBoss->GetActorLocation(), MyBoss->GetActorLocation(), FQuat::Identity, ECC_GameTraceChannel3 , FCollisionShape::MakeSphere(130.0f), Params);
// 
// 	DrawDebugSphere ( GetWorld ( ) , MyBoss->GetActorLocation ( ) , 130.0f , 21 , FColor::Green , false , 0.1f );
// 
// 	//일단 닿은게 플레이어라면 
// 	if ( bHit && Hit.GetActor()->IsA(ACPlayer::StaticClass()) )
// 	{
// 		//플레이어와의 각도를 계산함
// 		FVector ToPlayer = (Hit.GetActor()->GetActorLocation() - MyBoss->GetActorLocation()).GetSafeNormal2D();
// 		
// 		float DotProduct = FVector::DotProduct( MyBoss->GetActorForwardVector(), ToPlayer);
// 		float AngleRad = FMath::Acos(DotProduct);
// 		float AngleDeg = FMath::RadiansToDegrees(AngleRad);
// 
// 		//만약 각도가 전방 30도 사이라면, 정면에서 맞았을 경우
// 		if ( AngleDeg <= 30.0f )
// 		{
// 			//바로 카운터 공격이 동작하게 만듦
// 			GEngine->AddOnScreenDebugMessage ( 111 , 1.0f , FColor::White , TEXT ( "Counter Attack!!!" ) );
// 			//혹시 모르니 데미지 처리 못하게 bool을 true로 변경해줌
// 			MyBoss->IsGuardSucssess = true;
// 			//공격 상태 NONE으로 변경
// 			AttackState = EBossATTACKState::NONE;
// 			//시간 초기화
// 			CurGuardTime = 0.0f;
// 			//가드 조건 초기화
// 			MyBoss->GuardGage = 0.0f;
// 			//뒤에 코드 작동이 안되게 리턴
// 			return;
// 		}
// 
// 		//만약 각도가 100보다 크다면, 즉 후방 80도 사이에서 맞았을 경우
// 		else if ( AngleDeg >= 100.0f )
// 		{
// 			GEngine->AddOnScreenDebugMessage ( 112 , 1.0f , FColor::White , TEXT ( "Back Attack!!!" ) );
// 			//자세가 흐트러지며 BREAK상태가 됨
// 			AttackState = EBossATTACKState::NONE;
// 			State = EBossState::BREAK;
// 			//시간 초기화
// 			CurGuardTime = 0.0f;
// 			//가드 조건 초기화
// 			MyBoss->GuardGage = 0.0f;
// 			//뒤에 코드 작동이 안되게 리턴
// 			return;
// 		}
// 	}
// 
// 	//만약 그냥 가드 시간이 끝났을 경우
// 	if ( CurGuardTime >= LimiteGuardTime )
// 	{
// 		GEngine->AddOnScreenDebugMessage ( 113 , 1.0f , FColor::White , TEXT ( "Guard Time Limite" ) );
// 		//그냥 공격상태를 NONE상태로 되돌림
// 		AttackState = EBossATTACKState::NONE;
// 		//시간 초기화
// 		CurGuardTime = 0.0f;
// 		//가드 조건 초기화
// 		MyBoss->GuardGage = 0.0f;
// 	}
}

void UCBossFSM::SPATTACKState()
{
	//필살기 공격을 할 것이기에 이동은 멈춤
	AI->StopMovement();

	//필살기 상태가 되면 애니메이션 재생
	if ( !MyBoss->AnimInstance->Montage_IsPlaying ( MyBoss->AM_SPAttack ) )
	{
		MyBoss->AnimInstance->Montage_Play(MyBoss->AM_SPAttack);
	}

	//필살기 달리기 상태면 달리는거 따로 동작하게 만듦
	if (MyBoss->IsSPRun)
	{
		// 플레이어 위치
		FVector PlayerLocation = MyBoss->Target->GetActorLocation();
		// 현재 AI 위치
		FVector AILocation = MyBoss->GetActorLocation();
		// 플레이어를 향하는 방향 벡터 계산
		FVector DirectionToPlayer = (PlayerLocation - AILocation).GetSafeNormal();
		// 방향 벡터를 회전으로 변환 (Yaw만 고려)
		FRotator LookAtRotation = FRotationMatrix::MakeFromX(DirectionToPlayer).Rotator();
		// Z축 회전(Yaw)만 적용하여 캐릭터가 플레이어를 향하도록 설정
		FRotator NewRotation = FRotator(0.0f, LookAtRotation.Yaw, 0.0f);
		// 플레이어를 바라보게 고정함
		MyBoss->SetActorRotation(NewRotation);

		//플레이어를 향해서 움직이게 만듦
		AI->MoveToLocation(MyBoss->Target->GetActorLocation());

		if (TargetDist <= 200.0f)
		{
			//현재 재생중인 애니메이션 몽타주 확인
			UAnimMontage* NowMontage = MyBoss->AnimInstance->GetCurrentActiveMontage();
			if (!NowMontage)
			{
				return;
			}

			//현재 몽타주가 재생중인 섹션 확인
			FName NowSection = MyBoss->AnimInstance->Montage_GetCurrentSection(NowMontage);

			MyBoss->AnimInstance->Montage_JumpToSection(FName("SPFirstATK"), NowMontage);

			MyBoss->IsSPRun = false;

			MyBoss->bUseControllerRotationYaw = true;
			MyBoss->GetCharacterMovement()->bOrientRotationToMovement = true;

			return;
		}

		return;
	}

// 	CurSPReadyTime += GetWorld()->GetDeltaSeconds();
// 
// 	GEngine->AddOnScreenDebugMessage ( 90 , 1.0f , FColor::White , TEXT ( "SP Attack State!!" ) );
// 
// 	//먼저 일정 시간 동안은
// 	if ( CurSPReadyTime <= SPReadyTime )
// 	{
// 		//먼저 필살기 준비 자세를 취함
// 		GEngine->AddOnScreenDebugMessage ( 91 , 1.0f , FColor::White , TEXT ( "SPAttack Ready!!"));
// 	}
// 
// 	//필살기 패턴을 파훼 안하면 필살기 공격이 나감
// 	else
// 	{
// 		GEngine->AddOnScreenDebugMessage ( 91 , 1.0f , FColor::White , TEXT ( "SPAttack!!" ) );
// 
// 		//시간 초기화
// 		CurSPReadyTime = 0.0f;
// 
// 		//공격 상태를 NONE으로 되돌려줌
// 		AttackState = EBossATTACKState::NONE;
// 	}
}

void UCBossFSM::SETATKNONEState()
{
	// 플레이어를 향하는 방향 벡터 계산
	FVector DirectionToPlayer = (MyBoss->Target->GetActorLocation() - MyBoss->GetActorLocation()).GetSafeNormal();
	// 방향 벡터를 회전으로 변환 (Yaw만 고려)
	FRotator LookAtRotation = FRotationMatrix::MakeFromX(DirectionToPlayer).Rotator();
	// Z축 회전(Yaw)만 적용하여 캐릭터가 플레이어를 향하도록 설정
	FRotator NewRotation = FRotator(0.0f, LookAtRotation.Yaw, 0.0f);
	// 플레이어를 바라보게 고정함
	MyBoss->SetActorRotation(NewRotation);
}

void UCBossFSM::BACKSTEPState()
{
	//플레이어와 거리가 가까워지면 뒤로 물러나게 만듦
// 	if (TargetDist <= 300.0f /*&& TargetDist >= 150.0f*/)
// 	{
		//이동 값을 억지로 -값으로 변경
		if (!Cast<UCBossAnim>(MyBoss->AnimInstance)->IsBacksteping)
		{
			Cast<UCBossAnim>(MyBoss->AnimInstance)->IsBacksteping = true;
		}

		//현재 에너미와 플레이어 간에 방향을 구함
		FVector PlayerLocation = MyBoss->Target->GetActorLocation();
		FVector PawnLocation = MyBoss->GetActorLocation();
		FVector ToPlayerDirection = (PlayerLocation - PawnLocation).GetSafeNormal();

		// 플레이어로부터 멀어지는 방향 계산
		FVector DirectionAwayFromPlayer = (PawnLocation - PlayerLocation).GetSafeNormal();
		FVector RetreatLocation = MyBoss->GetActorLocation() + DirectionAwayFromPlayer * 200.0f; // 200 유닛 후퇴

		// NavMesh 상의 유효한 위치로 이동
		FNavLocation ProjectedLocation;
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		if (NavSystem && NavSystem->ProjectPointToNavigation(RetreatLocation, ProjectedLocation))
		{
			AI->MoveToLocation(ProjectedLocation.Location, 150.0f);
		}

		// 		// 이동할 거리
		// 		float BackstepDistance = 300.0f;
		// 
		// 		// Pawn의 앞 방향 벡터를 기준으로 좌/우 방향 계산
		// 		FVector BackVector = MyBoss->GetActorForwardVector() * ToPlayerDirection * BackstepDistance;
		// 
		// 		// 목표 위치 계산
		// 		FVector BackstepLocation = MyBoss->GetActorLocation() + BackVector;
		// 
		// 		//구한 전방 방향을 사용해서 뒤로 해당 구간동안 뒤로 물러나게 만듦
		// /*		MyBoss->SetActorLocation(MyBoss->GetActorLocation() + -ToPlayerDirection * 400.0f * GetWorld()->GetDeltaSeconds());*/
		// 
		// 		AI->MoveToLocation(BackstepLocation);

				// 플레이어를 향하는 방향 벡터 계산
		FVector DirectionToPlayer = (PlayerLocation - PawnLocation).GetSafeNormal();
		// 방향 벡터를 회전으로 변환 (Yaw만 고려)
		FRotator LookAtRotation = FRotationMatrix::MakeFromX(DirectionToPlayer).Rotator();
		// Z축 회전(Yaw)만 적용하여 캐릭터가 플레이어를 향하도록 설정
		FRotator NewRotation = FRotator(0.0f, LookAtRotation.Yaw, 0.0f);
		// 플레이어를 바라보게 고정함
		MyBoss->SetActorRotation(NewRotation);
/*	}*/

	if(TargetDist >= 550.0f)
	{
		if (Cast<UCBossAnim>(MyBoss->AnimInstance)->IsBacksteping)
		{
			Cast<UCBossAnim>(MyBoss->AnimInstance)->IsBacksteping = false;

			SetATKState = ESetATKState::SIDEMOVE;
			//타이머 써서 일정 시간 지나면 NONE으로 바뀌게
			
			GetWorld()->GetTimerManager().SetTimer(SetSIDEMOVEStateTimerHandle, this, &UCBossFSM::SetSIDEMOVEState, 0.2f, false);

			//AttackState = EBossATTACKState::NONE;
		}
	}
}

void UCBossFSM::SIDEMOVEState()
{
	if (!IsSideMoveSetting)
	{
		MyBoss->bUseControllerRotationYaw = false; // AI 컨트롤러 회전 비활성화
		MyBoss->GetCharacterMovement()->bOrientRotationToMovement = false; // 이동 방향 회전 비활성화

		// 현재 Pawn의 위치
		FVector CurrentLocation = MyBoss->GetActorLocation();

		// 랜덤으로 좌(-1) 또는 우(1) 방향 선택
		float Direction = FMath::RandBool() ? 1.0f : -1.0f;

		if (Direction >= 0.1f)
		{
			Cast<UCBossAnim>(MyBoss->AnimInstance)->MoveDirection = 100;
		}

		else if (Direction <= -0.1f)
		{
			Cast<UCBossAnim>(MyBoss->AnimInstance)->MoveDirection = -100;
		}

		// 이동할 거리
		float SideDistance = 550.0f;

		// Pawn의 오른쪽 방향 벡터를 기준으로 좌/우 방향 계산
		FVector SideVector = MyBoss->GetActorRightVector() * Direction * SideDistance;

		// 목표 위치 계산
		TargetSideLocation = CurrentLocation + SideVector;

		// 네비게이션 시스템을 통해 유효한 위치로 보정
		FNavLocation NavLocation;
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		if (NavSystem && NavSystem->ProjectPointToNavigation(TargetSideLocation, NavLocation))
		{
			TargetSideLocation = NavLocation.Location;

			//좌우 이동 셋팅이 끝났음을 알림
			IsSideMoveSetting = true;
		}

		else
		{
			//좌우 이동을 안하고 초기화
			TotalMoveDistance = 0.0f;

			IsSideMoveSetting = false;
		}
	}

	if (IsSideMoveSetting)
	{
		// 목표 위치로 이동
		AI->MoveToLocation(TargetSideLocation);

		// 		if (!MyBoss->bUseControllerRotationYaw)
		// 		{
		// 			MyBoss->bUseControllerRotationYaw = true;
		// 		}

				//회전 뒤집히는거 방지용 거리가 있을때만 플레이어 바라보게
		if (TargetDist >= 150.0f)
		{
			// 플레이어 위치
			FVector PlayerLocation = MyBoss->Target->GetActorLocation();
			// 현재 AI 위치
			FVector AILocation = MyBoss->GetActorLocation();
			// 플레이어를 향하는 방향 벡터 계산
			FVector DirectionToPlayer = (PlayerLocation - AILocation).GetSafeNormal();
			// 방향 벡터를 회전으로 변환 (Yaw만 고려)
			FRotator LookAtRotation = FRotationMatrix::MakeFromX(DirectionToPlayer).Rotator();
			// Z축 회전(Yaw)만 적용하여 캐릭터가 플레이어를 향하도록 설정
			FRotator NewRotation = FRotator(0.0f, LookAtRotation.Yaw, 0.0f);
			// 플레이어를 바라보게 고정함
			MyBoss->SetActorRotation(NewRotation);
		}

		//만약 이동한 위치랑 얼마 차이가 나지 않으면 다시 플레이어를 향해 움직임
		if (FVector::Dist(TargetSideLocation, MyBoss->GetActorLocation()) <= 150.0f)
		{
			//좌우 이동을 하기전 위치에서 거리값 계산을 막기위해 이동 후 위치를 마지막 위치로 설정
			LastLocation = MyBoss->GetActorLocation();

			//0으로 되돌려서 다시 플레이어를 향해 움직이게 만듦
			TotalMoveDistance = 0.0f;

			IsSideMoveSetting = false;

			MyBoss->bUseControllerRotationYaw = true;
			MyBoss->GetCharacterMovement()->bOrientRotationToMovement = true;

			SetATKState = ESetATKState::SETATKNONE;
			AttackState = EBossATTACKState::NONE;
			State = EBossState::ATTACK;
		}
	}
}

void UCBossFSM::SetSIDEMOVEState()
{
	SetATKState = ESetATKState::SIDEMOVE;
}

void UCBossFSM::SetSPDamage(float Damage)
{
	//받은 데미지를 넣음
	OnSpDamageAmount += Damage;

	//만약 데미지가 Break 조건 이상이 됐을 경우
	if ( OnSpDamageAmount >= SPBreakDamageAmount )
	{
		//필살기 패턴이 파훼됨
		GEngine->AddOnScreenDebugMessage ( 89 , 10.0f , FColor::Red , TEXT ( "SPAttack Break!!" ) );

		//필살기 상태가 되면 애니메이션 재생
		if ( MyBoss->AnimInstance->Montage_IsPlaying ( MyBoss->AM_SPAttack ) )
		{
			MyBoss->AnimInstance->Montage_Stop(0.0f, MyBoss->AM_SPAttack);
		}

		//공격상태는 NONE으로 변환
		AttackState = EBossATTACKState::NONE;
		//상태는 BREAK로 변환
		State = EBossState::BREAK;

		//시간 초기화
		CurSPReadyTime = 0.0f;
		//bool 초기화
		MyBoss->IsReadySPAttack = false;
	}
}

void UCBossFSM::SetCOMBOATTACKState(int32 RandomComboAttack)
{
	ComboAttackIndex = RandomComboAttack;

	AttackState = EBossATTACKState::COMBOATTACK;
}

void UCBossFSM::GetOwnerEnemy()
{
	//자신의 주인과 AI를 받음
	MyBoss = Cast<ACBossEnemy>(GetOwner());
	AI = Cast<AAIController>(MyBoss->GetController());

	//현재 자신이 있는 위치를 설정
	LastLocation = MyBoss->GetActorLocation();
}

void UCBossFSM::SetDashAttackLocation()
{
	DashTimer = 0.0f;
	StartDashLocation = MyBoss->GetActorLocation();

	//타겟의 위치를 설정
	FVector TargetLocation = MyBoss->Target->GetActorLocation();
	//뒤로 더 이동하기 위해 타겟의 방향을 계산
	FVector DirectionToTarget = (TargetLocation -StartDashLocation).GetSafeNormal();

	//플레이어 위치보다 더 뒤로 이동하게 값을 계산함
/*	CalculatedTargetLocation = TargetLocation + (DirectionToTarget * DashAttackOverDist);*/

	//플레이어 위치보다 약간 앞에서 멈추게 값을 계산함
	CalculatedTargetLocation = TargetLocation - (DirectionToTarget * DashAttackFrontDist);

	//타겟 위치를 목표로 설정
/*	CalculatedTargetLocation = TargetLocation;*/

	//Navmesh를 비활성화해줌
	MyBoss->GetCharacterMovement()->SetMovementMode(MOVE_Flying);

	// 타겟 방향 계산
	FVector BossLocation = MyBoss->GetActorLocation();
	FVector DirectionToPlayer = (CalculatedTargetLocation - BossLocation).GetSafeNormal();

	// Yaw 회전만 조정 (Pitch는 유지)
	FRotator TargetRotation = FRotationMatrix::MakeFromX(DirectionToPlayer).Rotator();
	TargetRotation.Pitch = 0.0f; // Pitch를 0으로 설정해 수평 회전만 적용
	TargetRotation.Roll = 0.0f;

	MyBoss->SetActorRotation(TargetRotation);

	//위치를 설정 
	IsSetDashAttackLocation = true;
}

float UCBossFSM::EaseInSine ( float x )
{
	return 1.0f - FMath::Cos ( ( x * PI ) / 2.0f );
}