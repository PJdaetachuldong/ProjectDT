#include "Component/CParryComponent.h"
#include "Global.h" 
#include "Weapons/CWeaponComponent.h" 
#include "Boss/CBossWeapon.h" 
#include "Component/CStateComponent.h" 
#include "GameFramework/Character.h" 
#include "Kismet/KismetMathLibrary.h" 
#include "Animation/AnimInstance.h" 
#include "Animation/AnimMontage.h" 
#include "DrawDebugHelpers.h" 
#include "Engine/World.h" 
#include "Weapons/CSubAction.h"

// --- 필요하다면 여기에 StaminaComponent 헤더 추가 ---
// #include "Component/CStaminaComponent.h" // 예시

// 생성자
UCParryComponent::UCParryComponent()
{
    PrimaryComponentTick.bCanEverTick = true; 
    bOnParryCooldown = false; 
    bIsParryWindowActive = false;
    CHelpers::GetAsset(&ParrySound,AssetPaths::ParrySound);
    // 디버그 드로잉 지속 시간을 기본값으로 설정 (상시 표시)
    DebugDrawDuration = -1.0f; 
}

// 게임 시작 시 호출
void UCParryComponent::BeginPlay()
{
    Super::BeginPlay();
    OwnerCharacter = Cast<ACharacter>(GetOwner());
    WeaponComponent = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
    StateComponent = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
    // StaminaComponent = CHelpers::GetComponent<UCStaminaComponent>(OwnerCharacter); // 스태미나 컴포넌트 가져오기

    // 필수 컴포넌트가 없는 경우 경고 메시지 출력
    if (!WeaponComponent) CLog::Log("ParryComponent: WeaponComponent is NULL on " + GetOwner()->GetName(), __FILE__, __LINE__);
    if (!StateComponent) CLog::Log("ParryComponent: StateComponent is NULL on " + GetOwner()->GetName(), __FILE__, __LINE__);
    // if (!StaminaComponent) CLog::Log("ParryComponent: StaminaComponent is NULL on " + GetOwner()->GetName(), __FILE__, __LINE__);
}

// 매 프레임 호출
void UCParryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // 패링 판정 윈도우가 활성화되어 있을 때만 감지 로직을 매 틱마다 수행
    if (bIsParryWindowActive)
    {
        PerformParryDetection();
    }
}

// R 키를 누르는 순간 호출 (IE_Pressed에 바인딩)
void UCParryComponent::OnParry()
{
    // 패링 시도 쿨다운 중이거나, 이미 패링 윈도우가 활성화된 상태라면 실행하지 않음
    if (bOnParryCooldown || bIsParryWindowActive)
    {
        return;
    }

    // --- 스태미나 체크 (선택 사항) ---
    // if (StaminaComponent && StaminaComponent->GetCurrentStamina() < StaminaCostOnParryAttempt)
    // {
    //     CLog::Log("Parry | Not enough stamina to parry.", __FILE__, __LINE__);
    //     // 스태미나 부족 시 효과 (예: 휘청임 애니메이션, 사운드)
    //     return;
    // }
    // --- 스태미나 체크 끝 ---

    // 패링 시도 시작 (애니메이션, 스태미나 소모)
    ActivateParryWindow();

    // 패링 시도 즉시 쿨다운 시작 (성공/실패 여부와 무관하게 다음 R키 입력 방지)
    bOnParryCooldown = true;
    GetWorld()->GetTimerManager().SetTimer(
        ParryCooldownTimerHandle,
        this,
        &UCParryComponent::ResetParryCooldown,
        ParryAttemptCooldown, // 패링 시도 후 다음 시도까지의 쿨다운 시간
        false // 한 번만 실행
    );
}

// 패링 시도 시 호출되는 함수 (패링 가능 상태 설정 및 타이머 시작)
void UCParryComponent::ActivateParryWindow()
{
    bIsParryWindowActive = true; // 패링 판정 윈도우 활성화

    // 기존에 활성화된 패링 윈도우 타이머가 있다면 클리어 (안전 장치)
    GetWorld()->GetTimerManager().ClearTimer(ParryWindowTimerHandle);

    // 패링 판정 유효 시간 타이머 시작
    GetWorld()->GetTimerManager().SetTimer(
        ParryWindowTimerHandle,
        this,
        &UCParryComponent::EndParryWindow,
        ParryWindowDuration, // 설정된 패링 판정 유효 시간 (매우 짧음)
        false // 한 번만 실행
    );
    
    // --- 패링 시작 애니메이션 재생 (핵심 페널티) ---
        if (!!WeaponComponent->GetSubAction()){
            WeaponComponent->GetSubAction()->Pressed();
            StateComponent->SetParryMode();
        }

    // --- 스태미나 소모 (선택 사항) ---
    // if (StaminaComponent)
    // {
    //     StaminaComponent->ConsumeStamina(StaminaCostOnParryAttempt);
    //     CLog::Log("Parry | Consumed " + FString::SanitizeFloat(StaminaCostOnParryAttempt) + " stamina.");
    // }
}

// 실제 패링 감지 로직 (틱마다 호출될 수 있음)
void UCParryComponent::PerformParryDetection()
{
    if (!OwnerCharacter) return;

    FVector Start = OwnerCharacter->GetActorLocation();
    FVector Forward = OwnerCharacter->GetActorForwardVector();
    FVector End = Start + Forward * ParryTraceDistance;

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(OwnerCharacter);

    bool bHit = GetWorld()->SweepSingleByChannel(
        HitResult,
        Start,
        End,
        FQuat::Identity,
        ECC_GameTraceChannel3, // 프로젝트에 맞는 Collision Channel 사용 (필요시 수정)
        FCollisionShape::MakeSphere(ParryTraceRadius),
        Params
    );
// --- 디버그 드로잉 끝 ---

    AActor* HitActor = HitResult.GetActor();
    if (bHit && HitActor && HitActor->IsA(ACBossWeapon::StaticClass()))
    {
        ACBossWeapon* BossWeapon = Cast<ACBossWeapon>(HitActor);
        if (BossWeapon)
        {
            if (!BossWeapon->CheckGuardBool()) 
            {
                // 보스 무기가 패링 가능한 상태가 아닐 때 (Hit했지만 패링 실패)
                CLog::Log("Parry | Hit boss weapon, but it's not in a parryable state.", __FILE__, __LINE__);
                return; 
            }


            // 패링 판정 윈도우 즉시 종료 (성공했으니 더 이상 감지할 필요 없음)
            EndParryWindow(); 
            GetWorld()->GetTimerManager().ClearTimer(ParryWindowTimerHandle); // 혹시 모를 타이머 클리어

            EParryState ParryDirection = GetHitQuadrant(HitResult.ImpactPoint);

            if (WeaponComponent)
            {
                ACBossWeapon* Enemy = Cast<ACBossWeapon>(HitActor);

                if(!Enemy->CheckGuardBool())  return;

                WeaponComponent->OnParry(ParryDirection); // 4방향 몽타주 재생 트리거
            }
            if (BossWeapon)
            {
                BossWeapon->PlayParringAnim(); // 보스 무기 패링 애니메이션 재생
            }

            OnParryDetected.Broadcast(ParryDirection); // 외부 델리게이트 호출

            // --- 스태미나 회복 (선택 사항) ---
            // if (StaminaComponent)
            // {
            //     StaminaComponent->RecoverStamina(StaminaRecoverOnParrySuccess);
            //     CLog::Log("Parry | Recovered " + FString::SanitizeFloat(StaminaRecoverOnParrySuccess) + " stamina on success.");
            // }

            // 중요: 패링 성공 시에는 여기서 리턴하여, EndParryWindow에서 실패 처리 로직이 실행되지 않도록 합니다.
            return; 
        }
    }
}

// 패링 시도 윈도우 종료 (성공/실패 무관)
void UCParryComponent::EndParryWindow()
{
    // 이미 윈도우가 비활성화된 상태라면 리턴 (중복 호출 방지)
    if (!bIsParryWindowActive) return;

    bIsParryWindowActive = false; // 패링 판정 윈도우 비활성

    // 타이머가 만료되어 이 함수가 호출되었다면, 패링 실패
    // PerformParryDetection에서 성공 시에는 직접 EndParryWindow를 호출하고 바로 리턴하기 때문에,
    // 이 블록은 '패링 윈도우 시간 내에 공격을 받지 못했을 때'만 실행됩니다.
    CLog::Log("Parry | FAILED: Parry window closed without successful parry. (No parryable hit within window)", __FILE__, __LINE__);

    // --- 패링 실패 시 페널티 (핵심!) ---
    // 플레이어에게 패널티를 줍니다.
    // 1. 상태 컴포넌트에 페널티 상태 설정 (예: 짧은 경직)
    if (StateComponent)
    {
        // StateComponent에 SetStunnedState(시간) 같은 함수가 필요합니다.
        // 예: StateComponent->SetState(ECharacterState::Stunned, 0.3f);
        // CLog::Log("Parry | Applying brief stun/penalty for failed parry.", __FILE__, __LINE__);
    }
    // 2. 패링 실패 애니메이션 재생 (짧고 취약한 애니메이션)
    if (OwnerCharacter && ParryFailMontage) 
    {
        // OwnerCharacter->PlayAnimMontage(ParryFailMontage);
        CLog::Log("Parry | Playing ParryFailMontage.");
    }
    // 3. 스태미나 소모 등 (게임 시스템에 따라)
    // if (StaminaComponent)
    // {
    //     StaminaComponent->ConsumeStamina(StaminaCostOnParryAttempt * 2.0f); // 실패 시 더 많은 스태미나 소모
    //     CLog::Log("Parry | Consumed " + FString::SanitizeFloat(StaminaCostOnParryAttempt * 2.0f) + " stamina on failure.");
    // }
}

// 패링 시도 쿨다운 리셋 함수
void UCParryComponent::ResetParryCooldown()
{
    bOnParryCooldown = false; // 다음 패링 시도 가능하도록 설정
    CLog::Log("Parry | Cooldown ended. Can attempt parry again.", __FILE__, __LINE__);
}

// 타격 지점의 사분면 계산 (기존과 동일)
EParryState UCParryComponent::GetHitQuadrant(const FVector& ImpactPoint) const
{
    if (!OwnerCharacter) return EParryState::Max;
    FVector LocalHit = UKismetMathLibrary::InverseTransformLocation(OwnerCharacter->GetActorTransform(), ImpactPoint);
    if (LocalHit.X >= 0)
        return (LocalHit.Y >= 0) ? EParryState::TL : EParryState::TR;
    else
        return (LocalHit.Y >= 0) ? EParryState::BL : EParryState::BR;
}
