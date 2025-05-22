// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CRangeAttack.generated.h"

UCLASS()
class PROJECT_DT_API ACRangeAttack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACRangeAttack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComp;

	//던져질때 속도
	float ThrowSpeed = 1600.0f;

	//현재 자신이 살아 있는 시간을 체크
	float CurLifeTime = 0.0f;

	//살 수 있는 생존 시간
	float LimitLifeTime = 7.0f;

	//타겟을 향해 날아가는 방향을 저장하는 변수
	FVector Direction;

	//콜리전과 외형을 비활성화 변환 시켜주는 함수 
	void SetActive(bool Value, FVector DirectionToTarget);

	void OverlapOther(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
