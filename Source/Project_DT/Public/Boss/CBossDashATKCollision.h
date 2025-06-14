// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBossEnemy.h"
#include "CBossDashATKCollision.generated.h"

UCLASS()
class PROJECT_DT_API ACBossDashATKCollision : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACBossDashATKCollision();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GetMyBoss(ACBossEnemy* Boss);

	ACBossEnemy* MyBoss;

	void OnDashCollision();

	void OffDashCollision();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DashATK)
	class UBoxComponent* DashATKCollision;

	UFUNCTION()
	void DashPlayerHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
