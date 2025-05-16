// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase/CEnemyBase.h"
#include "CMeleeEnemy.generated.h"

UCLASS()
class PROJECT_DT_API ACMeleeEnemy : public ACEnemyBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACMeleeEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
    float ChaseDistance;    //CHASE상태에서 CHASE상태가 유지되는 거리

    UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Stats)
    float WanderSpeed;

    UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Stats)
    float WanderRadius; //WANDER상태에서 플레이어가 다가오면 뒤로 물러나게 만드는 거리

    UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Stats)
    float BackStepSpeed; //플레이어가 붙어서 뒤로 물러날때 이동속도

	virtual void LoadStatsFromAsset() override;

	virtual void CheckPlayerInRange() override;

	virtual void SetStateCHASE(ACharacter* Player) override;

	virtual void EnemyHitDamage(float Damage) override;
};
