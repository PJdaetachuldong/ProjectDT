// Fill out your copyright notice in the Description page of Project Settings.


#include "Tutorial/CTutoManager.h"
#include "Components/BoxComponent.h"
#include "Character/CPlayer.h"
#include "Tutorial/CTutorialEnemy.h"
#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LHW_GameModeBase.h"
#include "Widget/CTutorialWidget.h"

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

	FirstTransform = CreateDefaultSubobject<USceneComponent>(L"FirstTransform");
	FirstTransform->SetupAttachment(SceneComp);

	SecondTransform = CreateDefaultSubobject<USceneComponent>(L"SecondTransform");
	SecondTransform->SetupAttachment(SceneComp);

	ThirdTransform = CreateDefaultSubobject<USceneComponent>(L"ThirdTransform");
	ThirdTransform->SetupAttachment(SceneComp);
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
			//스폰할 객체에 대한 스폰 옵션을 설정하는 구조체
			FActorSpawnParameters Params;
			//스폰 과정에 충돌이 생겨도 제자리에서 스폰할 수 있게 만듦
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			SpawnTutoEnemy1 = GetWorld()->SpawnActor<ACTutorialEnemy>(TutoEnemy, FirstTransform->GetComponentTransform(), Params);

			AIController->Possess(SpawnTutoEnemy1);

			SpawnTutoEnemy1->IsATKGide = true;
			SpawnTutoEnemy1->IsDontHit = false;
			SpawnTutoEnemy1->SettingManager(this);

			ALHW_GameModeBase* GameMode = Cast<ALHW_GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

			if (GameMode)
			{
				GameMode->TutorialWidget->SetSwitcherIndex(4);
			}
		}

		if (IsFirstTutoEnemyDIE && !SpawnTutoEnemy2)
		{
			CurSpawnTime += DeltaTime;

			if (CurSpawnTime >= EnemySpawnDelay)
			{
				SpawnTutoEnemy1->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				SpawnTutoEnemy1->GetMesh()->SetVisibility(false);

				SpawnTutoEnemy1->SetActorLocation(FVector(0, -200, 0));

				//스폰할 객체에 대한 스폰 옵션을 설정하는 구조체
				FActorSpawnParameters Params;
				//스폰 과정에 충돌이 생겨도 제자리에서 스폰할 수 있게 만듦
				Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				SpawnTutoEnemy2 = GetWorld()->SpawnActor<ACTutorialEnemy>(TutoEnemy, SecondTransform->GetComponentTransform(),Params);

				AIController->Possess(SpawnTutoEnemy2);

				SpawnTutoEnemy2->IsParry = true;
				SpawnTutoEnemy2->SettingManager(this);

				CurSpawnTime = 0.0f;
			}
		}

		if (IsFirstTutoEnemyDIE && IsSecondTutoEnemyDIE && !SpawnTutoEnemy3)
		{
			CurSpawnTime += DeltaTime;

			if (CurSpawnTime >= EnemySpawnDelay)
			{
				SpawnTutoEnemy2->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				SpawnTutoEnemy2->GetMesh()->SetVisibility(false);

				SpawnTutoEnemy2->SetActorLocation(FVector(0, -200, 0));

				//스폰할 객체에 대한 스폰 옵션을 설정하는 구조체
				FActorSpawnParameters Params;
				//스폰 과정에 충돌이 생겨도 제자리에서 스폰할 수 있게 만듦
				Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				SpawnTutoEnemy3 = GetWorld()->SpawnActor<ACTutorialEnemy>(TutoEnemy, ThirdTransform->GetComponentTransform(), Params);

				AIController->Possess(SpawnTutoEnemy3);

				SpawnTutoEnemy3->IsParry = false;

				SpawnTutoEnemy3->SettingManager(this);

				IsAllSpawn = true;
			}
		}
	}
}

void ACTutoManager::InputTutoEnemyDIE()
{
	if(IsFirstTutoEnemyDIE && !IsSecondTutoEnemyDIE) IsSecondTutoEnemyDIE = true;
	
	if(!IsFirstTutoEnemyDIE)
	{
		IsFirstTutoEnemyDIE = true;
		ALHW_GameModeBase* GameMode = Cast<ALHW_GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

		if (GameMode)
		{
			GameMode->TutorialWidget->SetSwitcherIndex(0);
		}
	}
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

