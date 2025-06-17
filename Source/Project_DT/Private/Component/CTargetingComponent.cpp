// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CTargetingComponent.h"
#include "Camera/CameraComponent.h"
#include "Global.h"
#include "MotionWarpingComponent.h"
#include "Boss/CBossEnemy.h"
#include "Component/CCameraActionComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Weapons/CWeaponComponent.h"
#include "Engine/OverlapResult.h"
#include "Component/CMovementComponent.h"
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
	Movement = CHelpers::GetComponent<UCMovementComponent> ( OwnerCharacter );
	Motion = CHelpers::GetComponent<UMotionWarpingComponent> ( OwnerCharacter );
	CameraComp = CHelpers::GetComponent<UCCameraActionComponent> ( OwnerCharacter );

}   

void UCTargetingComponent::TickComponent ( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent ( DeltaTime , TickType , ThisTickFunction );

    if ( Weapon->GetWeaponType ( ) != EWeaponType::Max )
        if ( !Movement->GetCancelLockOn())
            UpdateLockOn ( DeltaTime );

}

AActor* UCTargetingComponent::FindClosestEnemyByDirection ( float MaxDistance , const FVector& InputDirection )
{
    if ( !OwnerCharacter ) return nullptr;

    FVector CharacterLocation = OwnerCharacter->GetActorLocation ( );

    TArray<AActor*> PotentialTargets;
    float ClosestDot = -1.f;
    AActor* ClosestTarget = nullptr;

    TArray<FOverlapResult> Overlaps;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor ( OwnerCharacter );

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


        // 유효한 적을 배열에 추가
        PotentialTargets.Add ( Enemy );

        // 적 방향과 입력 방향 간 내적 계산
        FVector EnemyDirection = ( Enemy->GetActorLocation ( ) - CharacterLocation ).GetSafeNormal ( );
        float DotProduct = FVector::DotProduct ( InputDirection , EnemyDirection );

        UE_LOG ( LogTemp , Log ,
            TEXT ( "[LockOn Check] Enemy: %s | DotProduct: %.2f" ) ,
            *Enemy->GetName ( ) , DotProduct
        );

        if ( DotProduct > ClosestDot )
        {
            ClosestDot = DotProduct;
            ClosestTarget = Enemy;
            TargetEnemy=ClosestTarget;
            FVector TargetLoc = TargetEnemy->GetActorLocation() - TargetEnemy->GetActorForwardVector()*137.f;	//137은 오차
            FRotator TargetRot = TargetEnemy->GetActorRotation();
            Motion->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("Target"), TargetLoc, TargetRot);
        }
    }

    if ( ClosestTarget )
    {

        UE_LOG ( LogTemp , Warning ,
            TEXT ( "[LockOn] Selected Target: %s (DotProduct: %.2f)" ) ,
            *ClosestTarget->GetName ( ) , ClosestDot
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
    // TODO: 5초 동안 공격 입력이 없으면 LockedOnTarget을 nullptr로 설정하고 bIsLockedOn을 false로 변경

    if ( !OwnerCharacter ) return;

    // UCMovementComponent에서 마지막 입력 방향 가져오기
    if ( !Movement )return;

    FVector InputDirection = Movement->LastInputDirection.GetSafeNormal ( );
    if ( InputDirection.IsNearlyZero ( ) )
    {
        UE_LOG ( LogTemp , Warning , TEXT ( "[LockOn] No valid input direction. %f"),InputDirection.Size() );
        return;
    }

    AActor* Target = FindClosestEnemyByDirection ( 600.f , InputDirection );
    if ( Target )
    {
        LockedOnTarget = Target;
        bIsLockedOn = true;
        UE_LOG ( LogTemp , Warning , TEXT ( "[LockOn] Locked on to: %s" ) , *Target->GetName ( ) );
    }
    //else
    //{
    //    LockedOnTarget = nullptr;
    //    bIsLockedOn = false;
    //    UE_LOG ( LogTemp , Warning , TEXT ( "[LockOn] Lock-Off (No target found)" ) );
    //}
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
    FRotator NewCharRotation = FMath::RInterpTo ( OwnerCharacter->GetActorRotation ( ) , TargetRotation , DeltaTime , 100.f );
    if(!Movement->GetCancelLockOn())
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
        if (LockedOnTarget->IsA(ACBossEnemy::StaticClass()))
            Controller->SetControlRotation ( NewControlRot );
        FRotator lookAtRot = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), LockedOnTarget->GetActorLocation());
    }

    // 디버그
    DrawDebugSphere ( GetWorld ( ) , LockedOnTarget->GetActorLocation ( ) , 70.f , 16 , FColor::Blue , false , -1.f , 0 , 3.f );
}

void UCTargetingComponent::ResetLockOn ( )
{
    bIsLockedOn = false;
    LockedOnTarget = nullptr;
    TimeSinceLostTarget = 0.f;
}
