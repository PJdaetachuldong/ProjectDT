#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CParryComponent.generated.h"

// 패링 성공 시 타격 방향을 나타내는 Enum (4방향 몽타주 재생용)
UENUM()
enum class EParryState : uint8
{
    TL, // Top-Left
    BL, // Bottom-Left
    TR, // Top-Right
    BR, // Bottom-Right
    Max // Enum 개수
};

// Blueprint에서 구독할 수 있는 델리게이트 (선택 사항: 패링 성공 시 외부로 알림)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FParryDetectedSignature, EParryState, ParryDirection);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_DT_API UCParryComponent : public UActorComponent
{
    GENERATED_BODY()

public:    
    // 생성자
    UCParryComponent();

protected:
    // 게임 시작 시 호출
    virtual void BeginPlay() override;

public:    
    // 매 프레임 호출 (패링 감지 로직에 사용됨)
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // R 키 입력 시 호출될 함수 (IE_Pressed 이벤트에 바인딩)
    void OnParry(); // 다시 OnParry() 하나로 통일

private:
    // 패링 시도 시 호출되는 함수 (패링 가능 상태 설정 및 타이머 시작)
    void ActivateParryWindow();

    // 패링 판정 유효 시간 동안 매 틱마다 실제 감지 로직 수행
    void PerformParryDetection();

    // 패링 시도 윈도우 종료 (성공/실패 무관)
    void EndParryWindow();
    
    // 패링 쿨다운 타이머 리셋 함수
    void ResetParryCooldown();

    // 타격 지점의 사분면을 계산하는 함수
    EParryState GetHitQuadrant(const FVector& ImpactPoint) const;

private:
    // 소유자 캐릭터
    UPROPERTY()
    class ACharacter* OwnerCharacter;

    // 소유자 캐릭터의 무기 컴포넌트
    UPROPERTY(VisibleAnywhere, Category = "References")
    class UCWeaponComponent* WeaponComponent;

    // 소유자 캐릭터의 상태 컴포넌트 (스태미나, 경직 등 처리)
    UPROPERTY(VisibleAnywhere, Category = "References")
    class UCStateComponent* StateComponent;
    UPROPERTY()
    class USoundCue* ParrySound;
    
    // 이전에 구현된 스태미나 컴포넌트가 있다면 여기에 추가
    // UPROPERTY(VisibleAnywhere, Category = "References")
    // class UCStaminaComponent* StaminaComponent;

private:
    // **패링 시도 쿨다운 중인지 나타내는 플래그 (R키 광클 방지)**
    UPROPERTY(VisibleAnywhere, Category = "Parry Settings")
    bool bOnParryCooldown = false;

    // **현재 패링 판정 유효 시간 내에 있는지 나타내는 플래그**
    UPROPERTY(VisibleAnywhere, Category = "Parry Settings")
    bool bIsParryWindowActive = false;

    // 타이머 핸들
    FTimerHandle ParryCooldownTimerHandle;
    FTimerHandle ParryWindowTimerHandle;

    // **패링 시도 쿨다운 시간 (R키 광클 후 다음 시도까지의 딜레이)**
    // 이 시간 동안은 R키를 눌러도 패링 시도조차 불가능
    UPROPERTY(EditAnywhere, Category = "Parry Settings", meta = (ClampMin = "0.0"))
    float ParryAttemptCooldown = 0.5f; // 기본값 0.5초 (조절 가능)

    // **패링 판정 유효 시간 (R키 누른 후 이 시간 안에 공격이 들어와야 패링 성공)**
    // 이 시간은 실제 패링 판정의 '창'이며 매우 짧아야 합니다. (0.1초 미만 권장)
    UPROPERTY(EditAnywhere, Category = "Parry Settings", meta = (ClampMin = "0.01", ClampMax = "0.2"))
    float ParryWindowDuration = 0.08f; // 매우 짧게 조정 (0.08초, 약 5프레임)

    // 패링 트레이스 반경
    UPROPERTY(EditAnywhere, Category = "Parry Settings")
    float ParryTraceRadius = 70.0f; // 패링 감지 범위 (조절 가능)

    // 패링 트레이스 거리
    UPROPERTY(EditAnywhere, Category = "Parry Settings")
    float ParryTraceDistance = 50.0f; // 패링 감지 전방 거리 (조절 가능)

    // 패링 성공 시 호출될 델리게이트 (Blueprint에서 사용 가능)
    UPROPERTY(BlueprintAssignable)
    FParryDetectedSignature OnParryDetected;

    // --- 애니메이션 관련 ---
    UPROPERTY(EditAnywhere, Category = "Parry Animation")
    class UAnimMontage* ParryStartMontage; // 패링 시도 시작 애니메이션 몽타주 (필수)
    UPROPERTY(EditAnywhere, Category = "Parry Animation")
    class UAnimMontage* ParryFailMontage; // 패링 실패 시 재생할 애니메이션 몽타주 (선택 사항, 페널티용)

    // --- 디버그 관련 변수 ---
    UPROPERTY(EditAnywhere, Category = "Parry Debug")
    float DebugDrawDuration = 1.0f; // 디버그 선/구체 유지 시간
    UPROPERTY(EditAnywhere, Category = "Parry Debug")
    float DebugDrawThickness = 2.0f; // 디버그 선 두께

    // --- 스태미나 관련 (선택 사항) ---
    UPROPERTY(EditAnywhere, Category = "Parry Stamina")
    float StaminaCostOnParryAttempt = 10.0f; // 패링 시도 시 스태미나 소모량
    UPROPERTY(EditAnywhere, Category = "Parry Stamina")
    float StaminaRecoverOnParrySuccess = 5.0f; // 패링 성공 시 스태미나 회복량
};