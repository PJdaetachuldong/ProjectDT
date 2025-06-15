// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boss/CBossWeapon.h"
#include "CTutorialEnemy.h"
#include "CTutoWeapon.generated.h"

UCLASS()
class PROJECT_DT_API ACTutoWeapon : public ACBossWeapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACTutoWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* SceneComp;

	ACTutorialEnemy* MyOwner;

	virtual void PlayParringAnim() override;
	virtual bool CheckGuardBool() override;

	void SetTutoOwner(ACTutorialEnemy* WHO);

	FTimerHandle TutoParringCheckTimer;

	bool IsTutoPlayerParring = false;

	void TutoPlayerDamage();

	//보스 공격 종류에 따라 피격 불러오는 Hit Data가 다름
	int32 TutoHitNumber = 0;

	UPROPERTY(EditAnywhere )
	class UBoxComponent* WeaponCollComp;

	UFUNCTION()
	void TutoWeaponOverlap ( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult );
};
