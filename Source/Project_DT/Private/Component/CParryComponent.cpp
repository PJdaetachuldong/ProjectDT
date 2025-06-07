// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CParryComponent.h"
#include "Global.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CWeaponComponent.h"
#include "Boss/CBossWeapon.h"
#include "Component/CStateComponent.h"
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
	State = CHelpers::GetComponent<UCStateComponent> ( GetOwner ( ) );
}


void UCParryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if ( bIsParrying )
		DetectActor ( );
}

void UCParryComponent::DetectActor()
{
	if (!GetOwner()) return;

	FVector Start = GetOwner()->GetActorLocation();
	FVector Forward = GetOwner()->GetActorForwardVector();
	FVector End = Start + Forward;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	for (AActor* Ignored : TemporarilyIgnoredActors)
		Params.AddIgnoredActor(Ignored);


	bool bHit = GetWorld()->SweepSingleByChannel(
		Hit,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(TraceRadius+10.0f),
		Params
	);

// #if WITH_EDITOR
// 	// 시각화
// 	DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 0.1f, 0, 1.5f);
// 	DrawDebugSphere(GetWorld(), End, TraceRadius+10.0f, 16, FColor::Green, false, 0.1f);
// #endif

	AActor* HitActor = Hit.GetActor();
	if (bHit && HitActor && HitActor->IsA(ACBossWeapon::StaticClass()))
	{
		if (TemporarilyIgnoredActors.Contains(HitActor))
			return;
		ACBossWeapon* Enemy = Cast<ACBossWeapon>(HitActor);
		if (Enemy)
			if (!Enemy->CheckGuardBool()) {
				return;
			}

		// DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 20.f, 12, FColor::Red, false, 1.0f);
		// DrawDebugLine(GetWorld(), Start, HitActor->GetActorLocation(), FColor::Blue, false, 1.0f, 0, 2.0f);

		EParryState Quadrant = GetHitQuadrant(Hit.ImpactPoint);

		HandleTemporaryIgnore(HitActor);

		bIsGuarding = false;
		GetWorld()->GetTimerManager().ClearTimer(GuardTraceTimer);
		Weapon->OnParry(Quadrant);
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
	//if (bIsGuarding)return;
	OnParryCollision();

// 5프레임 뒤에 콜리전 끄기 (60FPS 기준 약 0.083초)
FTimerHandle TimerHandle;
FTimerDelegate TimerDelegate;

// 람다 캡처 방식 또는 바인딩 방식 사용 가능
TimerDelegate.BindLambda([this]()
{
	OffParryCollision();
});

GetWorld()->GetTimerManager().SetTimer(
	TimerHandle,
	TimerDelegate,
	5.0f / 60.0f, // 약 0.083초
	false // 반복 안 함
);
FTimerHandle PH;

//bCanParry = false;
//GetWorld ( )->GetTimerManager ( ).SetTimer (
//	PH,
//	[this]( ){bCanParry = true; } ,
//	0.2f , // 쿨타임 0.3초 (원하는 시간으로)
//	false
//);
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

// #if WITH_EDITOR
// 	// 시각화
// 	DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 0.1f);
// 	DrawDebugSphere(GetWorld(), End, TraceRadius, 16, bHit ? FColor::Green : FColor::Red, false, 0.1f);
// #endif

	if (bHit)
	{
		// 공격 감지 시 처리
		AActor* HitActor = HitResult.GetActor();
		if (HitActor->IsA(ACBossWeapon::StaticClass()))
		{
			auto* AnimInstance = Cast<UAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
			UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
			if (CurrentMontage) {
				AnimInstance->Montage_JumpToSection(FName("Hit"), CurrentMontage);
			}
		}
	}
}