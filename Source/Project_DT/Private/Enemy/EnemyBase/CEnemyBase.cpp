// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBase/CEnemyBase.h"
#include "Enemy/FSM/CMeleeEnemyFSM.h"
#include "Global.h"
// Sets default values
ACEnemyBase::ACEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//기본 값 설정
	MaxHP = 100.0f;
	CurHP = MaxHP;
	MaxShieldAmount = 0.0f;
	CurShieldAmount = MaxShieldAmount;
	DamageAmount = 10.0f;
	SphereRadius = 300.0f;
	AttackRange = 250.0f;

	/*FSMComponent = CreateDefaultSubobject<UCMeleeEnemyFSM> (TEXT("FSMComp"));*/
}

// Called when the game starts or when spawned
void ACEnemyBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACEnemyBase::SetEnemyManager(ACEnemyManager* ManagerActor, int32 ID)
{
	Manager = ManagerActor;
	MyUniqeID = ID;
}

void ACEnemyBase::SetStateCHASE(ACharacter* Player)
{

}

void ACEnemyBase::AttackRandomInt()
{
	//공격권 다시 돌리는것은 나중에 생각하기

// 	//랜덤값을 뽑아서 일정값 이상일 경우 다시 공격권을 뿌리게 만듦
// 	int32 RandomInt = FMath::RandRange(1,100);
//
// 	if (RandomInt >= 70)
// 	{
// 		Manager->ReturnbCanAttack();
// 	}
}

void ACEnemyBase::EnemyHitDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
bool bFromSweep, const FHitResult& SweepResult)
{
	// 	float ActualDamage = FMath::Max(0.0f, Damage /*- Defence*/);
	// 	CurHP = FMath::Clamp(CurHP - ActualDamage, 0.0f, MaxHP);
	//
	// 	if (CurHP <= 0.0f)
	// 	{
	// 		OnDeath();
	// 	}
}

void ACEnemyBase::Attack()
{
	if ( Target )
	{
		//공격하는 코드 작성
	}
}

void ACEnemyBase::LoadStatsFromAsset()
{
	if ( StatsAsset )
	{
		const FCEnemyStats& Stats = StatsAsset->Stats;
		MaxHP = Stats.MaxHP;
		CurHP = MaxHP;
		MaxShieldAmount = Stats.ShieldAmount;
		CurShieldAmount = MaxShieldAmount;
		DamageAmount = Stats.DamageAmount;
		SphereRadius = Stats.SphereRadius;
		AttackRange = Stats.AttackRange;
	}
}

void ACEnemyBase::CheckPlayerInRange()
{

}

void ACEnemyBase::OnPlayerLost()
{

}

void ACEnemyBase::OnDeath()
{

}

void ACEnemyBase::Hit(FString Name)
{
}

void ACEnemyBase::SetHP(float value)
{
	
}

void ACEnemyBase::SetShieldAmount(float value)
{

}
