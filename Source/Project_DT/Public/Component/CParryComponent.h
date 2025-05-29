// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CParryComponent.generated.h"

UENUM()
enum class EParryState : uint8
{
	TL,BL,TR,BR, Max
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam ( FParryDetectedSignature , EParryState , ParryDirection );

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_DT_API UCParryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCParryComponent();

	FORCEINLINE void OnParryCollision ( ) { bIsParrying = true; }
	FORCEINLINE void OffParryCollision ( ) { bIsParrying = false; }
	FORCEINLINE bool GetParryState() { return bIsParrying; }


protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void DetectActor();
public:
	UPROPERTY(BlueprintAssignable)
	FParryDetectedSignature OnParryDetected;

private:
	UPROPERTY()
	TArray<AActor*> TemporarilyIgnoredActors;

	bool IsInDetectionAngle(AActor* TargetActor) const;
	EParryState GetHitQuadrant(const FVector& ImpactPoint) const;
	void HandleTemporaryIgnore(AActor* ActorToIgnore);


private:
	class ACharacter* OwnerCharacter;
	UPROPERTY(VisibleAnywhere)
	class UCWeaponComponent* Weapon;

	bool bIsParrying;
};
