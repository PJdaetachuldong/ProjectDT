// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/DoActions/CDoAction_Combo.h"
#include "Global.h"
#include "Component/CStateComponent.h"
#include "Engine/DamageEvents.h"

void UCDoAction_Combo::DoAction()
{
    // DoActionDatas 배열의 크기가 1보다 작으면 첫 번째 동작을 수행할 수 없음
    // 콤보 시작 시 최소 1개의 동작 데이터는 있어야 함
    CheckTrue(DoActionDatas.Num() < 1);

    if (bEnable)
    {
       bEnable = false;
       bExist = true; // 현재 액션이 이미 활성화되어 있음을 나타냄

       return;
    }
    // 특정 상태에서는 DoAction을 시작할 수 없음
    CheckFalse(State->IsIdleMode());
    CheckTrue(State->IsHittedMode());
    CheckTrue(State->IsGuardMode());
    CheckTrue(State->IsParryMode());
    CheckTrue(State->IsCounterMode());

    Super::DoAction();
    if (bParry) return; // 패링 중이면 일반 DoAction을 수행하지 않음

    // --- 방어 코드: DoActionDatas[0] 접근 전 유효성 검사 ---
    if (DoActionDatas.IsValidIndex(0))
    {
        AddComboArray(DoActionDatas[0].DoAction(OwnerCharacter));
        DamageIndex = 0;
    }
    else
    {
        CLog::Log("Error: DoActionDatas[0] is not valid. Cannot start combo.", __FILE__, __LINE__);
        // 여기에 기본 동작이나 에러 처리 로직 추가 가능
    }
}

void UCDoAction_Combo::Begin_DoAction()
{
    Super::Begin_DoAction();
    CheckFalse(bExist); // bExist가 false여야만 Begin_DoAction이 호출될 수 있음 (초기화 또는 이전 액션 종료 후)
    CheckTrue(State->IsGuardMode()); // 가드 모드에서는 Begin_DoAction을 시작할 수 없음

    bExist = false;
    switch (ActionState)
    {
    case EActionState::Normal: {
        // --- 방어 코드: DoActionDatas[++Index] 접근 전 유효성 검사 ---
        // 인덱스를 먼저 증가시키므로, 미리 유효성 검사
        if (DoActionDatas.IsValidIndex(Index + 1))
        {
            AddComboArray(DoActionDatas[++Index].DoAction(OwnerCharacter));
            DamageIndex = Index;
        }
        else
        {
            CLog::Log("Error: DoActionDatas[++Index] is out of bounds. Combo sequence ended or error.", __FILE__, __LINE__);
            // 콤보 끝에 도달했거나 오류. 콤보를 초기화하거나 다른 처리 필요.
            End_DoAction(); // 콤보 종료
        }
    }
    break;
    case EActionState::Heavy: {
        // --- 방어 코드: DoHeavyActionDatas[++HeavyIndex] 접근 전 유효성 검사 ---
        // 인덱스를 먼저 증가시키므로, 미리 유효성 검사
        if (DoHeavyActionDatas.IsValidIndex(HeavyIndex + 1))
        {
            AddComboArray(DoHeavyActionDatas[++HeavyIndex].DoHeavyAction(OwnerCharacter));
            DamageIndex = HeavyIndex + 3; // HeavyIndex의 경우 3을 더하는 로직 유지
        }
        else
        {
            CLog::Log("Error: DoHeavyActionDatas[++HeavyIndex] is out of bounds. Heavy Combo sequence ended or error.", __FILE__, __LINE__);
            // 콤보 끝에 도달했거나 오류. 콤보를 초기화하거나 다른 처리 필요.
            End_DoAction(); // 콤보 종료
        }
    }
    break;
    default:
        CLog::Log("Warning: Unknown ActionState in Begin_DoAction.", __FILE__, __LINE__);
        break;
    }
}

void UCDoAction_Combo::End_DoAction()
{
    Super::End_DoAction();

    Index = 0;
    HeavyIndex = 0;
    DamageIndex = 0;
    bParry = false;
}

void UCDoAction_Combo::DoHeavyAction()
{
    // DoHeavyActionDatas 배열의 크기가 1보다 작으면 첫 번째 동작을 수행할 수 없음
    CheckTrue(DoHeavyActionDatas.Num() < 1);
    //CheckTrue(State->IsSubActionMode()); // 주석 처리된 부분 유지
    CheckTrue(State->IsGuardMode()); // 가드 모드에서는 HeavyAction을 시작할 수 없음

    if (bEnable)
    {
       bEnable = false;
       bExist = true;

       return;
    }

    CheckFalse(State->IsIdleMode());
    if (bParry) return; // 패링 중이면 HeavyAction을 수행하지 않음

    Super::DoHeavyAction();
    // --- 방어 코드: DoHeavyActionDatas[0] 접근 전 유효성 검사 ---
    if (DoHeavyActionDatas.IsValidIndex(0))
    {
        AddComboArray(DoHeavyActionDatas[0].DoHeavyAction(OwnerCharacter));
        DamageIndex = 3; // HeavyAction의 첫 동작은 DamageIndex 3으로 설정
    }
    else
    {
        CLog::Log("Error: DoHeavyActionDatas[0] is not valid. Cannot start heavy combo.", __FILE__, __LINE__);
        // 여기에 기본 동작이나 에러 처리 로직 추가 가능
    }
}

void UCDoAction_Combo::DoActionParry(EParryState parryState)
{
    // ParryActionDatas 배열의 크기가 1보다 작으면 패링 동작을 수행할 수 없음
    CheckTrue(ParryActionDatas.Num() < 1);

    Super::DoActionParry(parryState);
    switch (parryState)
    {
    case EParryState::TL:
        // --- 방어 코드: ParryActionDatas[0] 접근 전 유효성 검사 ---
        if (ParryActionDatas.IsValidIndex(0))
            ParryActionDatas[0].DoParryAction(OwnerCharacter);
        else
            CLog::Log("Error: ParryActionDatas[0] (TL) is out of bounds.", __FILE__, __LINE__);
        break;
    case EParryState::BL:
        // --- 방어 코드: ParryActionDatas[1] 접근 전 유효성 검사 ---
        if (ParryActionDatas.IsValidIndex(1))
            ParryActionDatas[1].DoParryAction(OwnerCharacter);
        else
            CLog::Log("Error: ParryActionDatas[1] (BL) is out of bounds.", __FILE__, __LINE__);
        break;
    case EParryState::TR:
        // --- 방어 코드: ParryActionDatas[2] 접근 전 유효성 검사 ---
        if (ParryActionDatas.IsValidIndex(2))
            ParryActionDatas[2].DoParryAction(OwnerCharacter);
        else
            CLog::Log("Error: ParryActionDatas[2] (TR) is out of bounds.", __FILE__, __LINE__);
        break;
    case EParryState::BR:
        // --- 방어 코드: ParryActionDatas[3] 접근 전 유효성 검사 ---
        if (ParryActionDatas.IsValidIndex(3))
            ParryActionDatas[3].DoParryAction(OwnerCharacter);
        else
            CLog::Log("Error: ParryActionDatas[3] (BR) is out of bounds.", __FILE__, __LINE__);
        break;
    case EParryState::Max:
        CLog::Log("Warning: EParryState::Max received in DoActionParry.", __FILE__, __LINE__);
        break;
    default:
        CLog::Log("Warning: Unknown EParryState in DoActionParry.", __FILE__, __LINE__);
        break;
    }
}

void UCDoAction_Combo::Begin_Parry()
{
    Super::Begin_Parry();
    bParry = true;
}

void UCDoAction_Combo::End_Parry()
{
    Super::End_Parry();
    bParry = false;
}

void UCDoAction_Combo::CounterAction()
{
    // CounterActionDatas 배열의 크기가 1보다 작으면 카운터 동작을 수행할 수 없음
    CheckTrue(CounterActionDatas.Num() < 1);
    CheckFalse(bExist); // bExist가 false여야만 CounterAction이 호출될 수 있음

    bExist = false;

    Super::CounterAction();
    // --- 방어 코드: CounterActionDatas[0] 접근 전 유효성 검사 ---
    if (CounterActionDatas.IsValidIndex(0))
    {
        CounterActionDatas[0].DoCounterAction(OwnerCharacter);
    }
    else
    {
        CLog::Log("Error: CounterActionDatas[0] is not valid. Cannot perform CounterAction.", __FILE__, __LINE__);
    }
}

void UCDoAction_Combo::ResetDoAction()
{
    Index = 0;
    HeavyIndex = 0;
    bParry = false;
    DamageIndex = 0;

    //bBeginAction = false; // 주석 처리된 부분 유지
}

void UCDoAction_Combo::OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCuaser, class ACharacter* InOther)
{
    Super::OnAttachmentBeginOverlap(InAttacker, InAttackCuaser, InOther);
    CheckNull(InOther);

    // --- 방어 코드: HitDatas[DamageIndex] 접근 전 유효성 검사 ---
    if (HitDatas.IsValidIndex(DamageIndex))
    {
        HitDatas[DamageIndex].SendDamage(InAttacker, InAttackCuaser, InOther);
    }
    else
    {
        CLog::Log("Error: HitDatas[DamageIndex] is out of bounds. DamageIndex: " + FString::FromInt(DamageIndex), __FILE__, __LINE__);
    }
}

void UCDoAction_Combo::AddComboArray(FString NewCombo)
{
    if (ComboArray.Num() >= 3)
    {
       ComboArray.RemoveAt(0);
    }

    ComboArray.Add(NewCombo);
}

bool UCDoAction_Combo::IsLeftMajority()
{
    int32 LeftCount = 0;
    int32 OtherCount = 0;

    for (const FString& Combo : ComboArray)
    {
       if (Combo.Equals(TEXT("Left"), ESearchCase::IgnoreCase))
       {
          LeftCount++;
       }
       else
       {
          OtherCount++;
       }
    }

    return LeftCount <= OtherCount;
}