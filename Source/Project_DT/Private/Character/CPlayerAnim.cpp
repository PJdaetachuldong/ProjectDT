// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CPlayerAnim.h"
#include "GameFramework/Character.h"
#include "Global.h"
#include "Character/CPlayer.h"

void UCPlayerAnim::NativeBeginPlay ( )
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACPlayer> ( TryGetPawnOwner ( ) );
	CheckNull ( OwnerCharacter );

	Weapon = CHelpers::GetComponent<UCWeaponComponent> ( OwnerCharacter );
	State = CHelpers::GetComponent<UCStateComponent> ( OwnerCharacter );
	if ( !!Weapon )
		Weapon->OnWeaponTypeChange.AddDynamic ( this , &UCPlayerAnim::OnWeaponTypeChanged );

}

void UCPlayerAnim::NativeUpdateAnimation ( float DeltaSeconds )
{
	Super::NativeUpdateAnimation ( DeltaSeconds );

	CheckNull ( OwnerCharacter );
	isMontagePlaying = OwnerCharacter->IsPlayingRootMotion();
	Speed = OwnerCharacter->GetVelocity ( ).Size2D ( );
	auto Owner = Cast<ACPlayer> ( TryGetPawnOwner ( ) );
	Movement = Owner->GetCharacterMovement();
	Trajectory = Owner->Trajectory;
	WeaponType = Weapon->GetWeaponType ( );
	StateType = State->GetStateType ( );
	isGuardHit=Owner->bGuardHit;
	CheckFootStep();
}

void UCPlayerAnim::CheckFootStep()
{
	
	USkeletalMeshComponent* SkelMesh = OwnerCharacter->GetMesh();
	if (!SkelMesh)
	{
		return;
	}

	// 왼발 체크
	bool bCurrentLeftFootOnGround = false;
	FVector LeftHitLocation = FVector::ZeroVector;
	UPhysicalMaterial* LeftHitPhysicalMaterial = nullptr;
	CheckFootstep(LeftFootBoneName, bCurrentLeftFootOnGround, LeftHitLocation, LeftHitPhysicalMaterial);

	// 왼발 접지 순간 감지
	if (bCurrentLeftFootOnGround && !bPrevLeftFootOnGround)
	{
		// 왼발이 막 땅에 닿은 순간
		OnLeftFootstep.Broadcast(LeftFootBoneName, LeftHitLocation); // 기본 이벤트
		if (LeftHitPhysicalMaterial)
		{
			CLog::Log("foot_left");
			OnLeftFootstepWithMaterial.Broadcast(LeftFootBoneName, LeftHitLocation, LeftHitPhysicalMaterial); 
		}
	}
	bPrevLeftFootOnGround = bCurrentLeftFootOnGround; // 다음 프레임을 위해 현재 상태 저장


	// 오른발 체크
	bool bCurrentRightFootOnGround = false;
	FVector RightHitLocation = FVector::ZeroVector;
	UPhysicalMaterial* RightHitPhysicalMaterial = nullptr;
	CheckFootstep(RightFootBoneName, bCurrentRightFootOnGround, RightHitLocation, RightHitPhysicalMaterial);

	// 오른발 접지 순간 감지
	if (bCurrentRightFootOnGround && !bPrevRightFootOnGround)
	{
		// 오른발이 막 땅에 닿은 순간
		OnRightFootstep.Broadcast(RightFootBoneName, RightHitLocation); // 기본 이벤트
		if (RightHitPhysicalMaterial)
		{
			CLog::Log("foot_RIGHT");
			OnRightFootstepWithMaterial.Broadcast(RightFootBoneName, RightHitLocation, RightHitPhysicalMaterial); // 재질 포함 이벤트
		}
	}
	bPrevRightFootOnGround = bCurrentRightFootOnGround; // 다음 프레임을 위해 현재 상태 저장
}
void UCPlayerAnim::CheckFootstep(FName FootBoneName, bool& bOutIsOnGround, FVector& OutHitLocation, UPhysicalMaterial*& OutHitPhysicalMaterial)
{
    bOutIsOnGround = false;
    OutHitLocation = FVector::ZeroVector;
    OutHitPhysicalMaterial = nullptr;

    USkeletalMeshComponent* SkelMesh = GetSkelMeshComponent();
    UWorld* World = GetWorld(); // UWorld 가져오기

    if (!SkelMesh || !World || !FootBoneName.IsValid() || FootBoneName == NAME_None)
    {
        return;
    }

    // 소켓 위치를 사용할 경우:
    FVector BoneLocation = SkelMesh->GetSocketLocation(FootBoneName);
    // 본 위치를 사용할 경우 (소켓이 없다면):
    // FVector BoneLocation = SkelMesh->GetBoneLocation(FootBoneName);


    FVector TraceStart = BoneLocation + FVector(0.0f, 0.0f, TraceZOffset);
    FVector TraceEnd = BoneLocation - FVector(0.0f, 0.0f, TraceLength);

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(GetOwningActor()); // 캐릭터 자신은 무시
    QueryParams.bTraceComplex = false; // 복잡한 충돌 검사 (필요에 따라 true로)
    QueryParams.bReturnPhysicalMaterial = true; // 물리 재질 반환

    // 실제 라인 트레이스 수행
    bool bHit = World->LineTraceSingleByChannel(
        HitResult,
        TraceStart,
        TraceEnd,
        TraceChannel,
        QueryParams
    );

    if (bHit && HitResult.bBlockingHit)
    {
        bOutIsOnGround = true;
        OutHitLocation = HitResult.Location;
        if (HitResult.PhysMaterial.IsValid())
        {
            OutHitPhysicalMaterial = HitResult.PhysMaterial.Get();
        }
    }
}

void UCPlayerAnim::OnWeaponTypeChanged ( EWeaponType InPrevType , EWeaponType InNewType )
{
	WeaponType = InNewType;
}
