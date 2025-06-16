// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_Intro.h"
#include "Global.h"
#include "LHW_GameModeBase.h"
#include "Component/CProductionComponent.h"

FString UCAnimNotify_Intro::GetNotifyName_Implementation() const
{
	return "Intro";
}

void UCAnimNotify_Intro::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	ALHW_GameModeBase* GameMode = Cast<ALHW_GameModeBase>(MeshComp->GetOwner()->GetWorld()->GetAuthGameMode());
	UCProductionComponent* Production=CHelpers::GetComponent<UCProductionComponent>(MeshComp->GetOwner());
	UCStateComponent* State=CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	CheckNull(Production);
	CheckNull(State);
	Production->SetViewToCharacterCamera(1.0f);
	State->SetIdleMode();
	FString ScriptText = TEXT("어라? 여기는... 어쩌다 잠이 들었더라?\n기억이 잘 나지 않아...\n우선은 더 어두워 지기 전에 빨리 집으로 돌아가야겠다.");
	GameMode->CreateScriptUI(ScriptText);
	
}
