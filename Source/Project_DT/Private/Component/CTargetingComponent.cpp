// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CTargetingComponent.h"
#include "Camera/CameraComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Weapons/CWeaponComponent.h"
#include "Engine/OverlapResult.h"
// Sets default values for this component's properties
UCTargetingComponent::UCTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UCTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter> ( GetOwner ( ) );
	Camera = CHelpers::GetComponent<UCameraComponent> ( OwnerCharacter );
	SpringArm = CHelpers::GetComponent<USpringArmComponent> ( OwnerCharacter );
	Weapon = CHelpers::GetComponent<UCWeaponComponent> ( OwnerCharacter );
}

void UCTargetingComponent::TickComponent ( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent ( DeltaTime , TickType , ThisTickFunction );
	currentTime += DeltaTime;
        OnLookOn ( );
    if ( currentTime > 1.0f ) {
		currentTime = 0.0f;
	}
    //if ( Weapon->GetWeaponType ( ) != EWeaponType::Max )
        UpdateLockOn ( DeltaTime );

}

AActor* UCTargetingComponent::FindClosestEnemyByDistance ( float MaxDistance )
{
    if ( !OwnerCharacter ) return nullptr;

    FVector CharacterLocation = OwnerCharacter->GetActorLocation ( );

    AActor* ClosestTarget = nullptr;
    float ClosestDistance = MaxDistance;

    TArray<FOverlapResult> Overlaps;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor ( OwnerCharacter );

    // 캐릭터 중심의 원형 탐색
    GetWorld ( )->OverlapMultiByObjectType (
        Overlaps ,
        CharacterLocation ,
        FQuat::Identity ,
        FCollisionObjectQueryParams ( ECollisionChannel::ECC_Pawn ) ,
        FCollisionShape::MakeSphere ( MaxDistance ) ,
        Params
    );

    for ( const FOverlapResult& Result : Overlaps )
    {
        AActor* Enemy = Result.GetActor ( );
        if ( !IsValid ( Enemy ) ) continue;

        float Distance = FVector::Dist ( CharacterLocation , Enemy->GetActorLocation ( ) );

        UE_LOG ( LogTemp , Log ,
            TEXT ( "[LockOn Check] Enemy: %s | Distance: %.1f" ) ,
            *Enemy->GetName ( ) , Distance
        );

        if ( Distance < ClosestDistance )
        {
            ClosestDistance = Distance;
            ClosestTarget = Enemy;
        }
    }

    if ( ClosestTarget )
    {
        DrawDebugSphere (
            GetWorld ( ) ,
            ClosestTarget->GetActorLocation ( ) ,
            60.f ,
            16 ,
            FColor::Red ,
            false , 0.1f , 0 , 3.f
        );

        UE_LOG ( LogTemp , Warning ,
            TEXT ( "[LockOn] Selected Target: %s (Distance: %.1f)" ) ,
            *ClosestTarget->GetName ( ) , ClosestDistance
        );
    }
    else
    {
        UE_LOG ( LogTemp , Warning , TEXT ( "[LockOn] No valid target found." ) );
    }

    return ClosestTarget;
}
void UCTargetingComponent::OnLookOn ( )
{
    AActor* Target = FindClosestEnemyByDistance ( 250.f );
    TimeSinceLostTarget += GetWorld()->DeltaTimeSeconds;
    if ( Target )
    {
        LockedOnTarget = Target;
        bIsLockedOn = true;
        UE_LOG ( LogTemp , Warning , TEXT ( "[LockOn] Locked on to: %s" ) , *Target->GetName ( ) );
    }
    else
    {
        if ( TimeSinceLostTarget >= TargetLossTimeout ){
        LockedOnTarget = nullptr;
        bIsLockedOn = false;
        UE_LOG ( LogTemp , Warning , TEXT ( "[LockOn] Lock-Off (No target found)" ) );
        }
    }
}

void UCTargetingComponent::UpdateLockOn ( float DeltaTime )
{
    if ( !bIsLockedOn || !IsValid ( LockedOnTarget ) )
    {
        bIsLockedOn = false;
        LockedOnTarget = nullptr;
        return;
    }

    if ( !OwnerCharacter ) return;

    FVector ToTarget = LockedOnTarget->GetActorLocation ( ) - OwnerCharacter->GetActorLocation ( );
    ToTarget.Z = 0.f;
    FRotator TargetRotation = ToTarget.Rotation ( );

    // 캐릭터 회전 보간
    FRotator NewCharRotation = FMath::RInterpTo ( OwnerCharacter->GetActorRotation ( ) , TargetRotation , DeltaTime , 10.f );
    OwnerCharacter->SetActorRotation ( NewCharRotation );

    // 카메라도 타겟 바라보게 (컨트롤러 회전 보간)
    AController* Controller = OwnerCharacter->GetController ( );
    if ( Controller )
    {
        FRotator ControlRot = Controller->GetControlRotation ( );

        // 카메라 위치 기준으로 타겟 바라보는 회전
        FVector CamLoc = Camera->GetComponentLocation ( );
        FRotator CamToTargetRot = ( LockedOnTarget->GetActorLocation ( ) - CamLoc ).Rotation ( );

        // 컨트롤러 회전 보간
        FRotator NewControlRot = FMath::RInterpTo ( ControlRot , CamToTargetRot , DeltaTime , 5.f );
        //Controller->SetControlRotation ( NewControlRot );
    }

    // 디버그
    DrawDebugSphere ( GetWorld ( ) , LockedOnTarget->GetActorLocation ( ) , 70.f , 16 , FColor::Blue , false , -1.f , 0 , 3.f );
}