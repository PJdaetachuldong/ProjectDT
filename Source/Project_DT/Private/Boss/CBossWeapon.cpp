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
#include "Boss/CBossAnim.h"

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
		//공격 판정 콜리전 비활성화
	}
}

// Called when the game starts or when spawned
void ACBossWeapon::BeginPlay()
{
	Super::BeginPlay();

	if(IsTutoWeapon) return;
		SwordCollComp->AttachToComponent(SwordMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Collision_Socket"));
		SwordCollComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	
}

// Called every frame
void ACBossWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsTutoWeapon) return;

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

	IsPlayerParring = true;
	
	MyBoss->AnimInstance->Montage_Play(MyBoss->AM_ParringInteraction);
	MyBoss->FSMComponent->ParringDontMoveTime = 0.0f;
}

void ACBossWeapon::PlayerDamage()
{
	//만약 플레이어가 패링 감지중이면
	if (IsPlayerParring)
	{
		OverlapRotator = FRotator::ZeroRotator;

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ParringEffect,
			OverlapLocation, // 충돌 지점
			OverlapRotator, // 충돌 방향
			FVector(1.0f), // 스케일
			true, // Auto Destroy
			true // Auto Activate
		);

		IsPlayerParring = false;

		return;
	}

	//슬래쉬 이펙트 생성
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		SlashEffect,
		OverlapLocation,
		OverlapRotator,
		FVector(1.0f),
		true,
		true
	);

	// 사용자 정의 데미지 이벤트 생성
	MyBoss->HitData->HitDatas[HitNumber].SendDamage(MyBoss, this, MyBoss->Target);
}

void ACBossWeapon::DashAttackHitCheck()
{
	OverlapLocation = MyBoss->Target->GetActorLocation();

	GetWorld()->GetTimerManager().SetTimer(ParringCheckTimer, this, &ACBossWeapon::PlayerDamage, 0.000001f, false);

// 	//만약 플레이어가 패링 감지중이면
// 	if (IsPlayerParring)
// 	{
// 		OverlapRotator = FRotator::ZeroRotator;
// 
// 		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
// 			GetWorld(),
// 			ParringEffect,
// 			OverlapLocation, // 충돌 지점
// 			OverlapRotator, // 충돌 방향
// 			FVector(1.0f), // 스케일
// 			true, // Auto Destroy
// 			true // Auto Activate
// 		);
// 
// 		return;
// 	}
// 
// 	//슬래쉬 이펙트 생성
// 	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
// 		GetWorld(),
// 		SlashEffect,
// 		OverlapLocation,
// 		OverlapRotator,
// 		FVector(1.0f),
// 		true,
// 		true
// 	);
// 
// 	// 사용자 정의 데미지 이벤트 생성
// 	MyBoss->HitData->HitDatas[HitNumber].SendDamage(MyBoss, this, MyBoss->Target);
}

void ACBossWeapon::WeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPlayer* Player = Cast<ACPlayer>(OtherActor);

	if (Player)
	{
		OverlapLocation = bFromSweep ? FVector(SweepResult.ImpactPoint) : OtherActor->GetActorLocation();

		OverlapRotator = bFromSweep ? SweepResult.ImpactNormal.Rotation() : FRotator::ZeroRotator;
		
		//가드가 가능한 공격일 경우
		if (IsGuard)
		{
			

// 			FVector ImpactNormal = FVector::ZeroVector;
// 			if (bFromSweep && SweepResult.IsValidBlockingHit())
// 			{
// 				ImpactNormal = SweepResult.ImpactNormal;
// 			}
// 			else
// 			{
// 				// 스윕 데이터가 없으면 무기의 이동 방향 또는 기본 방향 사용
// 				ImpactNormal = GetActorForwardVector().GetSafeNormal();
// 				UE_LOG(LogTemp, Warning, TEXT("Sweep data invalid, using Forward Vector: %s"), *ImpactNormal.ToString());
// 			}
// 
// 			// (0, 0, 0) 방지
// 			if (ImpactNormal.IsNearlyZero())
// 			{
// 				ImpactNormal = FVector(0, 0, 1); // 기본값으로 위쪽 방향
// 				UE_LOG(LogTemp, Warning, TEXT("ImpactNormal is zero, defaulting to (0, 0, 1)"));
// 			}
// 
// 			// 회전 계산
// 			if (FMath::Abs(ImpactNormal.Z) > FMath::Abs(ImpactNormal.X)) // 세로 방향
// 			{
// 				OverlapRotator = ImpactNormal.Rotation();
// 			}
// 			else // 가로 방향
// 			{
// 				OverlapRotator = FRotator(0.0f, ImpactNormal.Rotation().Yaw, 0.0f);
// 			}
			GetWorld()->GetTimerManager().SetTimer(ParringCheckTimer,this, &ACBossWeapon::PlayerDamage,0.1f,false);

// 			//만약 플레이어가 패링 감지중이면
// 			if (/*Weapon->GetDoAction() && Weapon->GetDoAction()->RetrunParry()*/ IsPlayerParring)
// 			{
// 				GEngine->AddOnScreenDebugMessage(111, 10.0f, FColor::White, TEXT("PlayerParrying"));
// 
// 				IsPlayerParring = false;
// 
// // 				//현재 재생중인 몽타주 멈춤
// // 				MyBoss->AnimInstance->StopAllMontages(0.0f);
// // 				MyBoss->FSMComponent->AttackState = EBossATTACKState::NONE;
// // 
// // 				MyBoss->AnimInstance->Montage_Play(MyBoss->AM_ParringInteraction);
// 
// 				return;
// 			}
// 
// 			FVector HitLocation = bFromSweep ? FVector(SweepResult.ImpactPoint) : OtherActor->GetActorLocation();
// 			FRotator HitRotation = bFromSweep ? SweepResult.ImpactNormal.Rotation() : FRotator::ZeroRotator;
// 
// 			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
// 				GetWorld(),
// 				HitEffect,
// 				HitLocation, // 충돌 지점
// 				HitRotation, // 충돌 방향
// 				FVector(1.0f), // 스케일
// 				true, // Auto Destroy
// 				true // Auto Activate
// 			);
// 
// 			// 사용자 정의 데미지 이벤트 생성
// 			MyBoss->HitData->HitDatas[HitNumber].SendDamage(MyBoss, this, Player);
 		}

		//가드 불가능 공격일 경우
		else
		{
			//필살기 첫번째 공격일 경우
			if (MyBoss->AnimInstance->Montage_IsPlaying(MyBoss->AM_SPAttack))
			{
				MyBoss->IsSPFirstATKHit = true;
			}

// 			FVector ImpactNormal = SweepResult.ImpactNormal;
// 			// Impact Normal을 Y축(세로)과 X축(가로) 방향으로 비교
// 			if (FMath::Abs(ImpactNormal.Z) > FMath::Abs(ImpactNormal.X)) // 세로 방향(위/아래) 충돌
// 			{
// 				OverlapRotator = ImpactNormal.Rotation(); // 세로 방향에 맞게 회전
// 			}
// 			else // 가로 방향(좌/우) 충돌
// 			{
// 				OverlapRotator = FRotator(0.0f, ImpactNormal.Rotation().Yaw, 0.0f); // Yaw로 가로 회전
// 			}

			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				SlashEffect,
				OverlapLocation,
				OverlapRotator,
				FVector(1.0f),
				true,
				true
			);


// 			FVector HitLocation = bFromSweep ? FVector(SweepResult.ImpactPoint) : OtherActor->GetActorLocation();
// 			FRotator HitRotation = bFromSweep ? SweepResult.ImpactNormal.Rotation() : FRotator::ZeroRotator;
// 
// 			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
// 				GetWorld(),
// 				HitEffect,
// 				HitLocation, // 충돌 지점
// 				HitRotation, // 충돌 방향
// 				FVector(1.0f), // 스케일
// 				true, // Auto Destroy
// 				true // Auto Activate
// 			);

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

