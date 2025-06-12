// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/RangedAttackObject/CRangeAttack.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Weapons/CWeaponComponent.h"
#include "Character/CPlayer.h"
#include "Global.h"
#include "Boss/CBossWeapon.h"
#include "Boss/CBossEnemy.h"
#include "Weapons/CDoAction.h"

// Sets default values
ACRangeAttack::ACRangeAttack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
// 	BoxComp = CreateDefaultSubobject<UBoxComponent>(L"RangedATKCollision");
// 	SetRootComponent(BoxComp);
// 	
// 	BoxComp->SetCollisionProfileName(FName("BossWeapon"));
// 	BoxComp->SetBoxExtent(FVector(221, 23, 217));

// 	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(L"MeshComp");
// 	MeshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACRangeAttack::BeginPlay()
{
	Super::BeginPlay();
	
/*	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACRangeAttack::OverlapPlayer);*/
}

// Called every frame
void ACRangeAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//현재 가시성이 활성화 된 상태에서만 실행됨
// 	if ( MeshComp->IsVisible())
// 	{
		//해당 방향을 향해 계속 앞으로 날아가게 만들어야 함
		SetActorLocation(GetActorLocation() + (Direction * ThrowSpeed) * DeltaTime);

		//그리고 생존 시간을 체크
		CurLifeTime +=DeltaTime;
		
		//만약 현재 생존시간이 제한 생존시간을 넘을 경우
		if ( CurLifeTime >= LimitLifeTime )
		{
// 			//자신을 안보이게 하고 다시 오브젝트 풀로 되돌림
// 			SetActive(false, FVector(0));

			Destroy();
		}
/*	}*/
}

void ACRangeAttack::SetDirectionAndBoss(FVector ToPlayerDirection, ACBossEnemy* Who)
{
	MyBoss = Who;
	Direction = ToPlayerDirection;

	// 방향 벡터를 회전으로 변환 (Yaw만 고려)
	FRotator LookAtRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	// Z축 회전(Yaw)만 적용하여 캐릭터가 플레이어를 향하도록 설정
	FRotator NewRotation = FRotator(LookAtRotation);

	SetActorRotation(NewRotation);
}

void ACRangeAttack::OverlapPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
// 	ACPlayer* Player = Cast<ACPlayer>(OtherActor);
// 
// 	if (Player && MyBoss)
// 	{
// 		//가드가 가능한 공격일 경우
// 		if (MyBoss->SpawnWeapon->IsGuard)
// 		{
// 			UCWeaponComponent* Weapon = CHelpers::GetComponent<UCWeaponComponent>(Player);
// 
// 			//만약 플레이어가 패링 감지중이면
// 			if (Weapon->GetDoAction() && Weapon->GetDoAction()->RetrunParry())
// 			{
// 				return;
// 			}
// 
// 			// 사용자 정의 데미지 이벤트 생성
// 			MyBoss->HitData->HitDatas[MyBoss->SpawnWeapon->HitNumber].SendDamage(MyBoss, MyBoss->SpawnWeapon, Player);
// 		}
// 
// 		//가드 불가능 공격일 경우
// 		else
// 		{
// 			//필살기 첫번째 공격일 경우
// 			if (MyBoss->AnimInstance->Montage_IsPlaying(MyBoss->AM_SPAttack))
// 			{
// 				MyBoss->IsSPFirstATKHit = true;
// 			}
// 
// 			//무조건 사용자 정의 데미지 이벤트 생성
// 			MyBoss->HitData->HitDatas[MyBoss->SpawnWeapon->HitNumber].SendDamage(MyBoss, MyBoss->SpawnWeapon, Player);
// 		}
// 	}
// 
// 	Destroy();
}

void ACRangeAttack::HitPlayer()
{
	if (MyBoss)
	{
		//가드가 가능한 공격일 경우
		if (MyBoss->SpawnWeapon->IsGuard)
		{
			UCWeaponComponent* Weapon = CHelpers::GetComponent<UCWeaponComponent>(MyBoss->Target);

			//만약 플레이어가 패링 감지중이면
			if (Weapon->GetDoAction() && Weapon->GetDoAction()->RetrunParry())
			{
				return;
			}

			// 사용자 정의 데미지 이벤트 생성
			MyBoss->HitData->HitDatas[MyBoss->SpawnWeapon->HitNumber].SendDamage(MyBoss, MyBoss->SpawnWeapon, MyBoss->Target);
		}

		//가드 불가능 공격일 경우
		else
		{
			//무조건 사용자 정의 데미지 이벤트 생성
			MyBoss->HitData->HitDatas[MyBoss->SpawnWeapon->HitNumber].SendDamage(MyBoss, MyBoss->SpawnWeapon, MyBoss->Target);
		}
	}

	Destroy();
}

// void ACRangeAttack::SetActive(bool Value, FVector DirectionToTarget)
// {
// 	//true이면 모습이 보이고 콜리전 활성화, 방향도 주기
// 	if ( Value )
// 	{
// 		BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
// 		Direction = DirectionToTarget;
// 
// 		//방향을 가지고 해당 방향을 바라보도록 회전함
// 		FRotator TargetRotation = Direction.Rotation();
// 		SetActorRotation(TargetRotation);
// 	}
// 	//false면 모습이 안보이고 콜리전 비활성화
// 	else
// 	{
// 		BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
// 	}
// 
// 	MeshComp->SetVisibility(Value);
// }



