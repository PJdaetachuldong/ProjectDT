// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../CEnemyDataAsset.h"
#include "../Manager/CEnemyManager.h"
#include "CEnemyBase.generated.h"

UCLASS()
class PROJECT_DT_API ACEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float CurHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float MaxShieldAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float CurShieldAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float SphereRadius;  //플레이어를 탐지할때 원 범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float AttackRange;  //공격이 실행되는 범위

	//Data Asset 참조
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	UCEnemyDataAsset* StatsAsset;

	//Target 저장
	ACharacter* Target;

	//에너미 매니저 저장하기 위한 변수와 매니저를 받는 함수
	UPROPERTY(VisibleAnywhere, Category = Manager)
	ACEnemyManager* Manager;

	//자신의 ID를 기억하기 위한 변수
	UPROPERTY(VisibleAnywhere, Category = Manager)
	int32 MyUniqeID;

	void SetEnemyManager(ACEnemyManager* ManagerActor, int32 ID);

	//자신과 같은 리스트에 있는 에너미가 플레이어를 발견했을 경우 플레이어를 추적하게 만드는 함수
	virtual void SetStateCHASE(ACharacter* Player);

	void AttackRandomInt();

	//true일 때만 플레이어를 공격하는 공격권 불
	bool IsCanAttack = false;

	//공통 기능
	UFUNCTION(BlueprintCallable)
	virtual void EnemyHitDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	virtual void Attack();

	UFUNCTION(BlueprintCallable)
	virtual void LoadStatsFromAsset();

	UFUNCTION(BlueprintCallable)
	virtual void CheckPlayerInRange();

	UFUNCTION(BlueprintCallable)
	 void OnPlayerLost();

	UFUNCTION(BlueprintCallable)
	void OnDeath();

	virtual void Hit(FString Name);

	virtual void SetHP(float value);

	virtual void SetShieldAmount(float value);
};
