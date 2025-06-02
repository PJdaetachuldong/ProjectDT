// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/CBossWeaponStructure.h"
#include "Global.h"
#include "Engine/DamageEvents.h"

void FEnemyToPlayerHit::SendDamage(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther)
{
	FEnemyDamageEvent e;
	e.HitData = this;
	InOther->TakeDamage(Power, e, InAttacker->GetController(), InAttackCauser);
}

void FEnemyToPlayerHit::PlayMontage(class ACharacter* InOwner)
{
	if (!!Montage)
		InOwner->PlayAnimMontage(Montage, PlayRate);
}

void FEnemyToPlayerHit::PlayHitStop(UWorld* InWorld)
{
	CheckTrue(FMath::IsNearlyZero(StopTime));

	TArray<APawn*>pawns;
	for (AActor* actor : InWorld->GetCurrentLevel()->Actors) {
		APawn* pawn = Cast<ACharacter>(actor);

		if (!!pawn) {
			pawn->CustomTimeDilation = 1e-3f;
			pawns.Add(pawn);
		}
	}
	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([=]() {
		for (APawn* pawn : pawns)
			pawn->CustomTimeDilation = 1;
		});
	FTimerHandle hanlde;
	InWorld->GetTimerManager().SetTimer(hanlde, timerDelegate, StopTime, false);
}

void FEnemyToPlayerHit::PlaySoundWave(class ACharacter* InOwner)
{
	CheckNull(Sound);

	UWorld* world = InOwner->GetWorld();
	FVector location = InOwner->GetActorLocation();

	UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);
}

void FEnemyToPlayerHit::PlayEffect(UWorld* InWorld, const FVector& InLocation)
{
	CheckNull(Effect);

	FTransform transform;
	transform.SetLocation(EffectLocation);
	transform.SetScale3D(EffectScale);
	transform.AddToTranslation(InLocation);

	CHelpers::PlayEffect(InWorld, Effect, transform);
}

void FEnemyToPlayerHit::PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation)
{
	CheckNull(Effect);

	FTransform transform;
	transform.SetLocation(InLocation + InRotation.RotateVector(EffectLocation));
	transform.SetScale3D(EffectScale);

	CHelpers::PlayEffect(InWorld, Effect, transform);
}
