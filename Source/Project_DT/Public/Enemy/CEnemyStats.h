// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CEnemyStats.generated.h"

USTRUCT(BlueprintType)
struct FCEnemyStats
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
    float MaxHP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
    float ShieldAmount; //쉴드게이지 량

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float DamageAmount;   //데미지 수치

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float SphereRadius;  //플레이어를 탐지할때 원 범위
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
    float ChaseDistance;    //CHASE상태에서 CHASE상태가 유지되는 거리

    UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Stats)
    float WanderSpeed;

    UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Stats)
    float WanderRadius; //WANDER상태에서 플레이어가 다가오면 뒤로 물러나게 만드는 거리

    UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Stats)
    float BackStepSpeed; //플레이어가 붙어서 뒤로 물러날때 이동속도
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float AttackRange;  //공격이 실행되는 범위

    // RangedEnemy를 위한 속성
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
    float AttackDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
    float ProjectileSpeed;
};
