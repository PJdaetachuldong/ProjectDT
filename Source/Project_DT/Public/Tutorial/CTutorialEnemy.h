// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy/EnemyBase/CEnemyBase.h"
#include "AIController.h"
#include "Boss/DataAsset/HitDataAsset_BossToPlayer.h"
#include "CTutorialEnemy.generated.h"

UENUM()
enum class ETutoState : uint8
{
	IDLE UMETA(DisplayName = "IDLE"),
	CHASE UMETA(DisplayName = "CHASE"),
	ATTACKREADY UMETA(DisplayName = "ATTACKREADY"),
	ATTACK UMETA(DisplayName = "ATTACK"),
	BREAK UMETA ( DisplayName = "BREAK" ),
	DIE UMETA(DisplayName = "DIE")
};

UCLASS()
class PROJECT_DT_API ACTutorialEnemy : public ACEnemyBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACTutorialEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AAIController* AI;

	class ACPlayer* Target;

	class UCTutoAnim* AnimInstance;

	class ACTutoManager* Manager;

	void SettingManager(ACTutoManager* LevelManager);

	UPROPERTY(EditAnywhere, Category = DataAsset)
	UHitDataAsset_BossToPlayer* TutoHitData;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<class ACTutoWeapon> TutoWeapon;

	UPROPERTY(EditAnywhere, Category = Montage)
	class UAnimMontage* AM_TutoParryInter;

	class ACTutoWeapon* SpawnTutoWeapon;

	void OnTutoCollision();
	void OffTutoCollision();

	UPROPERTY(EditAnywhere, Category = Tutorial)
	bool IsParry = false;

	UPROPERTY(EditAnywhere, Category = Tutorial)
	bool IsATKGide = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category =FSM)
	ETutoState State = ETutoState::IDLE;

	virtual void LoadStatsFromAsset() override;

	virtual void SetHP(float value) override;

	virtual void SetShieldAmount(float value) override;

	void AttackTurn();

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

	float CurAttackTime = 0.0f;
	float AttackLimitTime = 2.5f;

	void IDLEEnd();

	void ATKEnd();

	bool IsDontHit = true;

	void DIESetLocation();
};
