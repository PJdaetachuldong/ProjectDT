// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CAttachment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/ShapeComponent.h"
#include "Components/SceneComponent.h"
#include "Enemy/EnemyBase/CEnemyBase.h"
#include "Component/CStatusComponent.h"
#include "Weapons/CWeaponComponent.h"

// Sets default values
ACAttachment::ACAttachment()
{
    CHelpers::CreateComponent<USkeletalMeshComponent>(this, &ColorMesh, "ColorMesh");
    CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMesh, "SkeletalMesh");
    CHelpers::CreateComponent ( this , &Root , "Root" );
    SetRootComponent(Root);
    SkeletalMesh->SetupAttachment(Root);		
    ColorMesh->SetupAttachment(Root);
    
    SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    ColorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    SkeletalMesh->SetVisibility(false);
    ColorMesh->SetRelativeScale3D(FVector::ZeroVector);


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
       OffCollisions ( ); // BeginPlay 시점에는 충돌을 끄는 것이 일반적입니다.
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
    // 초기화되지 않았다면 현재 위치를 이전 위치로 설정하고 다음 틱부터 트레이스 시작
    if (!bInitialized)
    {
       PrevStartLocation = CurrentStartLocation;
       PrevEndLocation = CurrentEndLocation;
       bInitialized = true;
       return; // 첫 프레임에는 궤적을 계산할 이전 데이터가 없으므로 반환
    }

    // 3. 중간 지점 계산 (궤적 방향 계산에 사용 가능)
    FVector PrevMiddleLocation = (PrevStartLocation + PrevEndLocation) * 0.5f;
    FVector CurrentMiddleLocation = (CurrentStartLocation + CurrentEndLocation) * 0.5f;

    // 3. 히트 결과 초기화
    HitResults.Reset();

    // 4. 거리 기반 보간 단계 계산
    float InterpolationStep = 1.0f; // 트레이스 선분 간격 (cm)
    float Distance = FVector::Dist(PrevStartLocation, CurrentStartLocation);
    int32 NumSteps = FMath::CeilToInt(Distance / InterpolationStep);
    NumSteps = FMath::Max(NumSteps, 1); // 최소 1 보장 (0으로 나누는 것 방지)

    // 5. 제어점 계산 (베지어 곡선의 중간점. 칼의 휘어지는 정도 조절)
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

       // 기존 PerformTriangleTrace 호출 (Alpha 값 전달 없음)
       PerformTriangleTrace(StartA, MidA, EndA, HitResults);
       PerformTriangleTrace(StartB, MidB, EndB, HitResults);
    }

    // --- 7. 히트 처리 (수정된 부분) ---
    for (const FHitResult& Hit : HitResults)
    {
       // 충돌한 액터가 CEnemyBase 타입인지 확인
       if (Hit.GetActor()->IsA(ACEnemyBase::StaticClass()))
       {
          ACEnemyBase* Enemy = Cast<ACEnemyBase>(Hit.GetActor());

          // **[중복 충돌 방지 로직]**
          // 이번 스윙에서 이미 이 적을 히트했는지 확인합니다.
          if (HitActorsThisSwing.Contains(Enemy))
          {
             continue; // 이미 히트했다면 더 이상 처리하지 않고 다음 히트로 넘어갑니다.
          }

          // 새로 히트한 적이라면 Set에 추가합니다.
          HitActorsThisSwing.Add(Enemy);
          
          // EnemyActor는 이제 중복 충돌 방지에 필요 없습니다. (기존 사용 방식이 TSet으로 대체됨)
          // EnemyActor = Enemy; 
          
          // 기존 히트 처리 (딜링, 마나 회복 등)
          if (OnAttachmentBeginOverlap.IsBound())
             OnAttachmentBeginOverlap.Broadcast(OwnerCharacter, this, Cast<ACharacter>(Hit.GetActor()));
          
          Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);
          UCWeaponComponent* Weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
          
          if (Weapon->GetWeaponType()==EWeaponType::Katana)
             Status->RecoverMana(2);
          else if (Weapon->GetWeaponType()==EWeaponType::GreatSword)
             Status->RecoverMana(5);

          // **[나이아가라 이펙트 스폰 및 궤적 방향 계산]**
if (HitEffect)
{
    // SwingDirection 계산은 기존과 동일
    FVector SwingDirection = CurrentMiddleLocation - PrevMiddleLocation;

    FRotator SwingRotation = FRotator::ZeroRotator;
    bool bIsHorizontalAttack = false; // 가로 공격 여부를 판단할 변수
    
    if (!SwingDirection.IsNearlyZero(KINDA_SMALL_NUMBER))
    {
        SwingDirection.Normalize(); // 방향 벡터 정규화

        // --- 가로/세로 공격 구분 로직 ---
        // SwingDirection의 Z축 값으로 수직/수평성을 판단
        // Z축 값이 작으면 수평에 가깝고, Z축 값이 크면 수직에 가깝습니다.
        // 임계값(Threshold)은 게임 플레이에 맞춰 조정해야 합니다.
        float AbsZ = FMath::Abs(SwingDirection.Z); // Z축 방향의 절대값

        if (AbsZ < 0.3f) // Z축 변화가 적으면 가로 공격으로 판단 (예: 0.3 미만)
        {
            bIsHorizontalAttack = true;
            // UE_LOG(LogTemp, Warning, TEXT("Horizontal Attack!"));
        }
        else // Z축 변화가 크면 세로 공격으로 판단
        {
            bIsHorizontalAttack = false;
            // UE_LOG(LogTemp, Warning, TEXT("Vertical Attack!"));
        }
        // -----------------------------

        SwingRotation = SwingDirection.Rotation();
        // SwingRotation += FRotator(0.0f, 90.0f, 0.0f); // 필요하다면 이펙트 오프셋 추가
    }

    // 이제 bIsHorizontalAttack 값을 활용하여 이펙트, 사운드 등을 다르게 처리할 수 있습니다.
    if (bIsHorizontalAttack)
    {
        // 가로 공격용 이펙트 또는 사운드
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            HitEffect, // 또는 HitEffect_Horizontal 같은 다른 이펙트 사용
            Hit.ImpactPoint,
            SwingRotation+HorizontalRotation,

            FVector(1.0f),
            true,
            true
        );
    }
    else
    {
        // 세로 공격용 이펙트 또는 사운드
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            HitEffect, // 또는 HitEffect_Vertical 같은 다른 이펙트 사용
            Hit.ImpactPoint,
            SwingRotation+VerticalRotation,
            FVector(1.0f),
            true,
            true
        );
    }

    // HitEffect2는 기존처럼 충돌 노멀 기반 회전을 유지합니다.
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(
        GetWorld(),
        HitEffect2,
        Hit.ImpactPoint,
        Hit.ImpactNormal.Rotation(),
        FVector(1.0f),
        true,
        true
    );
}
       	UWorld* World = GetWorld();
       	if (World)
       	{
       		// 시간을 0.1배로 느리게 합니다. (1.0f가 정상 속도)
       		UGameplayStatics::SetGlobalTimeDilation(World, 0.4f);

       		// 0.1초 후에 시간을 원래대로 되돌리는 타이머를 설정합니다.
       		FTimerHandle TimerHandle_ResetTimeDilation;
       		World->GetTimerManager().SetTimer(
				   TimerHandle_ResetTimeDilation,
				   this,
				   &ACAttachment::ResetTimeDilation,
				   0.05f, // 0.1초 후에 실행
				   false // 반복하지 않음
			   );
       	}
       	OnHitEvent();
       }
    }

}

void ACAttachment::OnBeginEquip()
{
    SpawnWeapon();
}

void ACAttachment::OnUnequip()
{
    SkeletalMesh->SetVisibility(false);
    ColorMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f)); // 초기 스케일 1
    ScaleTime = 0.0f; // 시간 초기화

    auto UpdateScale = [this]()
    {
       if (!ColorMesh) return;

       // 시간 업데이트
       ScaleTime += 0.02f; // 타이머 간격만큼 시간 증가
       float Alpha = FMath::Clamp(ScaleTime / 0.4f, 0.0f, 1.0f); // 0~1로 보간

       // 스케일 Lerp (1에서 0으로)
       FVector NewScale = FMath::Lerp(FVector(1.0f, 1.0f, 1.0f), FVector(0.0f, 0.0f, 0.0f), Alpha);
       ColorMesh->SetRelativeScale3D(NewScale);

       // 0.4초가 지나면 타이머 종료
       if (ScaleTime >= 0.4f)
       {
          GetWorld()->GetTimerManager().ClearTimer(ScaleTimerHandle);
          ColorMesh->SetRelativeScale3D(FVector(0.0f, 0.0f, 0.0f)); // 최종 스케일 고정
       }
    };

    // 타이머 시작 (0.02초마다 UpdateScale 호출)
    GetWorld()->GetTimerManager().SetTimer(ScaleTimerHandle, UpdateScale, 0.02f, true);
}

void ACAttachment::AttachTo ( FName InSocketName )
{
    AttachToComponent ( OwnerCharacter->GetMesh ( ) , FAttachmentTransformRules ( EAttachmentRule::KeepRelative , true ) , InSocketName );
}

void ACAttachment::SpawnWeapon()
{
    ColorMesh->SetRelativeScale3D(FVector(0.0f, 0.0f, 0.0f)); // 초기 스케일 0
    ScaleTime = 0.0f; // 시간 초기화
    auto UpdateScale = [this]()
    {
       if (!ColorMesh) return;

       // 시간 업데이트
       ScaleTime += 0.02f; // 타이머 간격만큼 시간 증가
       float Alpha = FMath::Clamp(ScaleTime / 1.0f, 0.0f, 1.0f); // 0~1로 보간

       // 스케일 Lerp
       FVector NewScale = FMath::Lerp(FVector(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f), Alpha);
       ColorMesh->SetRelativeScale3D(NewScale);

       // 1초가 지나면 타이머 종료
       if (ScaleTime >= 1.0f)
       {
          GetWorld()->GetTimerManager().ClearTimer(ScaleTimerHandle);
          ColorMesh->SetRelativeScale3D(FVector(.0f, .0f, .0f)); // 최종 스케일 고정
          SkeletalMesh->SetVisibility(true);
       }
    };
    // 타이머 시작 (0.02초마다 UpdateScale 호출)
    GetWorld()->GetTimerManager().SetTimer(ScaleTimerHandle,UpdateScale, 0.02f, true);
    
    
    
}

void ACAttachment::OnCollisions ( )
{
    // 스윙 시작 시, 이전에 히트한 적 목록을 초기화합니다.
    HitActorsThisSwing.Reset(); 

    if ( OnAttachmentBeginCollision.IsBound ( ) )
       OnAttachmentBeginCollision.Broadcast ( );

    bCollisionTraceEnabled = true;
    EnemyActor = nullptr; // 이 변수는 이제 사용되지 않을 수 있습니다. (TSet으로 대체됨)

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
    CheckTrue ( OwnerCharacter == OtherActor ); // 자기 자신과 충돌 방지
    CheckTrue ( OwnerCharacter->GetClass ( ) == OtherActor->GetClass ( ) ); // 같은 클래스 (예: 다른 플레이어) 충돌 방지 (필요하다면 수정)
}

void ACAttachment::OnComponentEndOverlap ( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex )
{
    CheckTrue ( OwnerCharacter == OtherActor );
    CheckTrue ( OwnerCharacter->GetClass ( ) == OtherActor->GetClass ( ) );


    if ( OnAttachmentEndOverlap.IsBound ( ) )
       OnAttachmentEndOverlap.Broadcast ( OwnerCharacter, Cast<ACharacter> ( OtherActor ) );
}

// PerformTriangleTrace 함수는 기존 시그니처를 유지합니다.
void ACAttachment::PerformTriangleTrace(
    const FVector& A,
    const FVector& B,
    const FVector& C,
    TArray<FHitResult>& OutHits)
{
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(OwnerCharacter);
    Params.bTraceComplex = true; // 복잡한 충돌 (메시 단위) 트레이스

    UWorld* World = GetWorld();
    if (!World) return;

    // A to B 트레이스
    FHitResult Hit1;
    if (World->LineTraceSingleByChannel(Hit1, A, B, ECC_EngineTraceChannel5, Params))
    {
       OutHits.Add(Hit1);
       //DrawDebugLine(World, A, B, FColor::Red, false, 0.1f, 0, 2.f);
    }
    else
    {
       //DrawDebugLine(World, A, B, FColor::Green, false, 0.1f, 0, 1.f);
    }

    // B to C 트레이스
    FHitResult Hit2;
    if (World->LineTraceSingleByChannel(Hit2, B, C, ECC_EngineTraceChannel5, Params))
    {
       OutHits.Add(Hit2);
       //DrawDebugLine(World, B, C, FColor::Red, false, 0.1f, 0, 2.f);
    }
    else
    {
       //DrawDebugLine(World, B, C, FColor::Green, false, 0.1f, 0, 1.f);
    }

    // C to A 트레이스
    FHitResult Hit3;
    if (World->LineTraceSingleByChannel(Hit3, C, A, ECC_EngineTraceChannel5, Params))
    {
       OutHits.Add(Hit3);
       //DrawDebugLine(World, C, A, FColor::Red, false, 0.1f, 0, 2.f);
    }
    else
    {
       //DrawDebugLine(World, C, A, FColor::Green, false, 0.1f, 0, 1.f);
    }
}

// 베지어 곡선 계산 함수 (기존 코드 유지)
FVector ACAttachment::BezierCurve(const FVector& P0, const FVector& P1, const FVector& P2, float T)
{
    return FMath::Pow(1 - T, 2) * P0 + 2 * (1 - T) * T * P1 + FMath::Pow(T, 2) * P2;
}

// 베지어 곡선 접선 계산 함수 (새로 추가)
FVector ACAttachment::GetBezierCurveTangent(const FVector& P0, const FVector& P1, const FVector& P2, float T)
{
    // 2차 베지어 곡선 P(t) = (1-t)^2*P0 + 2(1-t)t*P1 + t^2*P2
    // 미분하면 P'(t) = 2(1-t)(P1-P0) + 2t(P2-P1)
    return 2 * (1 - T) * (P1 - P0) + 2 * T * (P2 - P1);
}
void ACAttachment::ResetTimeDilation()
{
	UWorld* World = GetWorld();
	if (World)
	{
		// 시간을 1.0배 (정상 속도)로 되돌립니다.
		UGameplayStatics::SetGlobalTimeDilation(World, 1.0f);
	}
}
void ACAttachment::OnHitEvent() // 예: 피격이 발생했을 때 호출되는 함수
{
	// ... (기존 피격 처리 로직) ...

	// HitSoundCues 배열에 사운드 큐가 하나라도 있는지 확인
	if (HitSoundCues.Num() > 0)
	{
		// 배열에서 랜덤한 인덱스 선택
		int32 RandomIndex = FMath::RandRange(0, HitSoundCues.Num() - 1);

		// 선택된 사운드 큐가 유효한지 확인
		if (HitSoundCues[RandomIndex])
		{
			// 액터 위치에서 사운드 큐 재생
			UGameplayStatics::PlaySoundAtLocation(
				this, // 월드를 가져올 오브젝트 (보통 자기 자신)
				HitSoundCues[RandomIndex], // 재생할 사운드 큐
				GetActorLocation() // 사운드가 재생될 월드 위치
			);
		}
	}
}
