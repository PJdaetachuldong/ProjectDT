// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/CBossEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Boss/FSM/CBossFSM.h"
#include "Weapons/CAttachment.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/RangedAttackObject/CRangeAttack.h"

ACBossEnemy::ACBossEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(FName("TestEnemy"));
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACBossEnemy::EnemyHitDamage);

	FSMComponent = CreateDefaultSubobject<UCBossFSM>(TEXT("FSMComponent"));

	//일단 임시로 하는 발사 위치 설정
	ThrowPosition = CreateDefaultSubobject<USceneComponent>(L"ThrowPosition");
	ThrowPosition->SetupAttachment(RootComponent);
	//일단 임시로 하는 발사 위치 설정
}

void ACBossEnemy::BeginPlay()
{
	Super::BeginPlay();

	Target = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	//원거리 공격을 최대 설정 값만큼 미리 생성함
	for ( int32 i = 0; i < MaxRangedAttackCount; ++i )
	{
		//스폰할 객체에 대한 스폰 옵션을 설정하는 구조체
		FActorSpawnParameters Params;
		//스폰 과정에 충돌이 생겨도 제자리에서 스폰할 수 있게 만듦
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//원거리 공격 오브젝트를 월드에 소환
		ACRangeAttack* RangedAttackObject = GetWorld()->SpawnActor<ACRangeAttack>(RangedAttackFactory, Params);
		//해당 원거리 공격 오브젝트 비활성화 처리
		RangedAttackObject->SetActive(false, FVector(0));
		//생성한 것을 오브젝트 풀에 넣음
		RangedAttackList.Add(RangedAttackObject);
	}
}

void ACBossEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CurTestSPTime += DeltaTime;

	//임의로 하는 필살기 패턴 테스트
// 	if ( CurTestSPTime >= TestSPTime )
// 	{
// 		FSMComponent->AttackState = EBossATTACKState::SPATTACK;
// 		CurTestSPTime =0.0f;
// 	}
	//임의로 하는 필살기 패턴 테스트
}

void ACBossEnemy::SPBreak()
{
	
}

void ACBossEnemy::EnemyHitDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//스페셜 공격때 데미지량 저장하는 코드를 적어야함

	ACAttachment* Weapon = Cast<ACAttachment> ( OtherActor );

	if ( Weapon )
	{
		GEngine->AddOnScreenDebugMessage ( 80 , 1.0f , FColor::Red , TEXT ( "Hit Boss" ) );

		//현재 필살기를 공격하려고 준비중이라면
		if ( FSMComponent->AttackState == EBossATTACKState::SPATTACK )
		{
			//데미지를 저장
			FSMComponent->SetSPDamage(10.0f);
		}

		//공격을 맞았을 때 쉴드 게이지가 있다면
		if ( ShieldAmount > 0.0f )
		{
			//쉴드게이지가 감소하도록 설정
			ShieldAmount -= /*Damage*/10.0f;

			//만약 쉴드게이지 감소되어서 0이 된다면
			if ( ShieldAmount <= 0.0f )
			{
				//에너미가 휘청거리는 애니메이션 출력? / 단 필살기 준비중에는 일어나지 않게 / 브레이크 상태에서도 나오지 않게
				if ( FSMComponent->AttackState != EBossATTACKState::SPATTACK || FSMComponent->State != EBossState::BREAK )
				{
					GEngine->AddOnScreenDebugMessage ( 81 , 1.0f , FColor::Red , TEXT ( "Boss Shield Break!!" ) );
				}

				//쉴드 게이지가 -의 값이 되면 해당 값 만큼 체력을 깎게 만듦
				CurHP += ShieldAmount;

				return;
			}

			//가드 애니메이션이 나오도록 만들어주기 / 단 필살기 준비중에는 일어나지 않게 / 브레이크 상태에서도 나오지 않게
			if ( FSMComponent->AttackState != EBossATTACKState::SPATTACK || FSMComponent->State != EBossState::BREAK )
			{
				GEngine->AddOnScreenDebugMessage ( 82 , 1.0f , FColor::Red , TEXT ( "Boss Gard Animation" ) );
			}
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

				GEngine->AddOnScreenDebugMessage ( 83 , 1.0f , FColor::Red , TEXT ( "Boss is Dead" ) );
			}

			//피격 애니메이션이 나오게 만듦 / 단 필살기 준비중에는 일어나지 않게 / 브레이크 상태에서도 나오지 않게
			if ( FSMComponent->AttackState != EBossATTACKState::SPATTACK || FSMComponent->State != EBossState::BREAK )
			{
				GEngine->AddOnScreenDebugMessage ( 84 , 1.0f , FColor::Red , TEXT ( "Boss Hit Damage" ) );
			}
		}
	}
}
