// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CAttachment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/ShapeComponent.h"
#include "Components/SceneComponent.h"
#include "Enemy/EnemyBase/CEnemyBase.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"
#include "Component/CStatusComponent.h"
// Sets default values
ACAttachment::ACAttachment()
{
	CHelpers::CreateComponent ( this , &Root , "Root" );
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMesh, "SkeletalMesh");
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACAttachment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter> ( GetOwner ( ) );

	TArray<USceneComponent*> children;
	Root->GetChildrenComponents ( true , children );
	for ( USceneComponent* child : children )
	{
		UShapeComponent* shape = Cast<UShapeComponent> ( child );

		if ( !!shape )
		{
			shape->OnComponentBeginOverlap.AddDynamic ( this , &ACAttachment::OnComponentBeginOverlap );
			shape->OnComponentEndOverlap.AddDynamic ( this , &ACAttachment::OnComponentEndOverlap );

			Collisions.Add ( shape );
		}
		OffCollisions ( );
	}

	Super::BeginPlay ( );

}

void ACAttachment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bCollisionTraceEnabled)
		return;


	// 1. 이전 프레임 위치 저장
	PrevStartLocation = CurrentStartLocation;
	PrevEndLocation = CurrentEndLocation;

	// 2. 소켓 기준으로 현재 위치 가져오기
	if(SkeletalMesh)
	{
		CurrentStartLocation = SkeletalMesh->GetSocketLocation("Bottom"); // 손잡이 쪽
		CurrentEndLocation = SkeletalMesh->GetSocketLocation("Top");      // 칼날 끝
	}
	if (!bInitialized)
	{
		PrevStartLocation = CurrentStartLocation;
		PrevEndLocation = CurrentEndLocation;
		bInitialized = true;
		return;
	}

	// 3. 중간 지점 계산
	FVector PrevMiddleLocation = (PrevStartLocation + PrevEndLocation) * 0.5f;
	FVector CurrentMiddleLocation = (CurrentStartLocation + CurrentEndLocation) * 0.5f;

	// 3. 히트 결과 초기화
	HitResults.Reset();

	// 4. 거리 기반 보간 단계 계산
	float InterpolationStep = 5.0f;
	float Distance = FVector::Dist(PrevStartLocation, CurrentStartLocation);
	int32 NumSteps = FMath::CeilToInt(Distance / InterpolationStep);
	NumSteps = FMath::Max(NumSteps, 1); // 최소 1 보장

	// 5. 제어점 계산 (조금 위로 올려 부드럽게)
	FVector ControlPointStart = (PrevStartLocation + CurrentStartLocation) * 0.5f + FVector(0, 0, 20.f);
	FVector ControlPointEnd = (PrevEndLocation + CurrentEndLocation) * 0.5f + FVector(0, 0, 20.f);

	// 6. 베지어 커브를 통한 삼각형 트레이스 반복
	for (int32 i = 0; i < NumSteps; ++i)
	{
		float Alpha1 = (float)i / NumSteps;
		float Alpha2 = (float)(i + 1) / NumSteps;

		FVector StartA = BezierCurve(PrevStartLocation, ControlPointStart, CurrentStartLocation, Alpha1);
		FVector StartB = BezierCurve(PrevStartLocation, ControlPointStart, CurrentStartLocation, Alpha2);

		FVector EndA = BezierCurve(PrevEndLocation, ControlPointEnd, CurrentEndLocation, Alpha1);
		FVector EndB = BezierCurve(PrevEndLocation, ControlPointEnd, CurrentEndLocation, Alpha2);

		FVector MidA = (StartA + EndA) * 0.5f;
		FVector MidB = (StartB + EndB) * 0.5f;

		PerformTriangleTrace(StartA, MidA, EndA, HitResults);
		PerformTriangleTrace(StartB, MidB, EndB, HitResults);
	}

	// 7. 히트 처리
	for (const FHitResult& Hit : HitResults)
	{
		if (Hit.GetActor()->IsA(ACEnemyBase::StaticClass()))
		{
			ACEnemyBase* Enemy = Cast<ACEnemyBase>(Hit.GetActor());
			if (EnemyActor == Enemy)return;
			EnemyActor = Enemy;
			Enemy->Hit(GetName());
			GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, FString::Printf(TEXT("Hit: %s"), *Hit.GetActor()->GetName()));
			Status=CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);
			Status->RecoverMana(5);
			if (OnAttachmentBeginOverlap.IsBound())
				OnAttachmentBeginOverlap.Broadcast(OwnerCharacter, this, Cast<ACharacter>(Hit.GetActor()));
			if (HitEffect) // HitEffect가 블루프린트에서 설정되었는지 확인
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					GetWorld(),
					HitEffect,
					Hit.ImpactPoint, // 충돌 지점
					Hit.ImpactNormal.Rotation(), // 충돌 노멀 기반 회전
					FVector(1.0f), // 스케일
					true, // AutoDestroy
					true  // AutoActivate
				);
			}
		}
	}
	// DrawDebugBox(GetWorld(), CurrentStartLocation, FVector(2.f), FColor::Blue, false, 0.1f);
	// DrawDebugBox(GetWorld(), CurrentEndLocation, FVector(2.f), FColor::Red, false, 0.1f);
	// DrawDebugBox(GetWorld(), PrevStartLocation, FVector(2.f), FColor::Cyan, false, 0.1f);
	// DrawDebugBox(GetWorld(), PrevEndLocation, FVector(2.f), FColor::Magenta, false, 0.1f);
}

void ACAttachment::AttachTo ( FName InSocketName )
{
	AttachToComponent ( OwnerCharacter->GetMesh ( ) , FAttachmentTransformRules ( EAttachmentRule::KeepRelative , true ) , InSocketName );
}

void ACAttachment::OnCollisions ( )
{
	if ( OnAttachmentBeginCollision.IsBound ( ) )
		OnAttachmentBeginCollision.Broadcast ( );

	bCollisionTraceEnabled = true;
	EnemyActor = nullptr;

}

void ACAttachment::OffCollisions ( )
{
	if ( OnAttachmentEndCollision.IsBound ( ) )
		OnAttachmentEndCollision.Broadcast ( );

	bCollisionTraceEnabled = false;

	// 초기화
	PrevStartLocation = FVector::ZeroVector;
	PrevEndLocation = FVector::ZeroVector;
	CurrentStartLocation = FVector::ZeroVector;
	CurrentEndLocation = FVector::ZeroVector;
	bInitialized = false;
}

void ACAttachment::OnComponentBeginOverlap ( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{
	CheckTrue ( OwnerCharacter == OtherActor );
	CheckTrue ( OwnerCharacter->GetClass ( ) == OtherActor->GetClass ( ) );

}

void ACAttachment::OnComponentEndOverlap ( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex )
{
	CheckTrue ( OwnerCharacter == OtherActor );
	CheckTrue ( OwnerCharacter->GetClass ( ) == OtherActor->GetClass ( ) );


	if ( OnAttachmentEndOverlap.IsBound ( ) )
		OnAttachmentEndOverlap.Broadcast ( OwnerCharacter, Cast<ACharacter> ( OtherActor ) );
}
void ACAttachment::PerformTriangleTrace(
	const FVector& A,
	const FVector& B,
	const FVector& C,
	TArray<FHitResult>& OutHits)
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);
	Params.bTraceComplex = true;

	UWorld* World = GetWorld();
	if (!World) return;

	// A to B
	FHitResult Hit1;
	if (World->LineTraceSingleByChannel(Hit1, A, B, ECC_EngineTraceChannel5, Params))
	{
		OutHits.Add(Hit1);
		// DrawDebugLine(World, A, B, FColor::Red, false, 0.1f, 0, 2.f);
	}
	else
	{
		// DrawDebugLine(World, A, B, FColor::Green, false, 0.1f, 0, 1.f);
	}

	// B to C
	FHitResult Hit2;
	if (World->LineTraceSingleByChannel(Hit2, B, C, ECC_EngineTraceChannel5, Params))
	{
		OutHits.Add(Hit2);
		// DrawDebugLine(World, B, C, FColor::Red, false, 0.1f, 0, 2.f);
	}
	else
	{
		// DrawDebugLine(World, B, C, FColor::Green, false, 0.1f, 0, 1.f);
	}

	// C to A
	FHitResult Hit3;
	if (World->LineTraceSingleByChannel(Hit3, C, A, ECC_EngineTraceChannel5, Params))
	{
		OutHits.Add(Hit3);
		// DrawDebugLine(World, C, A, FColor::Red, false, 0.1f, 0, 2.f);
	}
	else
	{
		// DrawDebugLine(World, C, A, FColor::Green, false, 0.1f, 0, 1.f);
	}
}
FVector ACAttachment::BezierCurve(const FVector& P0, const FVector& P1, const FVector& P2, float T)
{
	return FMath::Pow(1 - T, 2) * P0 + 2 * (1 - T) * T * P1 + FMath::Pow(T, 2) * P2;
}