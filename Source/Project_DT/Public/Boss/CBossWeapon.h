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

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SwordMesh;

	UPROPERTY(EditAnywhere )
	class UBoxComponent* SwordCollComp;

	UFUNCTION( )
	void WeaponOverlap ( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult );
};
