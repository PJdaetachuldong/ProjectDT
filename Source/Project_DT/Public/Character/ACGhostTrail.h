// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACGhostTrail.generated.h"

UCLASS()
class PROJECT_DT_API AACGhostTrail : public AActor
{
	GENERATED_BODY()

public:
	AACGhostTrail();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(EditDefaultsOnly,Category="Capture")
	float StartDelay=0;
	UPROPERTY(EditDefaultsOnly,Category="Capture")
	float Interval = 0.25f;
	UPROPERTY(EditDefaultsOnly,Category="Capture")
	FLinearColor Color = FLinearColor(1,1,1,1);
	UPROPERTY(EditDefaultsOnly,Category="Capture")
	float Exponent=1;
	UPROPERTY(EditDefaultsOnly,Category="Capture")
	FVector Scale = FVector::OneVector;
	UPROPERTY(EditDefaultsOnly,Category="Capture")
	FVector ScaleAmount=FVector::ZeroVector;
private:
	UPROPERTY(EditDefaultsOnly)
	class UPoseableMeshComponent* Mesh;

private:
	class ACharacter* Owner;
	class UMaterialInstanceDynamic* Material;

	FTimerHandle TimerHandle;

};
