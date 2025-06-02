// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/RangedAttackObject/CRangeAttack.h"
#include "Components/BoxComponent.h"

// Sets default values
ACRangeAttack::ACRangeAttack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(L"BoxCollision");
	SetRootComponent(BoxComp);
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACRangeAttack::OverlapOther);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(L"MeshComp");
	MeshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACRangeAttack::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACRangeAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//현재 가시성이 활성화 된 상태에서만 실행됨
	if ( MeshComp->IsVisible())
	{
		//해당 방향을 향해 계속 앞으로 날아가게 만들어야 함
		SetActorLocation(GetActorLocation() + (Direction * ThrowSpeed) * DeltaTime);

		//그리고 생존 시간을 체크
		CurLifeTime +=DeltaTime;
		
		//만약 현재 생존시간이 제한 생존시간을 넘을 경우
		if ( CurLifeTime >= LimitLifeTime )
		{
			//자신을 안보이게 하고 다시 오브젝트 풀로 되돌림
			SetActive(false, FVector(0));
		}
	}
}

void ACRangeAttack::SetActive(bool Value, FVector DirectionToTarget)
{
	//true이면 모습이 보이고 콜리전 활성화, 방향도 주기
	if ( Value )
	{
		BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Direction = DirectionToTarget;

		//방향을 가지고 해당 방향을 바라보도록 회전함
		FRotator TargetRotation = Direction.Rotation();
		SetActorRotation(TargetRotation);
	}
	//false면 모습이 안보이고 콜리전 비활성화
	else
	{
		BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	MeshComp->SetVisibility(Value);
}

void ACRangeAttack::OverlapOther ( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{
	
	
	//자신을 안보이게 하고 다시 오브젝트 풀로 되돌림
	SetActive(false, FVector(0));
}

