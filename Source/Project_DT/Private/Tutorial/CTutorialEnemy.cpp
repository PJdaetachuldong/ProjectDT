// Fill out your copyright notice in the Description page of Project Settings.


#include "Tutorial/CTutorialEnemy.h"
#include "Weapons/CWeaponStuctures.h"
#include "Kismet/KismetMathLibrary.h"
#include "Global.h"
#include "Tutorial/CTutoAnim.h"
#include "Tutorial/CTutoWeapon.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACTutorialEnemy::ACTutorialEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TSubclassOf<UAnimInstance> Anim;
	CHelpers::GetClass<UAnimInstance>(&Anim, AssetPaths::TutoAnim);
	GetMesh()->SetAnimClass(Anim);
}

// Called when the game starts or when spawned
void ACTutorialEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	
	AI = Cast<AAIController>(GetController());
	Target = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	AnimInstance = Cast<UCTutoAnim>(GetMesh()->GetAnimInstance());

	LoadStatsFromAsset();

	//무기 생성
	if (TutoWeapon)
	{
		FTransform SocketTransform = GetMesh()->GetSocketTransform(FName("TutoWeapon_Socket"));

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		SpawnTutoWeapon = GetWorld()->SpawnActor<ACTutoWeapon>(TutoWeapon, SocketTransform.GetLocation(), SocketTransform.Rotator(), SpawnParams);

		if (SpawnTutoWeapon)
		{
			SpawnTutoWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("TutoWeapon_Socket"));
			SpawnTutoWeapon->SetTutoOwner(this);
		}
	}
}

// Called every frame
void ACTutorialEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (State == ETutoState::BREAK || State == ETutoState::DIE) return;

	//패링 튜토일때 양식
	if (IsParry)
	{	
		//거리가 공격범위 아래일 경우
		if (FVector::Dist(Target->GetActorLocation(), GetActorLocation()) <= AttackRange)
		{
			CurAttackTime += DeltaTime;
			if (CurAttackTime >= AttackLimitTime)
			{
				//공격 상태로 변환
				State = ETutoState::ATTACK;
				AnimInstance->AnimState = ETutoState::ATTACK;

				CurAttackTime = 0.0f;
			}
		}

		else if (FVector::Dist(Target->GetActorLocation(), GetActorLocation()) > AttackRange)
		{
			AI->MoveToLocation(Target->GetActorLocation());
			State = ETutoState::CHASE;
			AnimInstance->AnimState = ETutoState::CHASE;
		}
	}

	//저스트 회피 튜토일때 양식
	if (IsJustVoid)
	{

	}
}

void ACTutorialEnemy::OnTutoCollision()
{
	if (SpawnTutoWeapon)
	{
		SpawnTutoWeapon->WeaponCollComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void ACTutorialEnemy::OffTutoCollision()
{
	if (SpawnTutoWeapon)
	{
		SpawnTutoWeapon->WeaponCollComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ACTutorialEnemy::LoadStatsFromAsset()
{
	Super::LoadStatsFromAsset();
}

void ACTutorialEnemy::SetHP(float value)
{
	CurHP = CurHP - value;
}

void ACTutorialEnemy::SetShieldAmount(float value)
{
	CurShieldAmount = CurShieldAmount - value;
}	

void ACTutorialEnemy::Hitted()
{
	//사망 상태면 안되게 막음
	if (State == ETutoState::BREAK || State == ETutoState::DIE) return;

	if (!!Damage.Event && !!Damage.Event->HitData) 
	{
		FHitData* data = Damage.Event->HitData;

		if (CurShieldAmount <= 0)
		{
			data->PlayMontage(this);
		}

		data->PlayHitStop(GetWorld());
		{
			FVector start = GetActorLocation();
			FVector target = Damage.Character->GetActorLocation();
			FVector direction = target - start;
			direction.Normalize();

			LaunchCharacter(-direction * data->Launch, false, false);
			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
		}
	}
	Damage.Character = nullptr;
	Damage.Causer = nullptr;
	Damage.Event = nullptr;
}

float ACTutorialEnemy::TakeDamage(float TakeDamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(TakeDamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Damage.Power = damage;
	Damage.Character = Cast<ACharacter>(EventInstigator->GetPawn());
	Damage.Causer = DamageCauser;
	Damage.Event = (FActionDamageEvent*)&DamageEvent;
	CLog::Log(Damage.Power);
	CLog::Log(Damage.Causer);

	if (State == ETutoState::DIE) return 0;
	
	if (CurShieldAmount > 0)
	{
		SetShieldAmount(Damage.Power);

		if (CurShieldAmount <= 0)
		{
			State = ETutoState::BREAK;
			AnimInstance->AnimState = ETutoState::BREAK;
		}

		return 0;
	}

	if (CurShieldAmount <= 0)
	{
		SetHP(Damage.Power);

		if (CurHP <= 0)
		{
			AnimInstance->StopAllMontages(0.1f);;

			State = ETutoState::DIE;
			AnimInstance->AnimState = ETutoState::DIE;
		}

		return 0;
	}

	return TakeDamageAmount;
}
