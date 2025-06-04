// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CParryComponent.h"
#include "Global.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CWeaponComponent.h"
#include "Boss/CBossWeapon.h"
// Sets default values for this component's properties
UCParryComponent::UCParryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCParryComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter> ( GetOwner ( ) );
	Weapon = CHelpers::GetComponent<UCWeaponComponent> ( GetOwner ( ) );
}


void UCParryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if ( bIsParrying )
		DetectActor ( );
}

void UCParryComponent::DetectActor ( )
{
	if ( !GetOwner ( ) ) return;

	FVector Location = GetOwner ( )->GetActorLocation ( );
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor ( GetOwner ( ) );

	for ( AActor* Ignored : TemporarilyIgnoredActors )
		Params.AddIgnoredActor ( Ignored );

	FVector HalfExtent = FVector ( 50.f , 50.f , 100.f );
	FCollisionShape BoxShape = FCollisionShape::MakeBox ( HalfExtent );

	bool bHit = GetWorld ( )->SweepSingleByChannel (
		Hit ,
		Location ,
		Location ,
		FQuat::Identity ,
		ECC_GameTraceChannel3 ,
		BoxShape ,
		Params
	);

	DrawDebugBox ( GetWorld ( ) , Location , HalfExtent , FQuat::Identity , FColor::Green , false , 0.1f );

	AActor* HitActor = Hit.GetActor ( );
	if ( bHit && HitActor && HitActor->IsA ( AActor::StaticClass ( ) ) )
	{
		if ( TemporarilyIgnoredActors.Contains ( HitActor ) )
			return;

		if ( !IsInDetectionAngle ( HitActor ) )
			return;

		CLog::Log ( "Hit" );

		DrawDebugSphere ( GetWorld ( ) , Hit.ImpactPoint , 20.f , 12 , FColor::Red , false , 1.0f );
		DrawDebugLine ( GetWorld ( ) , Location , HitActor->GetActorLocation ( ) , FColor::Blue , false , 1.0f , 0 , 2.0f );

		EParryState Quadrant = GetHitQuadrant ( Hit.ImpactPoint );

		HandleTemporaryIgnore ( HitActor );
		Weapon->OnParry ( Quadrant );
	}
}

bool UCParryComponent::IsInDetectionAngle ( AActor* TargetActor ) const
{
	FVector ToTarget = ( TargetActor->GetActorLocation ( ) - GetOwner ( )->GetActorLocation ( ) ).GetSafeNormal2D ( );
	float Angle = FMath::RadiansToDegrees ( FMath::Acos ( FVector::DotProduct ( GetOwner ( )->GetActorForwardVector ( ) , ToTarget ) ) );
	return Angle <= 65.f;
}

EParryState UCParryComponent::GetHitQuadrant ( const FVector& ImpactPoint ) const
{
	FVector LocalHit = UKismetMathLibrary::InverseTransformLocation ( GetOwner ( )->GetActorTransform ( ) , ImpactPoint );

	if ( LocalHit.X >= 0 )
		return ( LocalHit.Y >= 0 ) ? EParryState::TL : EParryState::TR;
	else
		return ( LocalHit.Y >= 0 ) ? EParryState::BL : EParryState::BR;

}

void UCParryComponent::HandleTemporaryIgnore ( AActor* ActorToIgnore )
{
	TemporarilyIgnoredActors.Add ( ActorToIgnore );

	FTimerHandle TimerHandle;
	GetWorld ( )->GetTimerManager ( ).SetTimer ( TimerHandle , [this , ActorToIgnore]( )
	{
		TemporarilyIgnoredActors.Empty();
	} , 0.2f , false );
}

void UCParryComponent::OnGuard()
{
	if (bIsGuarding)
		return;

	bIsGuarding = true;

	// 반복적인 스피어 트레이스 시작
	GetWorld()->GetTimerManager().SetTimer(
		GuardTraceTimer,
		this,
		&UCParryComponent::PerformGuardTrace,
		GuardTraceInterval,
		true
	);
}

void UCParryComponent::OffGuard()
{
	if (!bIsGuarding)
		return;

	bIsGuarding = false;

	// 타이머 종료
	GetWorld()->GetTimerManager().ClearTimer(GuardTraceTimer);
}

void UCParryComponent::PerformGuardTrace()
{
	AActor* Owner = GetOwner();
	if (!Owner)
		return;

	FVector Start = Owner->GetActorLocation() + FVector(0, 0, 0); // 높이 보정
	FVector Forward = Owner->GetActorForwardVector();
	FVector End = Start + Forward;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(TraceRadius),
		Params
	);

#if WITH_EDITOR
	// 시각화
	DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 0.1f);
	DrawDebugSphere(GetWorld(), End, TraceRadius, 16, bHit ? FColor::Green : FColor::Red, false, 0.1f);
#endif

	if (bHit)
	{
		// 공격 감지 시 처리
		AActor* HitActor = HitResult.GetActor();
		if (HitActor->IsA(ACBossWeapon::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Guarded against: %s"), *HitActor->GetName());

			// 여기에 블로킹 효과, 이펙트, 사운드, 스태미나 소비 등 추가
		}
	}
}
