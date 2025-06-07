// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CTargetingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_DT_API UCTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCTargetingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
private:
	virtual void TickComponent ( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction ) override;
public:
	//FORCEINLINE AActor* GetLockedOnTarget ( ) const { return LockedOnTarget; }
	FORCEINLINE bool IsLockedOn ( ) const { return bIsLockedOn; }

private:
	UPROPERTY(VisibleAnywhere)
	class ACharacter* OwnerCharacter;
	UPROPERTY ( VisibleAnywhere )
	class UCameraComponent* Camera;
	UPROPERTY ( VisibleAnywhere )
	class USpringArmComponent* SpringArm;
	UPROPERTY ( VisibleAnywhere )
	class UCWeaponComponent* Weapon;
	UPROPERTY ( VisibleAnywhere )
	class UCMovementComponent* Movement;
	UPROPERTY ( VisibleAnywhere )
	class UMotionWarpingComponent* Motion;
	UPROPERTY ( VisibleAnywhere )
	class UCCameraActionComponent* CameraComp;


	


private:
    UFUNCTION(BlueprintCallable)
    AActor* FindClosestEnemyByDirection(float MaxDistance, const FVector& InputDirection);

public:
	void OnLookOn ( );
	void UpdateLockOn ( float DeltaSeconds );
	void ResetLockOn ( );

	bool bSearchTarget=false;
private:
	UPROPERTY()
    AActor* LockedOnTarget = nullptr;

    bool bIsLockedOn = false;

	float currentTime = 0.0f;

	float TimeSinceLostTarget = 0.f;

	UPROPERTY(EditAnywhere, Category = "LockOn")
	float TargetLossTimeout = 5.f;
public:
	UPROPERTY(EditAnywhere, Category = "LockOn")
	class AActor* TargetEnemy;

};
