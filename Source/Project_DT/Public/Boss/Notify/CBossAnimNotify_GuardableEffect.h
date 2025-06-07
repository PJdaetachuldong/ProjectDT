// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Classes/NiagaraSystem.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"
#include "CBossAnimNotify_GuardableEffect.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_DT_API UCBossAnimNotify_GuardableEffect : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	FString GetNotifyName_Implementation() const override;

    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
    UNiagaraSystem* CanGuardEffect;

    // False일 때 사용할 나이아가라 시스템
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
    UNiagaraSystem* NotGuardEffect;

	// 이펙트를 붙일 소켓 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
    FName SocketName;

    // 로컬 위치 오프셋
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
    FVector LocalLocation;

    // 로컬 회전
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
    FRotator LocalRotation;

    //스폰된 나이아가라 컴포넌트 저장
    UNiagaraComponent* SpawnEffect;
};
