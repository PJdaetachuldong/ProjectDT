// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CEnemyManager.generated.h"

UCLASS()
class PROJECT_DT_API ACEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
	TArray<class ACEnemyBase*> Enemies;

	UPROPERTY(VisibleAnywhere, Category = Enemy)
	TMap<int32, ACEnemyBase*> EnemyMap;

	int32 UniqeID = 100;

	//각 에너미들에게 매니저를 처음에 셋팅하는 함수
	void SendManager();

	//어떤 에너미가 플레이어를 발견했을 경우 다른 에너미들에게 플레이어를 전달하는 함수
	void SendPlayerEncounter(ACharacter* Player,int32 ID);

	//에너미의 공격이 끝났을 경우 공격권을 다시 누군가에게 돌려주기 위한 함수
	void ReturnCanAttack();

	//에너미가 사망했을 경우 불리는 함수
	void RemoveEnemiesList(int32 ID,bool CanAttack);

};
