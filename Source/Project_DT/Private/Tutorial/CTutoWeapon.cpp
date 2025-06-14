// Fill out your copyright notice in the Description page of Project Settings.


#include "Tutorial/CTutoWeapon.h"
#include "Components/BoxComponent.h"
#include "Character/CPlayer.h"
#include "Tutorial/CTutoAnim.h"

// Sets default values
ACTutoWeapon::ACTutoWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponCollComp = CreateDefaultSubobject<UBoxComponent>(L"WeaponCollision");
	WeaponCollComp->SetBoxExtent(FVector(30));
	WeaponCollComp->OnComponentBeginOverlap.AddDynamic(this, &ACTutoWeapon::TutoWeaponOverlap);
	WeaponCollComp->SetCollisionProfileName(FName("BossWeapon"));
}

// Called when the game starts or when spawned
void ACTutoWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponCollComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called every frame
void ACTutoWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACTutoWeapon::PlayParringAnim()
{
	//현재 재생중인 몽타주 멈춤
	MyOwner->AnimInstance->StopAllMontages(0.0f);
	
	IsTutoPlayerParring = true;
}

void ACTutoWeapon::SetTutoOwner(ACTutorialEnemy* WHO)
{
	MyOwner = WHO;
}

void ACTutoWeapon::TutoPlayerDamage()
{
	if (IsTutoPlayerParring)
	{
		IsTutoPlayerParring = false;

		return;
	}

	// 사용자 정의 데미지 이벤트 생성
	MyOwner->TutoHitData->HitDatas[TutoHitNumber].SendDamage(MyOwner, this, MyOwner->Target);
}

void ACTutoWeapon::TutoWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPlayer* Player = Cast<ACPlayer>(OtherActor);

	if (Player)
	{
		//가드가 가능한 공격일 경우
		if (MyOwner->IsParry)
		{
/*			GetWorld()->GetTimerManager().SetTimer(ParringCheckTimer, this, &ACBossWeapon::PlayerDamage, 0.1f, false);*/

			GetWorld()->GetTimerManager().SetTimer(TutoParringCheckTimer, this, &ACTutoWeapon::TutoPlayerDamage, 0.1f, false);
		}

		else if(MyOwner->IsJustVoid)
		{
			//무조건 사용자 정의 데미지 이벤트 생성
			MyOwner->TutoHitData->HitDatas[TutoHitNumber].SendDamage(MyOwner, this, Player);
		}
	}
}

