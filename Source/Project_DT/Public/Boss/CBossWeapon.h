// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBossWeapon.generated.h"

UCLASS()
class PROJECT_DT_API ACBossWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACBossWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class ACBossEnemy* MyBoss;

	//보스 공격 종류에 따라 피격 불러오는 Hit Data가 다름
	int32 HitNumber = 0;

	//가드가 가능한 공격인지 체크하는 불
	bool IsGuard = false;
	
	//가드 가능한 공격인지 전달하는 변수
	bool CheckGuardBool();

	void PlayParringAnim();

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SwordMesh;

	UPROPERTY(EditAnywhere )
	class UBoxComponent* SwordCollComp;

	UFUNCTION( )
	void WeaponOverlap ( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult );
};
