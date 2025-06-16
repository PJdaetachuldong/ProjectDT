// Fill out your copyright notice in the Description page of Project Settings.


#include "Tutorial/CTutorialEnemy.h"
#include "Weapons/CWeaponStuctures.h"
#include "Kismet/KismetMathLibrary.h"
#include "Global.h"
#include "Tutorial/CTutoAnim.h"
#include "Tutorial/CTutoWeapon.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Tutorial/CTutoManager.h"

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

	if(State == ETutoState::DIE ) return;

	if (State == ETutoState::IDLE || State == ETutoState::BREAK) 
	{
		if(AI)
		{
			AI->StopMovement();
		}
		return;
	}
	else if (State == ETutoState::ATTACKREADY)
	{
		AI->StopMovement();

		CurAttackTime += DeltaTime;

		if (FVector::Dist(Target->GetActorLocation(), GetActorLocation()) > AttackRange)
		{
			State = ETutoState::CHASE;
			AnimInstance->AnimState = ETutoState::CHASE;

			/*AI->MoveToLocation(Target->GetActorLocation());*/
		}

		if (CurAttackTime >= AttackLimitTime)
		{
			//공격 상태로 변환
			State = ETutoState::ATTACK;
			AnimInstance->AnimState = ETutoState::ATTACK;

			CurAttackTime = 0.0f;
		}
	}

	else if (State == ETutoState::CHASE)
	{
		if (FVector::Dist(Target->GetActorLocation(), GetActorLocation()) > AttackRange)
		{
			AI->MoveToLocation(Target->GetActorLocation());
		}

		else if (FVector::Dist(Target->GetActorLocation(), GetActorLocation()) <= AttackRange)
		{
			AI->StopMovement();

			State = ETutoState::ATTACKREADY;
			AnimInstance->AnimState = ETutoState::ATTACKREADY;
		}
// 		State = ETutoState::CHASE;
// 		AnimInstance->AnimState = ETutoState::CHASE;
	}
}

void ACTutorialEnemy::SettingManager(ACTutoManager* LevelManager)
{
	Manager = LevelManager;

	AI = Cast<AAIController>(GetController());
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

			if (Manager)
			{
				Manager->InputTutoEnemyDIE();
			}
		}

		return 0;
	}

	return TakeDamageAmount;
}

void ACTutorialEnemy::IDLEEnd()
{
	if (AnimInstance)
	{
		State = ETutoState::CHASE;
		AnimInstance->AnimState = ETutoState::CHASE;

		AI = Cast<AAIController>(GetController());
	}
}

void ACTutorialEnemy::ATKEnd()
{
	State = ETutoState::CHASE;
	AnimInstance->AnimState = ETutoState::CHASE;
}

void ACTutorialEnemy::AttackTurn()
{
	//공격이 제대로 실행되기 전에 플레이어를 바라보도록 만듦
	// 플레이어 방향 계산
	FVector TargetLocation = Target->GetActorLocation();
	FVector MyLocation = GetActorLocation();
	FVector DirectionToTarget = (TargetLocation - MyLocation).GetSafeNormal();

	// Yaw 회전만 조정 (Pitch는 유지)
	FRotator TargetRotation = FRotationMatrix::MakeFromX(DirectionToTarget).Rotator();
	TargetRotation.Pitch = 0.0f; // Pitch를 0으로 설정해 수평 회전만 적용
	TargetRotation.Roll = 0.0f;

	// 부드러운 회전을 위해 Interp 사용 (선택 사항)
	FRotator CurrentRotation = GetActorRotation();
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 100.0f);

	// Pawn 회전 설정
	SetActorRotation(NewRotation);
}