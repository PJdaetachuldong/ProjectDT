// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CTutoManager.generated.h"

UCLASS()
class PROJECT_DT_API ACTutoManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACTutoManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = TutoEnemy)
	TSubclassOf<class ACTutorialEnemy> TutoEnemy;

	ACTutorialEnemy* SpawnTutoEnemy1;
	ACTutorialEnemy* SpawnTutoEnemy2;
	ACTutorialEnemy* SpawnTutoEnemy3;

	class AAIController* AIController;

	UPROPERTY(EditAnywhere)
	class USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* StartCollision;

	UPROPERTY(EditAnywhere, Category = SpawnLocation)
	class USceneComponent* FirstTransform;

	UPROPERTY(EditAnywhere, Category = SpawnLocation)
	class USceneComponent* SecondTransform;

	UPROPERTY(EditAnywhere, Category = SpawnLocation)
	class USceneComponent* ThirdTransform;

	bool IsTutoStart = false;

	bool IsFirstTutoEnemyDIE = false;
	bool IsSecondTutoEnemyDIE = false;

	void InputTutoEnemyDIE();

	float EnemySpawnDelay = 2.0f;
	float CurSpawnTime = 0.0f;

	bool IsAllSpawn = false;

	UFUNCTION()
	void SpawnStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
