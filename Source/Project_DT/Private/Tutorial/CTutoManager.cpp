// Fill out your copyright notice in the Description page of Project Settings.


#include "Tutorial/CTutoManager.h"
#include "Components/BoxComponent.h"
#include "Character/CPlayer.h"
#include "Tutorial/CTutorialEnemy.h"
#include "AIController.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ACTutoManager::ACTutoManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(L"SceneComp");
	SetRootComponent(SceneComp);

	StartCollision = CreateDefaultSubobject<UBoxComponent>(L"StartColli");
	StartCollision->SetupAttachment(SceneComp);
	StartCollision->SetCollisionProfileName(L"BossWeapon");
	StartCollision->OnComponentBeginOverlap.AddDynamic(this, &ACTutoManager::SpawnStart);
}

// Called when the game starts or when spawned
void ACTutoManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACTutoManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsAllSpawn) return;

	if (IsTutoStart)
	{
		if (!SpawnTutoEnemy1)
		{
			SpawnTutoEnemy1 = GetWorld()->SpawnActor<ACTutorialEnemy>(TutoEnemy, GetActorTransform());

			AIController->Possess(SpawnTutoEnemy1);

			SpawnTutoEnemy1->IsParry = true;
			SpawnTutoEnemy1->SettingManager(this);
		}

		if (IsFirstTutoEnemyDIE)
		{
			CurSpawnTime += DeltaTime;

			if (CurSpawnTime >= EnemySpawnDelay && !SpawnTutoEnemy2)
			{
				SpawnTutoEnemy1->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				SpawnTutoEnemy1->GetMesh()->SetVisibility(false);

				CurSpawnTime = 0.0f;
				
				SpawnTutoEnemy2 = GetWorld()->SpawnActor<ACTutorialEnemy>(TutoEnemy, GetActorTransform());

				AIController->Possess(SpawnTutoEnemy2);

				SpawnTutoEnemy2->IsParry = false;

				/*SpawnTutoEnemy2->SettingManager(this);*/

				IsAllSpawn = true;
			}
		}
	}
}

void ACTutoManager::InputTutoEnemyDIE()
{
	IsFirstTutoEnemyDIE = true;
}

void ACTutoManager::SpawnStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPlayer* Player = Cast<ACPlayer>(OtherActor);

	if (Player)
	{
		IsTutoStart = true;

		AIController = GetWorld()->SpawnActor<AAIController>(AAIController::StaticClass());

		StartCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

