// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPAttackCheckCollision.generated.h"

UCLASS()
class PROJECT_DT_API ASPAttackCheckCollision : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPAttackCheckCollision();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere)
	class USphereComponent* Collision;
	

	void SetActive(bool Value, FVector SpawnLocation);
};
