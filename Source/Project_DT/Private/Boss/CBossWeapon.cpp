// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/CBossWeapon.h"
#include "Components/BoxComponent.h"
#include "Character/CPlayer.h"
#include "Weapons/CWeaponStuctures.h"
#include "Boss/CBossEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Global.h"
#include "Boss/FSM/CBossFSM.h"
#include "Weapons/CWeaponComponent.h"
#include "Weapons/CDoAction.h"

// Sets default values
ACBossWeapon::ACBossWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SwordMesh = CreateDefaultSubobject<USkeletalMeshComponent>(L"SwordMesh");
	/*SwordMesh->SetupAttachment(GetMesh());*/

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(L"/Script/Engine.SkeletalMesh'/Game/ODH/Asset/Boss/ElfArden/BaseMesh/Sword/SK_sword.SK_sword'");
	if (TempMesh.Succeeded())
	{
		SwordMesh->SetSkeletalMesh(TempMesh.Object);
		/*SwordMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Weapon_Socket"));*/

		SwordCollComp = CreateDefaultSubobject<UBoxComponent>(L"SwordCollision");
		SwordCollComp->SetBoxExtent(FVector(10, 48, 10));
		SwordCollComp->OnComponentBeginOverlap.AddDynamic(this, &ACBossWeapon::WeaponOverlap);
		SwordCollComp->SetCollisionProfileName(FName("BossWeapon"));
		SwordCollComp->AttachToComponent(SwordMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Collision_Socket"));
		//공격 판정 콜리전 비활성화
		SwordCollComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

// Called when the game starts or when spawned
void ACBossWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACBossWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetOwner())
	{
		MyBoss = Cast<ACBossEnemy>(GetOwner());
	}
}

bool ACBossWeapon::CheckGuardBool()
{
	return IsGuard;
}

void ACBossWeapon::PlayParringAnim()
{
	//현재 재생중인 몽타주 멈춤
	MyBoss->AnimInstance->StopAllMontages(0.0f);
	MyBoss->FSMComponent->AttackState = EBossATTACKState::NONE;

	MyBoss->AnimInstance->Montage_Play(MyBoss->AM_ParringInteraction);
}

void ACBossWeapon::WeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPlayer* Player = Cast<ACPlayer>(OtherActor);

	if (Player)
	{
		//가드가 가능한 공격일 경우
		if (IsGuard)
		{
			UCWeaponComponent* Weapon = CHelpers::GetComponent<UCWeaponComponent>(Player);

			//만약 플레이어가 패링 감지중이면
			if (Weapon->GetDoAction() && Weapon->GetDoAction()->RetrunParry())
			{

// 				//현재 재생중인 몽타주 멈춤
// 				MyBoss->AnimInstance->StopAllMontages(0.0f);
// 				MyBoss->FSMComponent->AttackState = EBossATTACKState::NONE;
// 
// 				MyBoss->AnimInstance->Montage_Play(MyBoss->AM_ParringInteraction);

				return;
			}

			FVector HitLocation = bFromSweep ? FVector(SweepResult.ImpactPoint) : OtherActor->GetActorLocation();
			FRotator HitRotation = bFromSweep ? SweepResult.ImpactNormal.Rotation() : FRotator::ZeroRotator;

			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				HitEffect,
				HitLocation, // 충돌 지점
				HitRotation, // 충돌 방향
				FVector(1.0f), // 스케일
				true, // Auto Destroy
				true // Auto Activate
			);

			// 사용자 정의 데미지 이벤트 생성
			MyBoss->HitData->HitDatas[HitNumber].SendDamage(MyBoss, this, Player);
		}

		//가드 불가능 공격일 경우
		else
		{
			//필살기 첫번째 공격일 경우
			if (MyBoss->AnimInstance->Montage_IsPlaying(MyBoss->AM_SPAttack))
			{
				MyBoss->IsSPFirstATKHit = true;
			}

			FVector HitLocation = bFromSweep ? FVector(SweepResult.ImpactPoint) : OtherActor->GetActorLocation();
			FRotator HitRotation = bFromSweep ? SweepResult.ImpactNormal.Rotation() : FRotator::ZeroRotator;

			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				HitEffect,
				HitLocation, // 충돌 지점
				HitRotation, // 충돌 방향
				FVector(1.0f), // 스케일
				true, // Auto Destroy
				true // Auto Activate
			);

			//무조건 사용자 정의 데미지 이벤트 생성
			MyBoss->HitData->HitDatas[HitNumber].SendDamage(MyBoss, this, Player);
		}
		

// 		//만약 대쉬 공격 애니메이션 중이라면
// 		if (!AnimInstance->Montage_IsPlaying(AM_DashAttack))
// 		{
// 			IsDashAttackHit = true;
// 		}
	}
}

