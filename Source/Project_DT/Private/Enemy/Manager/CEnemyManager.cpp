// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Manager/CEnemyManager.h"
#include "Enemy/EnemyBase/CEnemyBase.h"

// Sets default values
ACEnemyManager::ACEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ACEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACEnemyManager::SendManager()
{
	for (int32 i = 0; i < Enemies.Num(); ++i)
	{
		//해당 에너미에게 매니저가 누구이고 ID가 무엇인지 정보를 전달
		Enemies[i]->SetEnemyManager(this, UniqeID);

		//Map에 ID와 에너미를 같이 저장
		EnemyMap.Add(Enemies[i]->MyUniqeID, Enemies[i]);

		//ID가 중복되지 않도록 값을 1 늘려줌
		UniqeID++;
	}
}

void ACEnemyManager::SendPlayerEncounter(ACharacter* Player, int32 ID)
{
	//ID를 사용해 Map에서 해당 에너미를 찾음
	ACEnemyBase** FoundEnemy = EnemyMap.Find(ID);

	//해당 에너미 주소를 사용해서 배열 인덱스를 찾음
	int32 EnemyIndex = Enemies.Find(*FoundEnemy);

	//반복문을 사용해 모든 에너미에게 플레이어 발견 정보를 전달
	for (int32 i = 0; i < Enemies.Num(); ++i)
	{
		//만약 처음 플레이어를 발견한 에너미라면
		if (i == EnemyIndex)
		{
			//실행하지 않고 다음 에너미가 불리도록 넘김
			continue;
		}
		//플레이어의 정보를 전달
		Enemies[i]->SetStateCHASE(Player);
	}
}

void ACEnemyManager::ReturnCanAttack()
{
	int32 RandomIndex = FMath::RandRange(0,Enemies.Num() - 1);

	Enemies[RandomIndex]->IsCanAttack = true;
}

void ACEnemyManager::RemoveEnemiesList(int32 ID, bool CanAttack)
{
	//ID를 사용해 Map에서 해당 에너미를 찾음
	ACEnemyBase** FoundEnemy = EnemyMap.Find(ID);

	//해당 에너미 주소를 사용해서 배열 인덱스를 찾음
	int32 EnemyIndex = Enemies.Find(*FoundEnemy);

	//자신이 마지막 에너미이면 공격권 넘기기를 실행하지 않음
	if (Enemies.Num() == 1)
	{
		//리스트와 맵에서 삭제
		Enemies.RemoveAt(EnemyIndex);
		EnemyMap.Remove(ID);
	}

	//마지막 에너미가 아니면
	else
	{
		//만약 공격권을 갖고 있다면
		if (CanAttack)
		{
			//남은 에너미 중 랜덤하게 하나를 고름
			int32 RandomIndex = FMath::RandRange(0,Enemies.Num() - 1);

			//공격권을 해당 에너미에게 넘겨주게 만들어줌
			Enemies[RandomIndex]->IsCanAttack = true;

			//리스트와 맵에서 삭제
			Enemies.RemoveAt(EnemyIndex);
			EnemyMap.Remove(ID);
		}

		//공격권을 갖고 있지 않을 경우
		else
		{
			//리스트와 맵에서 삭제
			Enemies.RemoveAt(EnemyIndex);
			EnemyMap.Remove(ID);
		}
	}
}

