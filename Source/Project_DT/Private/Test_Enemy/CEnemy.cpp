// Fill out your copyright notice in the Description page of Project Settings.


#include "Test_Enemy/CEnemy.h"
#include "Global.h"
#include "Component/CMointageComponent.h"
#include "Component/CStateComponent.h"
#include "Character/CPlayerAnim.h"
#include "Component/CMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/CWeaponStuctures.h"
#include "GameFramework/Character.h"
#include "Weapons/CAttachment.h"
#include "Component/CParryComponent.h"
// Sets default values
ACEnemy::ACEnemy ( )
{
	CHelpers::CreateActorComponent<UCMointageComponent> ( this , &Montages , "Montages" );
	//CHelpers::CreateActorComponent<UCMovementComponent> ( this , &Movement , "Movement" );
	CHelpers::CreateActorComponent<UCStateComponent> ( this , &State , "State" );
	CHelpers::CreateActorComponent<UCWeaponComponent> ( this , &Weapon , "Weapon" );
	//CHelpers::CreateActorComponent<UCStatusComponent> ( this , &Status , "Status" );


	GetMesh ( )->SetRelativeLocation ( FVector ( 0 , 0 , -90 ) );
	GetMesh ( )->SetRelativeRotation ( FRotator ( 0 , -90 , 0 ) );

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh> ( &mesh , AssetPaths::CHARACTER_MESH );
	GetMesh ( )->SetSkeletalMesh ( mesh );

	TSubclassOf<UAnimInstance> Anim;

	CHelpers::GetClass<UAnimInstance> ( &Anim , AssetPaths::CHARACTER_ANIM );
	GetMesh ( )->SetAnimClass ( Anim );

	GetCharacterMovement ( )->RotationRate = FRotator ( 0 , 720 , 0 );
	PrimaryActorTick.bCanEverTick = true;
}

void ACEnemy::BeginPlay ( )
{
	Super::BeginPlay ( );

	//Movement->OnWalk ( );

	Create_DynamicMeterical ( this );
	Change_Color ( this , OriginColor );

	State->OnStateTypeChanged.AddDynamic ( this , &ACEnemy::OnStateTypeChanged );
	Weapon->SetKatanaMode();

}

void ACEnemy::Tick ( float DeltaTime )
{
	Super::Tick ( DeltaTime );
	OverlapBegin();

}

void ACEnemy::OnStateTypeChanged ( EStateType InPrevType , EStateType InNewType )
{
	switch ( InNewType )
	{
	case EStateType::Hitted: Hitted ( ); break;
	}
}


float ACEnemy::TakeDamage ( float DamageAmount , struct FDamageEvent const& DamageEvent , class AController* EventInstigator , AActor* DamageCauser )
{
	float damage = Super::TakeDamage ( DamageAmount , DamageEvent , EventInstigator , DamageCauser );
	Damage.Power = damage;
	Damage.Character = Cast<ACharacter> ( EventInstigator->GetPawn ( ) );
	Damage.Causer = DamageCauser;
	Damage.Event = (FActionDamageEvent*)&DamageEvent;
	CLog::Log ( Damage.Power );

	//State->SetHittedMode ( );
	Weapon->SetKatanaMode ( );

	return damage;
}

void ACEnemy::Hitted ( )
{
	Damage.Power = 0;

	{
		Change_Color ( this , FLinearColor::Red );

		FTimerDelegate timerDelegate;
		timerDelegate.BindUFunction ( this , "RestoreColor" );

		GetWorld ( )->GetTimerManager ( ).SetTimer ( RestoreColor_TimerHandle , timerDelegate , 0.2f , false );
	}

	if ( !!Damage.Event && !!Damage.Event->HitData ){
		FHitData* data = Damage.Event->HitData;
		data->PlayMontage ( this );
		data->PlayHitStop ( GetWorld ( ) );
		{
			FVector start = GetActorLocation();
			FVector target = Damage.Character->GetActorLocation();
			FVector direction = target - start;
			direction.Normalize();

			LaunchCharacter(-direction * data->Launch, false, false);
			//SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
		}
	}
	Damage.Character = nullptr;
	Damage.Causer = nullptr;
	Damage.Event = nullptr;
}

void ACEnemy::OverlapBegin ( )
{
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor ( this );

	// 일시적으로 무시 중인 액터는 전부 무시
	for ( AActor* Ignored : TemporarilyIgnoredActors )
	{
		Params.AddIgnoredActor ( Ignored );
	}

	FVector HalfExtent = FVector ( 50.f , 50.f , 100.f );
	FCollisionShape BoxShape = FCollisionShape::MakeBox ( HalfExtent );

	bool bHit = GetWorld ( )->SweepSingleByChannel (
		Hit ,
		GetActorLocation ( ) ,
		GetActorLocation ( ) ,
		FQuat::Identity ,
		ECC_GameTraceChannel3 ,
		BoxShape ,
		Params
	);

	DrawDebugBox ( GetWorld ( ) , GetActorLocation ( ) , HalfExtent , FQuat::Identity , FColor::Green , false , 0.1f );

	if ( bHit && Hit.GetActor ( )->IsA ( ACAttachment::StaticClass ( ) ) )
	{
		// 무시 중이라면 건너뜀
		if ( TemporarilyIgnoredActors.Contains ( Hit.GetActor ( ) ) )
			return;

		FVector ToPlayer = ( Hit.GetActor ( )->GetActorLocation ( ) - GetActorLocation ( ) ).GetSafeNormal2D ( );
		float AngleDeg = FMath::RadiansToDegrees ( FMath::Acos ( FVector::DotProduct ( GetActorForwardVector ( ) , ToPlayer ) ) );

		if ( AngleDeg <= 65.0f )
		{
			CLog::Log ( "Hit" );

			DrawDebugSphere ( GetWorld ( ) , Hit.ImpactPoint , 20.0f , 12 , FColor::Red , false , 1.0f );
			DrawDebugLine ( GetWorld ( ) , GetActorLocation ( ) , Hit.GetActor ( )->GetActorLocation ( ) , FColor::Blue , false , 1.0f , 0 , 2.0f );

			FVector LocalHit = UKismetMathLibrary::InverseTransformLocation ( GetActorTransform ( ) , Hit.ImpactPoint );

			EParryState Quadrant = EParryState::Max;

			if ( LocalHit.X >= 0 )
				Quadrant = ( LocalHit.Y >= 0 ) ? EParryState::TR : EParryState::TL;
			else
				Quadrant = ( LocalHit.Y >= 0 ) ? EParryState::BR : EParryState::BL;

			Weapon->OnParry ( Quadrant );



			// 잠깐 무시 처리
			AActor* HitActor = Hit.GetActor ( );
			TemporarilyIgnoredActors.Add ( HitActor );

			FTimerHandle TimerHandle;
			FTimerDelegate TimerDel;

			GetWorld ( )->GetTimerManager ( ).SetTimer ( TimerHandle , [&]( ){
			{
				TemporarilyIgnoredActors.Reset();
			}} , 0.5f , false );
		}
	}
}

void ACEnemy::RestoreColor ( )
{
	CLog::Log ( "RestoreColor" );

	Change_Color ( this , OriginColor );

	GetWorld ( )->GetTimerManager ( ).ClearTimer ( RestoreColor_TimerHandle );
}