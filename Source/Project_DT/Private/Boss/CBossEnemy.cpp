// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/CBossEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Boss/FSM/CBossFSM.h"
#include "Weapons/CAttachment.h"

ACBossEnemy::ACBossEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(FName("TestEnemy"));
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACBossEnemy::EnemyHitDamage);

	FSMComponent = CreateDefaultSubobject<UCBossFSM>(TEXT("FSMComponent"));


}

void ACBossEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ACBossEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//추적 상태에서 거리를 체크해서 거리가 멀 경우
	TargetDist = FVector::Dist(Target->GetActorLocation(), GetActorLocation());
	
	if (FSMComponent->State == EBossState:: CHASE)
	{
		//거리를 체크해서 거리가 멀 경우
		if ( TargetDist >= LongDist )
		{		
			//거리가 먼 상태가 얼마나 지속되었는지 체크한다
			CurChaseTime += DeltaTime;
		}

		//만약 거리가 먼 상태가 일정 시간 유지되었다면
		if ( CurChaseTime >= DashAttackCooltime )
		{
			//공격 상태로 변환
			FSMComponent->State=EBossState::ATTACK;
			//그리고 대쉬 공격이 이루어지도록 공격상태도 변환
			FSMComponent->AttackState = EBossATTACKState::DASHATTACK;

			//뒤에 코드 실행 안되게 리턴
			return;
		}

		//그냥 추적 상태인데 플레이어가 물약 마시는 모션을 할 경우
// 		if ()
// 		{
// 			//공격 상태로 변환하고
// 			FSMComponent->State == EBossState::ATTACK;
// 			//바로 원거리 공격하게 변환
// 			FSMComponent->AttackState == EBossATTACKState::RANGEDATTACK;
// 		}
		//그냥 추적 상태인데 플레이어가 물약 마시는 모션을 할 경우
	}
	//추적 상태에서 거리를 체크해서 거리가 멀 경우

	//만약 공격 상태이면서 아직 어떤 공격을 할지 정하지 않았을 경우
	if(FSMComponent->State == EBossState::ATTACK && FSMComponent->AttackState == EBossATTACKState::NONE)
	{
		//가드 게이지를 채워줌
		GuardGage += DeltaTime;
		//가드 게이지를 채워줌

		//만약 가드 조건이 충족되었을 경우
		if ( GuardGage <= GuardPlaying )
		{
			//가드 상태로 변화
			FSMComponent->AttackState = EBossATTACKState::COUNTERATTACK;
			//초기화
			GuardGage = 0.0f;

			//밑에 코드 안 일어나게 리턴
			return;
		}

		//플레이어가 공격범위 이내일경우
		if ( TargetDist <= AttackRange )
		{
			FSMComponent->SetRANGEDATTACKState(FMath::RandRange(0,2));
		}
	}
	//만약 공격 상태이면서 아직 어떤 공격을 할지 정하지 않았을 경우

	//스페셜 공격에 들어갔을 경우
	if ( FSMComponent->AttackState == EBossATTACKState::SPATTACK )
	{
		
	}
}

void ACBossEnemy::SPBreak()
{

}

void ACBossEnemy::EnemyHitDamage ( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{
	//스페셜 공격때 데미지량 저장하는 코드를 적어야함

	ACAttachment* Weapon = Cast<ACAttachment> ( OtherActor );

	if ( Weapon )
	{
		GEngine->AddOnScreenDebugMessage ( 40 , 1.0f , FColor::Red , TEXT ( "Hit Enemy" ) );

		//공격을 맞았을 때 쉴드 게이지가 있다면
		if ( ShieldAmount > 0.0f )
		{
			//쉴드게이지가 감소하도록 설정
			ShieldAmount -= /*Damage*/10.0f;

			//만약 쉴드게이지 감소되어서 0이 된다면
			if ( ShieldAmount <= 0.0f )
			{
				//에너미가 휘청거리는 애니메이션 출력?
				GEngine->AddOnScreenDebugMessage ( 41 , 1.0f , FColor::Red , TEXT ( "Shield Break!!" ) );

				//쉴드 게이지가 -의 값이 되면 해당 값 만큼 체력을 깎게 만듦
				CurHP += ShieldAmount;

				return;
			}

			//가드 애니메이션이 나오도록 만들어주기
			GEngine->AddOnScreenDebugMessage ( 42 , 1.0f , FColor::Red , TEXT ( "Gard Animation" ) );
		}

		//쉴드 게이지가 없는 상태에서 맞았을 경우
		else
		{
			//체력을 깎게 만들어줌
			CurHP -= /*Damage*/10.0f;

			//체력이 깎였는데 0 이하가 된다면
			if ( CurHP <= 0.0f )
			{
				//사망 상태로 만듦
				FSMComponent->State = EBossState::DIE;

				//만약 에너미 매니저가 있다면
				if ( Manager )
				{
					Manager->RemoveEnemiesList ( MyUniqeID , IsCanAttack );
				}

				GEngine->AddOnScreenDebugMessage ( 43 , 1.0f , FColor::Red , TEXT ( "Enemy is Dead" ) );
			}

			//피격 애니메이션이 나오게 만듦
			GEngine->AddOnScreenDebugMessage ( 44 , 1.0f , FColor::Red , TEXT ( "Enemy Hit Damage" ) );
		}
	}
}
