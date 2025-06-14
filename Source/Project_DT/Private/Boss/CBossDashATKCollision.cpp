// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/CBossDashATKCollision.h"
#include "Character/CPlayer.h"
#include "Components/BoxComponent.h"
#include "Boss/CBossWeapon.h"

// Sets default values
ACBossDashATKCollision::ACBossDashATKCollision()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DashATKCollision = CreateDefaultSubobject<UBoxComponent>(L"DashColli");
	SetRootComponent(DashATKCollision);
	DashATKCollision->SetCollisionProfileName(L"BossWeapon");
	DashATKCollision->OnComponentBeginOverlap.AddDynamic(this, &ACBossDashATKCollision::DashPlayerHit);
	/*DashATKCollision->SetRelativeLocation(FVector(0, 59, 100));*/
	DashATKCollision->SetBoxExtent(FVector(41, 29, 80));
}

// Called when the game starts or when spawned
void ACBossDashATKCollision::BeginPlay()
{
	Super::BeginPlay();

	DashATKCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called every frame
void ACBossDashATKCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
// 	ACPlayer* Player = Cast<ACPlayer>(OtherActor);
// 
// 	if (Player)
// 	{
// 		MyBoss->SpawnWeapon->DashAttackHitCheck();
// 	}
}

void ACBossDashATKCollision::GetMyBoss(ACBossEnemy* Boss)
{
	MyBoss = Boss;
}

void ACBossDashATKCollision::OnDashCollision()
{
	if (DashATKCollision)
	{
		DashATKCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void ACBossDashATKCollision::OffDashCollision()
{
	if (DashATKCollision)
	{
		DashATKCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ACBossDashATKCollision::DashPlayerHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OtherActor Class: %s"), *GetNameSafe(OtherActor));

	ACPlayer* Player = Cast<ACPlayer>(OtherActor);

	if (Player)
	{
		MyBoss->SpawnWeapon->DashAttackHitCheck();
	}
}

