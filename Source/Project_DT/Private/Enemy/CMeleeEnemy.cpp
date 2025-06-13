// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/CMeleeEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy/FSM/CMeleeEnemyFSM.h"
#include "Character/CPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Weapons/CAttachment.h"
#include "Enemy/AIController/CMeleeAIController.h"
#include "Weapons/CWeaponStuctures.h"
#include "Kismet/KismetMathLibrary.h"
#include "Global.h"

// Sets default values
ACMeleeEnemy::ACMeleeEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ChaseDistance = 200.0f;
	WanderSpeed = 450.0f;
	WanderRadius = 500.0f;
	BackStepSpeed = 50.0f;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempSkeletal (TEXT("/Script/Engine.SkeletalMesh'/Game/Ghoul/BaseMesh/SK_Ghoul.SK_Ghoul'"));
	if (TempSkeletal.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempSkeletal.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-88),FRotator(0,-90,0));
	}

	TSubclassOf<UAnimInstance> Anim;
	CHelpers::GetClass<UAnimInstance>(&Anim, AssetPaths::MeleeAnim);
	GetMesh()->SetAnimClass(Anim);

	GetCharacterMovement ( )->bUseRVOAvoidance = true;
	GetCharacterMovement ( )->AvoidanceConsiderationRadius = 200.0f;
	GetCharacterMovement ( )->AvoidanceWeight = 0.2f;
	GetCharacterMovement ( )->SetAvoidanceGroup (1);
	GetCharacterMovement ( )->SetGroupsToAvoid (1);
	
	GetCapsuleComponent()->SetCollisionProfileName(FName("TestEnemy"));
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACMeleeEnemy::EnemyHitDamage);

	FSMComponent = CreateDefaultSubobject<UCMeleeEnemyFSM> ( TEXT ( "FSMComp" ) );


	AIControllerClass = ACMeleeAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void ACMeleeEnemy::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = GetMesh()->GetAnimInstance();
}

// Called every frame
void ACMeleeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if ( FSMComponent->State == EMeleeEnemyState::IDLE )
	{
		CheckPlayerInRange();
	}

	if (FSMComponent->State == EMeleeEnemyState::CHASE && Target)
	{
		float Distance = FVector::Dist(GetActorLocation(), Target->GetActorLocation());
		if (Distance <= AttackRange)
		{
			//공격권이 있으면 ATTACK상태로 변환
			if (IsCanAttack)
			{
				FSMComponent->State = EMeleeEnemyState::ATTACK;
			}
			//공격권이 없으면 WANDER상태로 변환
			else
			{
				FSMComponent->State = EMeleeEnemyState::WANDER;
			}
		}
	}

	//쉴드가 부셔진 상태이면
	if (CurShieldAmount <= 0.0f)
	{
		//현재 부서진 상태를 계속 체크
		CurBreakTime += DeltaTime;

		//일정 시간 이상이 되면
		if (CurBreakTime >= ResetShieldTime)
		{
			//실드를 다시 복구
			CurShieldAmount = MaxShieldAmount;

			CurBreakTime = 0.0f;
		}
	}
}

void ACMeleeEnemy::LoadStatsFromAsset()
{
	Super::LoadStatsFromAsset();

	if ( StatsAsset )
	{
		ChaseDistance = StatsAsset->Stats.ChaseDistance;
	}
}

void ACMeleeEnemy::CheckPlayerInRange()
{
	//SphereTrace와 내적을 사용해서 플레이어를 감지하는 방식
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool bHit = GetWorld()->SweepSingleByChannel(Hit,GetActorLocation(), GetActorLocation(), FQuat::Identity, ECC_GameTraceChannel3 , FCollisionShape::MakeSphere(500.0f), Params);

	if (bHit && Hit.GetActor()->IsA(ACPlayer::StaticClass()))
	{
// 		UE_LOG ( LogTemp , Warning , TEXT ( "Hit Actor: %s, Class: %s" ) ,
// 			*Hit.GetActor ( )->GetName ( ) , *Hit.GetActor ( )->GetClass ( )->GetName ( ) );

		FVector ToPlayer = (Hit.GetActor()->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
		
		float DotProduct = FVector::DotProduct(GetActorForwardVector(), ToPlayer);
		float AngleRad = FMath::Acos(DotProduct);
		float AngleDeg = FMath::RadiansToDegrees(AngleRad);

		if (AngleDeg <= 40.0f)
		{
			//플레이어 탐지하는데 장애물이 있는지 확인함
			FHitResult LOSHit;
			bool bLOS = GetWorld()->LineTraceSingleByChannel(LOSHit, GetActorLocation(), Hit.GetActor()->GetActorLocation(), ECC_WorldDynamic , Params);

			if (bLOS && LOSHit.GetActor()->IsA( ACPlayer::StaticClass()))
			{
				//타겟 저장
				Target = Cast<ACharacter>(/*LOSHit*/Hit.GetActor());
				IsCanAttack = true;
				FSMComponent-> State = EMeleeEnemyState::CHASE;

				if ( Manager )
				{
					//매니저에게 플레이어를 발견했다고 전달, 다른 에너미도 추적하도록 만듦
					Manager->SendPlayerEncounter(Target, MyUniqeID);
				}
			}
		}
	}
}

void ACMeleeEnemy::SetStateCHASE(ACharacter* Player)
{
	Target = Player;
	FSMComponent->State = EMeleeEnemyState::CHASE;
}

void ACMeleeEnemy::EnemyHitDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
bool bFromSweep, const FHitResult& SweepResult)
{
	ACAttachment* Weapon = Cast<ACAttachment>(OtherActor);

	if ( Weapon )
	{
		//공격을 맞았을 때 쉴드 게이지가 있다면
		if (CurShieldAmount > 0.0f )
		{
			//쉴드게이지가 감소하도록 설정
			CurShieldAmount -= /*Damage*/10.0f;

			//만약 쉴드게이지 감소되어서 0이 된다면
			if (CurShieldAmount <= 0.0f )
			{

				//쉴드 게이지가 -의 값이 되면 해당 값 만큼 체력을 깎게 만듦
				CurHP += CurShieldAmount;

				return;
			}
		}

		//쉴드 게이지가 없는 상태에서 맞았을 경우
		else
		{
			//체력을 깎게 만들어줌
			CurHP -= /*Damage*/10.0f;

			//체력이 깎였는데 0 이하가 된다면
			if ( CurHP <= 0.0f )
			{
				//사망 상태로 만듦
				FSMComponent->State = EMeleeEnemyState::DIE;

				//만약 에너미 매니저가 있다면
				if ( Manager )
				{
					Manager->RemoveEnemiesList ( MyUniqeID , IsCanAttack );
				}
			}
		}
	}
}

void ACMeleeEnemy::Hitted()
{
	//사망 상태면 안되게 막음
	if (FSMComponent->State == EMeleeEnemyState::DIE) return;

	Damage.Power = 0;
	
	if (!!Damage.Event && !!Damage.Event->HitData) {
		FHitData* data = Damage.Event->HitData;

		if (CurShieldAmount <= 0.0f)
		{
			if (ShieldBreakHit < 2) ++ShieldBreakHit;

			if (FSMComponent->State == EMeleeEnemyState::BREAK && ShieldBreakHit >= 2)
			{
				if (IsCanAttack)
				{
					FSMComponent->State = EMeleeEnemyState::CHASE;
				}
				else if (!IsCanAttack)
				{
					FSMComponent->State = EMeleeEnemyState::WANDER;
				}
			}

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

float ACMeleeEnemy::TakeDamage(float TakeDamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(TakeDamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Damage.Power = damage;
	Damage.Character = Cast<ACharacter>(EventInstigator->GetPawn());
	Damage.Causer = DamageCauser;
	Damage.Event = (FActionDamageEvent*)&DamageEvent;
	CLog::Log(Damage.Power);
	Hitted();
	return TakeDamageAmount;
}

void ACMeleeEnemy::Hit(FString Name)
{
	UE_LOG(LogTemp, Log, TEXT("Actor Name: %s"), *Name);

	bool IsKatana = Name.Contains(TEXT("Katana"));

	if (IsKatana)
	{
		//사망 상태면 안되게 막음
		if (FSMComponent->State == EMeleeEnemyState::DIE) return;

		//현재 쉴드가 있을 경우
		if (CurShieldAmount > 0)
		{
			//현재 어떠한 공격 애니메이션이 재생 중이라면
			if (AnimInstance->Montage_IsPlaying(AM_Attack))
			{
				//쉴드와 체력의 감소를 4:2의 비율로 감소함
				CurShieldAmount -= 10.0f * 0.4f;
				CurHP -= 10.0f * 0.2f;

				//체력이 0이하가 됐을 경우
				if (CurHP <= 0)
				{
					//보스 사망
					FSMComponent->State = EMeleeEnemyState::DIE;

					//만약 에너미 매니저가 있다면
					if (Manager)
					{
						Manager->RemoveEnemiesList(MyUniqeID, IsCanAttack);
					}

					return;
				}

				//쉴드가 0이하가 됐을 경우
				if (CurShieldAmount <= 0)
				{
					// -가 된 쉴드 게이지만큼 체력을 깎아줌
					CurHP += CurShieldAmount;

					if (FSMComponent->State != EMeleeEnemyState::BREAK)
					{
						//브레이크 상태로 됨
						FSMComponent->State = EMeleeEnemyState::BREAK;
					}
				}
			}

			//공격 모션이 재생중이 아니라면
			else
			{
				//쉴드가 있는 경우에는 검으로 막는 애니메이션 재생
				//브레이크 상태가 아니면 재생되게, 나중에 조건 바꾸기
				if (CurShieldAmount > 0 && FSMComponent->State != EMeleeEnemyState::BREAK)
				{
					AnimInstance->Montage_Play(AM_ShieldHit);
				}

				//쉴드는 데미지의 값 만큼 감소
				CurShieldAmount -= /*5*/10;

				//쉴드가 0이하가 됐을 경우
				if (CurShieldAmount <= 0)
				{
					// -가 된 쉴드 게이지만큼 체력을 깎아줌
					CurHP += CurShieldAmount;

					//체력이 0이하가 됐을 경우
					if (CurHP <= 0)
					{
						//보스 사망
						FSMComponent->State = EMeleeEnemyState::DIE;

						//만약 에너미 매니저가 있다면
						if (Manager)
						{
							Manager->RemoveEnemiesList(MyUniqeID, IsCanAttack);
						}

						return;
					}

					if (FSMComponent->State != EMeleeEnemyState::BREAK)
					{
						//브레이크 상태로 됨
						FSMComponent->State = EMeleeEnemyState::BREAK;
					}
					//아래 코드가 실행 안되게 리턴
					return;
				}
			}
		}

		//현재 쉴드가 없는 경우
		else
		{
			//체력이 데미지 10의 비율로 감소
			CurHP -= /*20*/10;

			//0이하가 됐을 경우
			if (CurHP <= 0.0f)
			{
				//사망 처리
				FSMComponent->State = EMeleeEnemyState::DIE;

				//만약 에너미 매니저가 있다면
				if (Manager)
				{
					Manager->RemoveEnemiesList(MyUniqeID, IsCanAttack);
				}

				return;
			}
		}
	}

	else
	{
		bool IsGreatSword = Name.Contains(TEXT("GreateSword"));

		if (IsGreatSword)
		{
			//사망 상태면 안되게 막음
			if (FSMComponent->State == EMeleeEnemyState::DIE) return;

			//현재 쉴드가 있을 경우
			if (CurShieldAmount > 0)
			{
				//현재 어떠한 공격 애니메이션이 재생 중이라면
				if (AnimInstance->Montage_IsPlaying(AM_Attack))
				{
					//쉴드와 체력의 감소를 4:2의 비율로 감소함
					CurShieldAmount -= 10.0f * 0.8f;
					CurHP -= 10.0f * 0.2f;

					//체력이 0이하가 됐을 경우
					if (CurHP <= 0)
					{
						//보스 사망
						FSMComponent->State = EMeleeEnemyState::DIE;

						//만약 에너미 매니저가 있다면
						if (Manager)
						{
							Manager->RemoveEnemiesList(MyUniqeID, IsCanAttack);
						}

						return;
					}

					//쉴드가 0이하가 됐을 경우
					if (CurShieldAmount <= 0)
					{
						// -가 된 쉴드 게이지만큼 체력을 깎아줌
						CurHP += CurShieldAmount;

						if (FSMComponent->State != EMeleeEnemyState::BREAK)
						{
							//브레이크 상태로 됨
							FSMComponent->State = EMeleeEnemyState::BREAK;
						}
					}
				}

				//공격 모션이 재생중이 아니라면
				else
				{
					//쉴드가 있는 경우에는 검으로 막는 애니메이션 재생
					//브레이크 상태가 아니면 재생되게, 나중에 조건 바꾸기
					if (CurShieldAmount > 0 && FSMComponent->State != EMeleeEnemyState::BREAK)
					{
						AnimInstance->Montage_Play(AM_ShieldHit);
					}

					//쉴드는 데미지의 값 만큼 감소
					CurShieldAmount -= /*15*/10;

					//쉴드가 0이하가 됐을 경우
					if (CurShieldAmount <= 0)
					{
						// -가 된 쉴드 게이지만큼 체력을 깎아줌
						CurHP += CurShieldAmount;

						//체력이 0이하가 됐을 경우
						if (CurHP <= 0)
						{
							//보스 사망
							FSMComponent->State = EMeleeEnemyState::DIE;

							//만약 에너미 매니저가 있다면
							if (Manager)
							{
								Manager->RemoveEnemiesList(MyUniqeID, IsCanAttack);
							}

							return;
						}

						if (FSMComponent->State != EMeleeEnemyState::BREAK)
						{
							//브레이크 상태로 됨
							FSMComponent->State = EMeleeEnemyState::BREAK;
						}
						//아래 코드가 실행 안되게 리턴
						return;
					}
				}
			}

			//현재 쉴드가 없는 경우
			else
			{
				//체력이 데미지 10의 비율로 감소
				CurHP -= /*5*/10;

				//0이하가 됐을 경우
				if (CurHP <= 0.0f)
				{
					//사망 처리
					FSMComponent->State = EMeleeEnemyState::DIE;

					//만약 에너미 매니저가 있다면
					if (Manager)
					{
						Manager->RemoveEnemiesList(MyUniqeID, IsCanAttack);
					}

					return;
				}
			}
		}
	}
}
