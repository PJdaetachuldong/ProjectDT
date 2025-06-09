// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase/CEnemyBase.h"
#include "CMeleeEnemy.generated.h"

UCLASS()
class PROJECT_DT_API ACMeleeEnemy : public ACEnemyBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACMeleeEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// FSM 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UCMeleeEnemyFSM* FSMComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
    float ChaseDistance;    //CHASE상태에서 CHASE상태가 유지되는 거리

    UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Stats)
    float WanderSpeed;

    UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Stats)
    float WanderRadius; //WANDER상태에서 플레이어가 다가오면 뒤로 물러나게 만드는 거리

    UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Stats)
    float BackStepSpeed; //플레이어가 붙어서 뒤로 물러날때 이동속도

    float CurBreakTime = 0.0f;

    float ResetShieldTime = 8.0f;

    int32 ShieldBreakHit = 0;

	virtual void LoadStatsFromAsset() override;

	virtual void CheckPlayerInRange() override;

	virtual void SetStateCHASE(ACharacter* Player) override;

	virtual void EnemyHitDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

    UAnimInstance* AnimInstance;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
    class UAnimMontage* AM_ShieldHit;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
    class UAnimMontage* AM_Attack;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
    class UAnimMontage* AM_Break;

    ////////플레이어 데미지 부분////////////
private:
    struct FEnemyDamageData
    {
        float Power;
        class ACharacter* Character;
        class AActor* Causer;

        struct FActionDamageEvent* Event;
    } Damage;

protected:
    virtual void Hitted();
public:
    float TakeDamage(float TakeDamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

    virtual void Hit(FString Name) override;
};
