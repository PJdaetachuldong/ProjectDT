// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBase/CEnemyBase.h"
#include "DataAsset/HitDataAsset_BossToPlayer.h"
#include "CBossEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_DT_API ACBossEnemy : public ACEnemyBase
{
	GENERATED_BODY()
	
public:
	ACBossEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//원거리 공격 오브젝트 할당
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACRangeAttack> RangedAttackFactory;

	//원거리 공격 오브젝트를 Object Pool로 관리
	//오브젝트의 최초 생성 수 (혹시몰라 여유분까지)
	int32 MaxRangedAttackCount = 4;

	//원거리 공격 오브젝트 목록
	UPROPERTY(EditAnywhere, Category = ObjectPool)
	TArray<class ACRangeAttack*> RangedAttackList;

	//현재 쉴드가 없는 시간을 체크하는 변수
	float CurBreakTime = 0.0f;

	//쉴드가 다시 복구되는 시간
	float ResetShieldTime = 7.0f;

	//돌진 공격이 맞았는지 판단하는 변수
	bool IsDashAttackHit = false;
	
	//가드 조건 게이지
	float GuardGage = 0.0f;

	//가드가 실행되는 조건 설정 변수
	float GuardPlaying = 10.0f;

	//가드가 성공하였는지 체크하는 변수
	bool IsGuardSucssess = false;

	//현재 필살기를 사용하려고 준비 자세를 취하고 있는지 확인하는 변수
	bool IsReadySPAttack = false;

	//쉴드 상태에서 공격을 몇회 맞았는지 체크하는 변수
	int32 ShieldHitCount = 0;

	//쉴드 상태에서 일정 횟수 이상 공격을 맞았을 경우 반격 공격이 실행되는 조건 변수
	int32 ShieldHitCounter = 2;

	int32 ShieldBreakHit = 0;

	bool IsSPRun = false;

	bool IsSPFirstATKHit = false;

	//쉴드가 있을때 카운터까지 횟수 체크하는 시간
	float GuardingTime = 0.0f;

	bool BackstepUse();

	int32 BackstepPercent = 60;

	//일단 임시로 하는 발사 위치 설정
	UPROPERTY(EditAnywhere)
	class USceneComponent* ThrowPosition;
	//일단 임시로 하는 발사 위치 설정

	UAnimInstance* AnimInstance;
	
	void ReadyDashAttack();

	void OnSwordCollision();

	void OffSwordCollision();

	bool OnGuardCollision();

	void OffGuardCollision();

	bool CheckPlayer();

	FVector Attack3Location;

	void SetLocation();

	bool CheckAttack1_3Player();

	void AttackTurn();

	void ATK2_4Turn();

	void PlayNextSectionAttack( UAnimMontage* CurrentMontage , FName CurrentSection );

	FName GetNextSection(FName SectionName);

	void RunCheckPlayerDist();

	void DashAttackEnd();

	float SetRateDown(UAnimMontage* CurrentMontage, FName CurrentSection);

	bool SetGuardBool(UAnimMontage* CurrentMontage, FName CurrentSection);

	// FSM 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UCBossFSM* FSMComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Montage)
	class UAnimMontage* AM_Die;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Montage)
	class UAnimMontage* AM_ComboAttack_01;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Montage)
	class UAnimMontage* AM_ComboAttack_02;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Montage)
	class UAnimMontage* AM_DashAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Montage)
	class UAnimMontage* AM_RangedAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Montage)
	class UAnimMontage* AM_Guard;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Montage)
	class UAnimMontage* AM_SPAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Montage)
	class UAnimMontage* AM_Break;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Montage)
	class UAnimMontage* AM_ShieldHit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Montage)
	class UAnimMontage* AM_ParringInteraction;

	//Rate Scale 매핑 초기화 함수
	void InitializeMontageMap();

	TMap<UAnimMontage*, TArray<FMontageRateScale>> MontageScaleMap;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<class ACBossWeapon> MyWeapon;

	class ACBossWeapon* SpawnWeapon;

	UPROPERTY(EditAnywhere, Category = SPAttack)
	TSubclassOf<class ASPAttackCheckCollision> SPAttackCollision;

	UPROPERTY(EditAnywhere, Category = SPAttack)
	class ASPAttackCheckCollision* SpawnSpColli;

	void SetSPColli();

	void SPAttack();

	void EndSPAttack();

	UPROPERTY(EditAnywhere, Category = DataAsset)
	UHitDataAsset_BossToPlayer* HitData;

	//각 공격을 쉽게 불러올 수 있게 맵으로 정리
	TMap<FName, EAttackType> AttackSectionToEnumMap;

	void InitAttackTMap();
	
	virtual void EnemyHitDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void LoadStatsFromAsset() override;

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

	UFUNCTION()
    void OnPlayerHealed();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Start)
	class UBoxComponent* StartCollision;

	bool BossStart = false;

	UFUNCTION()
	void Start(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};

USTRUCT(BlueprintType)
struct FMontageRateScale
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SectionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RateScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsGuard;
};

UENUM(BlueprintType)
enum class EAttackType : uint8
{
    Attack01_01,
    Attack01_02,
    Attack01_03,
    Attack01_04,
    Attack02_01,
    Attack02_02,
    Attack02_03,
    Attack02_04,
    None
};