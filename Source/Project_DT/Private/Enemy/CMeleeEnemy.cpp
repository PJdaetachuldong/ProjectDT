// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/CMeleeEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy/FSM/CMeleeEnemyFSM.h"
#include "Character/CPlayer.h"

// Sets default values
ACMeleeEnemy::ACMeleeEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ChaseDistance = 200.0f;
	WanderSpeed = 450.0f;
	WanderRadius = 500.0f;
	BackStepSpeed = 50.0f;
	
	GetCharacterMovement ( )->bUseRVOAvoidance = true;
	GetCharacterMovement ( )->AvoidanceConsiderationRadius = 200.0f;
	GetCharacterMovement ( )->AvoidanceWeight = 0.2f;
	GetCharacterMovement ( )->SetAvoidanceGroup ( 1 );
	GetCharacterMovement ( )->SetGroupsToAvoid ( 1 );
}

// Called when the game starts or when spawned
void ACMeleeEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACMeleeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckPlayerInRange();

	if (FSMComponent->State == EMeleeEnemyState::CHASE && Target)
	{
		float Distance = FVector::Dist(GetActorLocation(), Target->GetActorLocation());
		if (Distance <= ChaseDistance)
		{
			//공격권이 있으면 ATTACK상태로 변환
			if (IsCanAttack)
			{
				FSMComponent->State = EMeleeEnemyState::ATTACK;
			}
			//공격권이 없으면 WANDER상태로 변환
			else
			{
				FSMComponent->State = EMeleeEnemyState::WANDER;
			}
		}
	}
}

void ACMeleeEnemy::LoadStatsFromAsset()
{
	Super::LoadStatsFromAsset();

	if ( StatsAsset )
	{
		ChaseDistance = StatsAsset->Stats.AttackRange;
	}
}

void ACMeleeEnemy::CheckPlayerInRange()
{
	//SphereTrace와 내적을 사용해서 플레이어를 감지하는 방식
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool bHit = GetWorld()->SweepSingleByChannel(Hit,GetActorLocation(), GetActorLocation(), FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(500.0f), Params);

	DrawDebugSphere(GetWorld(), GetActorLocation(), 500.0f, 21, FColor::Green, false, 0.1f);
	
	if (bHit && Hit.GetActor()->IsA(ACPlayer::StaticClass()))
	{
		FVector ToPlayer = (Hit.GetActor()->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
		
		float DotProduct = FVector::DotProduct(GetActorForwardVector(), ToPlayer);
		float AngleRad = FMath::Acos(DotProduct);
		float AngleDeg = FMath::RadiansToDegrees(AngleRad);

		if (AngleDeg <= 40.0f)
		{
			//플레이어 탐지하는데 장애물이 있는지 확인함
			FHitResult LOSHit;
			bool bLOS = GetWorld()->LineTraceSingleByChannel(LOSHit, GetActorLocation(), Hit.GetActor()->GetActorLocation(), ECC_Visibility, Params);

			if (bLOS && LOSHit.GetActor()->IsA(ACPlayer::StaticClass()))
			{
				//타겟 저장
				Target = Cast<ACharacter>(LOSHit.GetActor());
				IsCanAttack = true;
				FSMComponent-> State = EMeleeEnemyState::CHASE;

				if ( Manager )
				{
					//매니저에게 플레이어를 발견했다고 전달, 다른 에너미도 추적하도록 만듦
					Manager->SendPlayerEncounter(Target, MyUniqeID);
				}
			}
		}
	}
}

void ACMeleeEnemy::SetStateCHASE(ACharacter* Player)
{
	Target = Player;
	FSMComponent->State = EMeleeEnemyState::CHASE;
}

void ACMeleeEnemy::EnemyHitDamage(float Damage)
{
	//공격을 맞았을 때 쉴드 게이지가 있다면
	if ( ShieldAmount > 0.0f )
	{
		//쉴드게이지가 감소하도록 설정
		ShieldAmount -= Damage;

		//만약 쉴드게이지 감소되어서 0이 된다면
		if ( ShieldAmount <= 0.0f )
		{
			//에너미가 휘청거리는 애니메이션 출력?
			GEngine->AddOnScreenDebugMessage ( 40 , 1.0f , FColor::Red , TEXT ( "Shield Break!!" ) );

			//쉴드 게이지가 -의 값이 되면 해당 값 만큼 체력을 깎게 만듦
			CurHP += ShieldAmount;

			return;
		}

		//가드 애니메이션이 나오도록 만들어주기
		GEngine->AddOnScreenDebugMessage ( 40 , 1.0f , FColor::Red , TEXT ( "Gard Animation" ) );
	}

	//쉴드 게이지가 없는 상태에서 맞았을 경우
	else
	{
		//체력을 깎게 만들어줌
		CurHP -= Damage;

		//체력이 깎였는데 0 이하가 된다면
		if ( CurHP <= 0.0f )
		{
			//사망 상태로 만듦
			FSMComponent->State = EMeleeEnemyState::DIE;

			//만약 에너미 매니저가 있다면
			if ( Manager )
			{
				Manager->RemoveEnemiesList(MyUniqeID, IsCanAttack);
			}

			GEngine->AddOnScreenDebugMessage ( 40 , 1.0f , FColor::Red , TEXT ( "Enemy is Dead" ) );
		}

		//피격 애니메이션이 나오게 만듦
		GEngine->AddOnScreenDebugMessage ( 40 , 1.0f , FColor::Red , TEXT ( "Enemy Hit Damage" ) );
	}
}
