// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/CBossWeapon.h"
#include "Components/BoxComponent.h"
#include "Character/CPlayer.h"
#include "Weapons/CWeaponStuctures.h"
#include "Boss/CBossEnemy.h"
#include "Kismet/GameplayStatics.h"

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
		SwordCollComp->OnComponentBeginOverlap.AddDynamic(this, &ACBossWeapon::WeaponOverlap);
		SwordCollComp->SetCollisionProfileName(FName("BossWeapon"));
		SwordCollComp->AttachToComponent(SwordMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Collision_Socket"));
		SwordCollComp->SetBoxExtent(FVector(10, 48, 3));
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

void ACBossWeapon::WeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPlayer* Player = Cast<ACPlayer>(OtherActor);

	if (Player)
	{
		//만약 플레이어가 패링 감지중이면
		if (Player->Parry->bIsParrying)
		{
			//경직 애니메이션 재생
			GEngine->AddOnScreenDebugMessage(130, 1.0f, FColor::Red, TEXT("Player Parrying"));

			return;
		}

		// 사용자 정의 데미지 이벤트 생성
		MyBoss->HitData->HitDatas[HitNumber].SendDamage(MyBoss, this, Player);

// 		//만약 대쉬 공격 애니메이션 중이라면
// 		if (!AnimInstance->Montage_IsPlaying(AM_DashAttack))
// 		{
// 			IsDashAttackHit = true;
// 		}
	}
}

