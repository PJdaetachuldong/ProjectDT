// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/CBossManager.h"
#include "Components/BoxComponent.h"
#include "Character/CPlayer.h"
#include "Boss/CBossEnemy.h"

// Sets default values
ACBossManager::ACBossManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StartCollision = CreateDefaultSubobject<UBoxComponent>(L"StartColli");
	SetRootComponent(StartCollision);
	StartCollision->SetCollisionProfileName(L"BossWeapon");
	StartCollision->OnComponentBeginOverlap.AddDynamic(this, &ACBossManager::Start);
}

// Called when the game starts or when spawned
void ACBossManager::BeginPlay()
{
	Super::BeginPlay();


}

void ACBossManager::Start(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
// 	ACharacter* Chara = Cast<ACharacter>(OtherActor);
// 
// 	if (Chara)
// 	{
// 		ACPlayer* Player = Cast<ACPlayer>(OtherActor);
// 
// 		if (Player)
// 		{
// 		
// 			if (Boss)
// 			{
// 				Boss->OnPlayerOverlap();
// 			}
// 			
// 			StartCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
// 		}
// 
// 		else
// 		{
// 
// 		}
// 	}
}

