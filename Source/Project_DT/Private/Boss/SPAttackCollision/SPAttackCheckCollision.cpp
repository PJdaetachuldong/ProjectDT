// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/SPAttackCollision/SPAttackCheckCollision.h"
#include "Components/SphereComponent.h"

// Sets default values
ASPAttackCheckCollision::ASPAttackCheckCollision()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(L"Collision");
	SetRootComponent(Collision);
	Collision->SetSphereRadius(300.0f);
	Collision->SetCollisionProfileName(L"BossWeapon");
}

// Called when the game starts or when spawned
void ASPAttackCheckCollision::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASPAttackCheckCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASPAttackCheckCollision::SetActive(bool Value, FVector SpawnLocation)
{
	//true이면 콜리전 활성화에 위치 재설정
	if (Value)
	{
		Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		//받아온 위치에 소환
		SetActorLocation(SpawnLocation);

		DrawDebugSphere(
			GetWorld(),
			GetActorLocation(),
			Collision->GetScaledSphereRadius(),
			32, // 세그먼트 수 (구체의 부드러움)
			FColor::Black,
			false, // 지속 여부
			20.0f // 지속 시간 (0이면 매 프레임 갱신)
		);
	}
	//false면 콜리전 비활성화
	else
	{
		Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

