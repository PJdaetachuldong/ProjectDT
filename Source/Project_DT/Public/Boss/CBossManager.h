// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBossManager.generated.h"

UCLASS()
class PROJECT_DT_API ACBossManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACBossManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Start)
	class UBoxComponent* StartCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Boss)
	class ACBossEnemy* Boss;

	UFUNCTION()
	void Start(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
