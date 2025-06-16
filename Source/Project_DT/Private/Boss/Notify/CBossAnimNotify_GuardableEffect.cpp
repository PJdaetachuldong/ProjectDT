// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notify/CBossAnimNotify_GuardableEffect.h"
#include "Global.h"
#include "Boss/CBossEnemy.h"

FString UCBossAnimNotify_GuardableEffect::GetNotifyName_Implementation() const
{
	return "EnabledEffect";
}

void UCBossAnimNotify_GuardableEffect::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	//현재 공격이 가드가 가능한 공격인지 체크를 함
	//현재 진행 중인 몽타주를 알아냄
	UAnimMontage* NowMontage = MeshComp->GetAnimInstance()->GetCurrentActiveMontage();
	if (!NowMontage)
	{
		return;
	}

	//현재 진행중인 몽타주의 섹션을 알아냄
	FName NowSection = MeshComp->GetAnimInstance()->Montage_GetCurrentSection(NowMontage);

	// bool 값에 따라 사용할 나이아가라 시스템 선택
	UNiagaraSystem* SelectedSystem = My->SetGuardBool(NowMontage, NowSection) ? CanGuardEffect : NotGuardEffect;

	if (SelectedSystem)
	{
// 		// 나이아가라 이펙트를 스폰 (설정한 소켓, 위치 오프셋, 회전에 맞춤)
// 		SpawnEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
// 			SelectedSystem,
// 			MeshComp,
// 			SocketName, // 설정한 소켓 이름
// 			LocalLocation, // 로컬 위치 오프셋
// 			LocalRotation, // 로컬 회전
// 			EAttachLocation::KeepRelativeOffset,
// 			true // Auto Destroy
// 		);

		// 나이아가라 이펙트를 지정된 위치에 스폰
		SpawnEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			My, // 소유자 (예: Actor 또는 World Context Object)
			SelectedSystem,
			My->EffectTrans->GetComponentLocation(), 
			My->EffectTrans->GetComponentRotation(),
			FVector(1.0f),
			true
		);
	}
}

void UCBossAnimNotify_GuardableEffect::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBossEnemy* My = Cast<ACBossEnemy>(MeshComp->GetOwner());

	CheckNull(My);

	// 스폰된 나이아가라 컴포넌트 제거
	if (SpawnEffect)
	{
		SpawnEffect->Deactivate();
		SpawnEffect->DestroyComponent();
		SpawnEffect = nullptr;
	}
}
