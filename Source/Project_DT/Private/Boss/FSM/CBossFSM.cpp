// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss/FSM/CBossFSM.h"
#include "Boss/CBossEnemy.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Boss/RangedAttackObject/CRangeAttack.h"
#include "Character/CPlayer.h"

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
	CurIDLETime +=GetWorld()->GetDeltaSeconds();
	if ( CurIDLETime >= IDLETime )
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
	//브레이크 상태가 되면 브레이크의 지속시간을 체크함
	CurBreakTime +=GetWorld()->GetDeltaSeconds();

	//지속시간이 끝났다면
	if ( CurBreakTime >= BreakLimitTime )
	{
		//일어선 다음

		//CHASE상태로 변환? ATTACK상태로 변환? 나중에 정하기
		State = EBossState::ATTACK;
	}
}

void UCBossFSM::DIEState()
{

}

void UCBossFSM::NONEState()
{
	//공격 패턴 우선 순위
	//SPATTACK(필살기) > RANGEDATTACK(원거리 공격) > DASHATTACK(대쉬 공격)
	//COUNTERATTACK(가드 공격) > COMBOATTACK (콤보 공격)
	
	//플레이어를 향해서 움직이게 만듦
	AI->MoveToLocation(MyBoss->Target->GetActorLocation());

	//필살기 사용 부분
	//현재는 체력을 기준으로 필살기를 발동, 체력이 반 이하로 내려갔을 경우
	if ( MyBoss->CurHP <= MyBoss->MaxHP / 2 )
	{
		//필살기 상태로 전환
		AttackState = EBossATTACKState::SPATTACK;
		//뒤에 코드 실행 안되게 막음
		return;
	}
	//필살기 사용 부분

	//공격 NONE상태에서 거리를 체크해서 거리가 멀 경우
	if ( TargetDist >= LongDist )
	{
		//거리가 먼 상태가 얼마나 지속되었는지 체크한다
		CurChaseTime += GetWorld()->GetDeltaSeconds();

		//거리가 먼 상태인데 플레이어가 물약 마시는 모션을 할 경우
		if (/*임시로 다른 조건 넣었음*/ CurChaseTime >= 6.0f)
		{
			//임시로 넣은거 나중에 지워야함 제발!
			CurChaseTime = 0.0f;

			//바로 원거리 공격하게 변환
			AttackState = EBossATTACKState::RANGEDATTACK;
			//원거리 공격이 바로 실행되게 이곳에서 한번 실행
			SpawnRangedActor((MyBoss->Target->GetActorLocation() - MyBoss->GetActorLocation()).GetSafeNormal2D());
			//뒤에 코드 안불리게 리턴
			return;
		}
		//거리가 먼 상태인데 플레이어가 물약 마시는 모션을 할 경우

		//만약 거리가 먼 상태가 일정 시간 유지되었다면
		if ( CurChaseTime >= /*DashAttackCooltime*/ 15.0f )
		{
			//그리고 대쉬 공격이 이루어지도록 공격상태도 변환
			AttackState = EBossATTACKState::DASHATTACK;

			//빠르게 달리는 느낌을 주기 위하여 이동속도 바꾸기
			MyBoss->GetCharacterMovement()->MaxWalkSpeed = 1000.0f;

			//뒤에 코드 실행 안되게 리턴
			return;
		}
	}
	//공격 NONE상태에서 거리를 체크해서 거리가 멀 경우
	
	//공격 NONE상태에서 거리를 체크해서 거리가 가까울 경우
	if ( TargetDist <= MyBoss->AttackRange )
	{
		//가드 게이지를 채워줌
		MyBoss->GuardGage += GetWorld()->GetDeltaSeconds();
		//가드 게이지를 채워줌

		//만약 가드 조건이 충족되었을 경우
		if ( MyBoss->GuardGage <= MyBoss->GuardPlaying )
		{
			//가드 상태로 변화
			AttackState = EBossATTACKState::COUNTERATTACK;
			//초기화
			MyBoss->GuardGage = 0.0f;

			//밑에 코드 안 일어나게 리턴
			return;
		}

		//다른 조건이 다 충족되지 않았을 경우
		else
		{	
			//랜덤 공격이 실행
			SetCOMBOATTACKState(FMath::RandRange(0, 1));
		}
	}
}

void UCBossFSM::RANGEDATTACKState()
{
	//타겟과 자신의 방향을 체크
	FVector TargetLocation = MyBoss->Target->GetActorLocation();
	FVector MyLocation = MyBoss->GetActorLocation();
	FVector DirectionToTarget = (TargetLocation - MyLocation).GetSafeNormal2D();

	//해당 방향으로 원거리 공격 발사
	//애니메이션을 넣는다면 해당 애니메이션 노티파이로 원거리 공격 발사되게
	CurRandgedTime += GetWorld()->GetDeltaSeconds();
	
	//여기서 코드를 작성하면 Tick으로 여러번 불리기에 여기에 작성하면 안됨
	/*SpawnRangedActor(DirectionToTarget);*/

	if ( CurRandgedTime >= 2.5f )
	{
		SpawnRangedActor(DirectionToTarget);
		AttackState = EBossATTACKState::NONE;
		CurRandgedTime = 0.0f;
		return;
	}
	//애니메이션을 넣는다면 해당 애니메이션 노티파이로 원거리 공격 발사되게

	//그런 다음 공격 상태를 다시 NONE으로 변경
	/*AttackState = EBossATTACKState::NONE;*/
}

void UCBossFSM::SpawnRangedActor(FVector Direction)
{
	bool FindResult = false;

	//발사 위치의 Transform을 가져옴
	FTransform ThrowPos = MyBoss->ThrowPosition->GetComponentTransform();

	//오브젝트 풀 리스트를 검사하여 비활성화 된 액터를 찾음
	for ( int32 i = 0; i < MyBoss->RangedAttackList.Num(); ++i )
	{
		//비활성화 된 액터를 찾으면
		if ( !MyBoss->RangedAttackList[i]->MeshComp->IsVisible() )
		{
			FindResult = true;

			//활성화를 해주고 방향을 전달
			MyBoss->RangedAttackList[i]->SetActive(true, Direction);
			//던져지기 시작하는 위치에 배치
			MyBoss->RangedAttackList[i]->SetActorTransform(ThrowPos);

			//반복 그만하기
			break;
		}
	}
}

void UCBossFSM::DASHATTACKState()
{
	//위치 셋팅이 되어있는 상태이면
	if ( IsSetDashAttackLocation )
	{
		//돌진 시간을 업데이트
		DashTimer += GetWorld()->GetDeltaSeconds() / DashDuration;

		//만약 시간이 정해진 시간 보다 커진다면
		if ( DashTimer >= 1.0f)
		{
			//돌진은 완료했다고 bool값 false로 전환
			IsSetDashAttackLocation = false;
			//Navmesh를 이용하여 다시 이동하게 만듦
			MyBoss->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			//공격 상태를 다시 NONE으로 되돌림
			AttackState = EBossATTACKState::NONE;
			//이동 속도도 다시 원래 추적 속도로 되돌려줌
			MyBoss->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
			//추적 시간도 초기화
			CurChaseTime = 0.0f;
			//리턴
			return;
		}

		//easeinsine이 적용되도록 계산하게 만듦
		float t = EaseInSine(DashTimer);
		//Lerp를 사용하여 사이를 보간
		FVector NewLocation = FMath::Lerp(StartDashLocation, CalculatedTargetLocation, t);
		//이동하게 함
		MyBoss->SetActorLocation( NewLocation );
	}

	//위치 세팅이 안되어있는 상태이면
	else if ( !IsSetDashAttackLocation )
	{
		//위치를 설정 
		IsSetDashAttackLocation = true;
		DashTimer = 0.0f;
		StartDashLocation = MyBoss->GetActorLocation();

		//타겟의 위치를 설정
		FVector TargetLocation = MyBoss->Target->GetActorLocation();
		//뒤로 더 이동하기 위해 타겟의 방향을 계산
		FVector DirectionToTarget = (TargetLocation - StartDashLocation).GetSafeNormal();

		//플레이어 위치보다 더 뒤로 이동하게 값을 계산함
		CalculatedTargetLocation = TargetLocation + (DirectionToTarget * DashAttackOverDist);

		//Navmesh를 비활성화해줌
		MyBoss->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	}
}

void UCBossFSM::COMBOATTACKState()
{
	
}

void UCBossFSM::COUNTERATTACKState()
{
	//먼저 가드 애니메이션이 나오게 만듦

	//가드 상태 지속시간을 체크함
	CurGuardTime +=GetWorld()->GetDeltaSeconds();

	//가드 상태일때는 플레이어를 바라보도록 만듦
	// 플레이어 방향 계산
	FVector PlayerLocation = MyBoss->Target->GetActorLocation();
	FVector PawnLocation = MyBoss->GetActorLocation();
	FVector DirectionToPlayer = (PlayerLocation - PawnLocation).GetSafeNormal();

	// Yaw 회전만 조정 (Pitch는 유지)
	FRotator TargetRotation = FRotationMatrix::MakeFromX(DirectionToPlayer).Rotator();
	TargetRotation.Pitch = 0.0f; // Pitch를 0으로 설정해 수평 회전만 적용
	TargetRotation.Roll = 0.0f;

	// 부드러운 회전을 위해 Interp 사용 (선택 사항)
	FRotator CurrentRotation = MyBoss->GetActorRotation();
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 1.0f);

	// Pawn 회전 설정
	MyBoss->SetActorRotation (NewRotation);

	//공격이 현재 에너미가 바라보는 방향에서 어느 각도로 맞았는지 체크하게 만듦
	//SphereTrace와 내적을 사용해서 플레이어의 공격 각도를 체크하는 방식
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(MyBoss);
	bool bHit = GetWorld()->SweepSingleByChannel(Hit, MyBoss->GetActorLocation(), MyBoss->GetActorLocation(), FQuat::Identity, ECC_GameTraceChannel3 , FCollisionShape::MakeSphere(130.0f), Params);

	DrawDebugSphere ( GetWorld ( ) , MyBoss->GetActorLocation ( ) , 130.0f , 21 , FColor::Green , false , 0.1f );

	//일단 닿은게 플레이어라면 
	if ( bHit && Hit.GetActor()->IsA(ACPlayer::StaticClass()) )
	{
		//플레이어와의 각도를 계산함
		FVector ToPlayer = (Hit.GetActor()->GetActorLocation() - MyBoss->GetActorLocation()).GetSafeNormal2D();
		
		float DotProduct = FVector::DotProduct( MyBoss->GetActorForwardVector(), ToPlayer);
		float AngleRad = FMath::Acos(DotProduct);
		float AngleDeg = FMath::RadiansToDegrees(AngleRad);

		//만약 각도가 전방 30도 사이라면, 정면에서 맞았을 경우
		if ( AngleDeg <= 30.0f )
		{
			//바로 카운터 공격이 동작하게 만듦
			GEngine->AddOnScreenDebugMessage ( 111 , 1.0f , FColor::White , TEXT ( "Counter Attack!!!" ) );
			//혹시 모르니 데미지 처리 못하게 bool을 true로 변경해줌
			MyBoss->IsGuardSucssess = true;
			//공격 상태 NONE으로 변경
			AttackState = EBossATTACKState::NONE;
			//시간 초기화
			CurGuardTime = 0.0f;
			//가드 조건 초기화
			MyBoss->GuardGage = 0.0f;
			//뒤에 코드 작동이 안되게 리턴
			return;
		}

		//만약 각도가 100보다 크다면, 즉 후방 80도 사이에서 맞았을 경우
		else if ( AngleDeg >= 100.0f )
		{
			GEngine->AddOnScreenDebugMessage ( 112 , 1.0f , FColor::White , TEXT ( "Back Attack!!!" ) );
			//자세가 흐트러지며 BREAK상태가 됨
			AttackState = EBossATTACKState::NONE;
			State = EBossState::BREAK;
			//시간 초기화
			CurGuardTime = 0.0f;
			//가드 조건 초기화
			MyBoss->GuardGage = 0.0f;
			//뒤에 코드 작동이 안되게 리턴
			return;
		}
	}

	//만약 그냥 가드 시간이 끝났을 경우
	if ( CurGuardTime >= LimiteGuardTime )
	{
		GEngine->AddOnScreenDebugMessage ( 113 , 1.0f , FColor::White , TEXT ( "Guard Time Limite" ) );
		//그냥 공격상태를 NONE상태로 되돌림
		AttackState = EBossATTACKState::NONE;
		//시간 초기화
		CurGuardTime = 0.0f;
		//가드 조건 초기화
		MyBoss->GuardGage = 0.0f;
	}
}

void UCBossFSM::SPATTACKState()
{
	//필살기 상태가 되면

	CurSPReadyTime += GetWorld()->GetDeltaSeconds();

	GEngine->AddOnScreenDebugMessage ( 90 , 1.0f , FColor::White , TEXT ( "SP Attack State!!" ) );

	//먼저 일정 시간 동안은
	if ( CurSPReadyTime <= SPReadyTime )
	{
		//먼저 필살기 준비 자세를 취함
		GEngine->AddOnScreenDebugMessage ( 91 , 1.0f , FColor::White , TEXT ( "SPAttack Ready!!"));
	}

	//필살기 패턴을 파훼 안하면 필살기 공격이 나감
	else
	{
		GEngine->AddOnScreenDebugMessage ( 91 , 1.0f , FColor::White , TEXT ( "SPAttack!!" ) );

		//시간 초기화
		CurSPReadyTime = 0.0f;

		//공격 상태를 NONE으로 되돌려줌
		AttackState = EBossATTACKState::NONE;
	}
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

		//공격상태는 NONE으로 변환
		AttackState = EBossATTACKState::NONE;
		//상태는 BREAK로 변환
		State = EBossState::BREAK;

		//시간 초기화
		CurSPReadyTime = 0.0f;
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
}

float UCBossFSM::EaseInSine ( float x )
{
	return 1.0f - FMath::Cos ( ( x * PI ) / 2.0f );
}