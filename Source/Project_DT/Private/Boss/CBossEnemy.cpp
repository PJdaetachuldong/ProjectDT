// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/CBossEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Boss/FSM/CBossFSM.h"
#include "Weapons/CAttachment.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/RangedAttackObject/CRangeAttack.h"
#include "Boss/CBossAnim.h"
#include "Components/BoxComponent.h"
#include "Character/CPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/CParryComponent.h"
#include "Weapons/CWeaponStuctures.h"
#include "Kismet/KismetMathLibrary.h"
#include "Global.h"
#include "Boss/CBossWeapon.h"
#include "Boss/DataAsset/HitDataAsset_BossToPlayer.h"
#include "Boss/SPAttackCollision/SPAttackCheckCollision.h"
#include "Component/CStatusComponent.h"
#include "Widget/BossWidget.h"
#include "Weapons/CWeaponComponent.h"
#include "Weapons/CDoAction.h"
#include "Components/ArrowComponent.h"
#include "Component/CMointageComponent.h"
#include "Boss/CBossDashATKCollision.h"

ACBossEnemy::ACBossEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempSkeletal (TEXT("/Script/Engine.SkeletalMesh'/Game/ODH/Asset/Boss/ElfArden/BaseMesh/SK_ElfArden.SK_ElfArden'"));
	if (TempSkeletal.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempSkeletal.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	GetCapsuleComponent()->SetCollisionProfileName(FName("TestEnemy"));

	FSMComponent = CreateDefaultSubobject<UCBossFSM>(TEXT("FSMComponent"));

	TSubclassOf<UAnimInstance> Anim;
	CHelpers::GetClass<UAnimInstance>(&Anim, AssetPaths::BossAnim);
	GetMesh()->SetAnimClass(Anim);

	//일단 임시로 하는 발사 위치 설정
	ThrowPosition = CreateDefaultSubobject<UArrowComponent>(L"ThrowPosition");
	ThrowPosition->SetupAttachment(RootComponent);
	//일단 임시로 하는 발사 위치 설정

	ConstructorHelpers::FObjectFinder<UAnimMontage> TempCombo1 (L"/Script/Engine.AnimMontage'/Game/ODH/Animation/Boss/Montage/ComboAttack_01/AM_ComboAttack_01_01.AM_ComboAttack_01_01'");
	if ( TempCombo1.Succeeded ( ) )
	{
		AM_ComboAttack_01 = TempCombo1.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> TempCombo2 ( L"/Script/Engine.AnimMontage'/Game/ODH/Animation/Boss/Montage/ComboAttack_02/Combo_Attack_02.Combo_Attack_02'" );
	if ( TempCombo2.Succeeded ( ) )
	{
		AM_ComboAttack_02 = TempCombo2.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> TempDash ( L"/Script/Engine.AnimMontage'/Game/ODH/Animation/Boss/Montage/DashAttack/AM_DashAttack.AM_DashAttack'" );
	if ( TempDash.Succeeded ( ) )
	{
		AM_DashAttack = TempDash.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> TempRanged ( L"/Script/Engine.AnimMontage'/Game/ODH/Animation/Boss/Montage/Ranged_Attack.Ranged_Attack'" );
	if ( TempRanged.Succeeded ( ) )
	{
		AM_RangedAttack = TempRanged.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> TempGuard ( L"/Script/Engine.AnimMontage'/Game/ODH/Animation/Boss/Montage/Guard/AM_Guard.AM_Guard'" );
	if ( TempGuard.Succeeded ( ) )
	{
		AM_Guard = TempGuard.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> TempSP ( L"/Script/Engine.AnimMontage'/Game/ODH/Animation/Boss/Montage/SPAttack/AM_SPAttack1.AM_SPAttack1'" );
	if ( TempSP.Succeeded ( ) )
	{
		AM_SPAttack = TempSP.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> TempBreak ( L"/Script/Engine.AnimMontage'/Game/ODH/Animation/Boss/Montage/Break/AM_Break.AM_Break'" );
	if ( TempBreak.Succeeded ( ) )
	{
		AM_Break = TempBreak.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> TempShieldHit ( L"/Script/Engine.AnimMontage'/Game/ODH/Animation/Boss/Montage/Hit/AM_ShieldHit.AM_ShieldHit'" );
	if ( TempShieldHit.Succeeded ( ) )
	{
		AM_ShieldHit = TempShieldHit.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> TempParringInter(L"/Script/Engine.AnimMontage'/Game/ODH/Animation/Boss/Montage/AM_ParringInter.AM_ParringInter'");
	if (TempParringInter.Succeeded())
	{
		AM_ParringInteraction = TempParringInter.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> TempDie(L"/Script/Engine.AnimMontage'/Game/ODH/Animation/Boss/Montage/AM_Die.AM_Die'");
	if (TempDie.Succeeded())
	{
		AM_Die = TempDie.Object;
	}

	ConstructorHelpers::FObjectFinder<UHitDataAsset_BossToPlayer> TempHitData (L"/Script/Project_DT.HitDataAsset_BossToPlayer'/Game/ODH/DataAsset/DA_Hit.DA_Hit'");
	if (TempHitData.Succeeded())
	{
		HitData = TempHitData.Object;
	}

	StartCollision = CreateDefaultSubobject<UBoxComponent>(L"StartColli");
	StartCollision->SetupAttachment(GetMesh());
	StartCollision->SetCollisionProfileName(L"BossWeapon");
	StartCollision->OnComponentBeginOverlap.AddDynamic(this, &ACBossEnemy::Start);

// 	DashATKCollision = CreateDefaultSubobject<UBoxComponent>(L"DashColli");
// 	DashATKCollision->SetupAttachment(GetMesh());
// 	DashATKCollision->SetCollisionProfileName(L"BossWeapon");
// 	DashATKCollision->OnComponentBeginOverlap.AddDynamic(this, &ACBossEnemy::DashPlayerHit);
// 	DashATKCollision->SetRelativeLocation(FVector(0, 59, 100));
// 	DashATKCollision->SetBoxExtent(FVector(41,29,80));

	CHelpers::GetClass(&BossUIClass,AssetPaths::BossUI);
}

void ACBossEnemy::BeginPlay()
{
	Super::BeginPlay();

	//임시로 타겟을 그냥 플레이어로 지정
	Target = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	//임시로 타겟을 그냥 플레이어로 지정
	if(Target){
		UCStatusComponent* Status=CHelpers::GetComponent<UCStatusComponent>(Target);
	if (Status) {
		Status->OnHeal.AddDynamic(this, &ACBossEnemy::OnPlayerHealed);
		}
	}

	//무기 생성
	if (MyWeapon)
	{
		FTransform SocketTransform = GetMesh()->GetSocketTransform(FName("Weapon_Socket"));

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		SpawnWeapon = GetWorld()->SpawnActor<ACBossWeapon>(MyWeapon, SocketTransform.GetLocation(), SocketTransform.Rotator(),SpawnParams);

		if (SpawnWeapon)
		{
			SpawnWeapon->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Weapon_Socket"));
		}
	}

	if (DashATKColli)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		SpawnDashATKColli = GetWorld()->SpawnActor<ACBossDashATKCollision>(DashATKColli, GetActorLocation(), GetActorRotation(), SpawnParams);

		if (SpawnDashATKColli)
		{
			// 스폰한 엑터를 부모의 루트 컴포넌트에 붙이기
			SpawnDashATKColli->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepWorldTransform );

			SpawnDashATKColli->GetMyBoss(this);
		}
	}

	if (SPAttackCollision)
	{
		//스폰할 객체에 대한 스폰 옵션을 설정하는 구조체
		FActorSpawnParameters Params;
		//스폰 과정에 충돌이 생겨도 제자리에서 스폰할 수 있게 만듦
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//필살기 공격 장판을 월드에 소환
		SpawnSpColli = GetWorld()->SpawnActor<ASPAttackCheckCollision>(SPAttackCollision, Params);
		SpawnSpColli->SetActive(false, GetActorLocation());
	}

	AnimInstance = Cast<UCBossAnim>(GetMesh()->GetAnimInstance());

	LoadStatsFromAsset();

	//몽타주 매핑 초기화
	InitializeMontageMap();

	//공격 섹션 이름 초기화
	InitAttackTMap();

	// 게임 시작 시 액터를 미리 스폰하고 비활성화(보이지 않게 설정)
	if (GuardVFX && GetWorld())
	{
		SpawnBarrier = GetWorld()->SpawnActor<AActor>(GuardVFX, GetActorTransform());
		if (SpawnBarrier)
		{
			// SpawnBarrier를 ACBossEnemy의 자식으로 붙임
			SpawnBarrier->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

			// 액터의 루트 컴포넌트 가시성 비활성화
			SpawnBarrier->SetActorHiddenInGame(true);
		}
	}

	FirstLocation = GetActorLocation();
	FirstRotation = GetActorRotation();

	/*DashATKCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/
}

void ACBossEnemy::InitializeMontageMap()
{
	if (AM_ComboAttack_01)
	{
		TArray<FMontageRateScale> ComboAttack01Rates;
		ComboAttack01Rates.Add(FMontageRateScale{FName("ComboAttack_01_01"), 0.5f, true});
		ComboAttack01Rates.Add(FMontageRateScale{FName("ComboAttack_01_02"), 0.7f, false });
		ComboAttack01Rates.Add(FMontageRateScale{FName("ComboAttack_01_03"), 0.7f, true });
		ComboAttack01Rates.Add(FMontageRateScale{FName("ComboAttack_01_04"), 0.7f, true });
		MontageScaleMap.Add(AM_ComboAttack_01, ComboAttack01Rates);
	}
	
	if (AM_ComboAttack_02)
	{
		TArray<FMontageRateScale> ComboAttack02Rates;
		ComboAttack02Rates.Add(FMontageRateScale{FName("ComboAttack_02_01"), 0.5f, true });
		ComboAttack02Rates.Add(FMontageRateScale{FName("ComboAttack_02_02"), 0.7f, false});
		ComboAttack02Rates.Add(FMontageRateScale{FName("ComboAttack_02_03"), 0.7f,false});
		ComboAttack02Rates.Add(FMontageRateScale{FName("ComboAttack_02_04"), 0.7f, true });
		MontageScaleMap.Add(AM_ComboAttack_02, ComboAttack02Rates);
	}

	if (AM_DashAttack)
	{
		TArray<FMontageRateScale> DashAttackRates;
		DashAttackRates.Add(FMontageRateScale{FName("DashAttackReady"), 0.7f, false});
		DashAttackRates.Add(FMontageRateScale{FName("Run"), 0.7f, false });
		DashAttackRates.Add(FMontageRateScale{FName("DashAttack"), 0.7f, true});
		MontageScaleMap.Add(AM_DashAttack, DashAttackRates);
	}

	if (AM_RangedAttack)
	{
		TArray<FMontageRateScale> RangedAttackRates;
		RangedAttackRates.Add(FMontageRateScale{ FName("Default"), 1.0f, false });
		MontageScaleMap.Add(AM_RangedAttack, RangedAttackRates);
	}

	if (AM_Guard)
	{
		TArray<FMontageRateScale> GuardRates;
		GuardRates.Add(FMontageRateScale{ FName("Guard_Start"), 0.7f });
		GuardRates.Add(FMontageRateScale{ FName("GuardStarting"), 0.7f });
		GuardRates.Add(FMontageRateScale{ FName("Guard_End"), 0.7f });
		GuardRates.Add(FMontageRateScale{ FName("Counter"), 1.0f, false});
		MontageScaleMap.Add(AM_Guard, GuardRates);
	}

	if (AM_SPAttack)
	{
		TArray<FMontageRateScale> SPAttackRates;
		SPAttackRates.Add(FMontageRateScale{ FName("Alram"), 0.7f, false });
		SPAttackRates.Add(FMontageRateScale{ FName("SPFirstATK"), 0.7f, false });
		SPAttackRates.Add(FMontageRateScale{ FName("SPAttackStart"), 0.7f, false });
		SPAttackRates.Add(FMontageRateScale{ FName("SPAttackLoop"), 0.7f, false });
		SPAttackRates.Add(FMontageRateScale{ FName("SPAttack"), 0.7f ,false});
		MontageScaleMap.Add(AM_SPAttack, SPAttackRates);
	}

	if (AM_ShieldHit)
	{
		TArray<FMontageRateScale> GuardRates;
		GuardRates.Add(FMontageRateScale{ FName("Hit"), 10.0f, false});
		GuardRates.Add(FMontageRateScale{ FName("Interaction"), 0.7f, false});
		GuardRates.Add(FMontageRateScale{ FName("Counter"), 1.0f, true});
		GuardRates.Add(FMontageRateScale{ FName("End"), 0.7f, false});
		MontageScaleMap.Add(AM_ShieldHit, GuardRates);
	}
}

void ACBossEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

/*	DrawDebugBox(GetWorld(), DashATKCollision->GetComponentLocation(), DashATKCollision->GetScaledBoxExtent(), FColor::Red, false, 0.1f);*/

	if(!BossStart) return;

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

			SetShieldAmount(-CurShieldAmount);

			ShieldBreakHit = 0;

			CurBreakTime = 0.0f;
		}
	}

	//만약 쉴드가 있는 상태에서 공격을 맞은 상태이면
	if (GuardingTime > 0.0f)
	{
		//맞은 횟수를 보존하는 시간이 흘러감
		GuardingTime -= DeltaTime;

		//0초 미만이면 
		if (GuardingTime <= 0.0f)
		{
			//맞은 횟수 초기화
			ShieldHitCount = 0;
		}
	}

	if (Cast<ACPlayer>(Target)->GetComponentByClass<UCMointageComponent>()->isDead)
	{
		RestartGame();
	}
}

void ACBossEnemy::SpawnGuardVFX()
{
	if (SpawnBarrier)
	{
		// 액터를 게임에서 보이게 설정
		SpawnBarrier->SetActorHiddenInGame(false);
	}
}

void ACBossEnemy::DeSpawnGuardVFX()
{
	if (SpawnBarrier)
	{
		// 액터를 게임에서 보이게 설정
		SpawnBarrier->SetActorHiddenInGame(true);
	}

// 	if (SpawnBarrier)
// 	{
// 		SpawnBarrier->Destroy();
// 		SpawnBarrier = nullptr;
// 	}
}

bool ACBossEnemy::BackstepUse()
{
	//불려왔을 때 랜덤값을 뽑아 값에 맞으면 백스텝을
	int32 RandomInt = FMath::RandRange(1,100);

	if (RandomInt > BackstepPercent)
	{
		BackstepPercent -= 10;
		return false;
	}

	else
	{
		BackstepPercent = 60;
		return true;
	}
}

// void ACBossEnemy::SPBreak()
// {
// 	
// }

void ACBossEnemy::ReadyDashAttack()
{
	FSMComponent->SetDashAttackLocation();	
}	

void ACBossEnemy::OnSwordCollision()
{
	//블프에서 안 터지게 막아주기
	if(SpawnWeapon)
	{ 
		//공격 콜리전 활성화
		SpawnWeapon->SwordCollComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void ACBossEnemy::OffSwordCollision()
{
	//블프에서 안 터지게 막아주기
	if(SpawnWeapon)
	{ 
		//공격 콜리전 비활성화
		SpawnWeapon->SwordCollComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ACBossEnemy::OnDashCollision()
{
	if (SpawnDashATKColli)
	{
		SpawnDashATKColli->OnDashCollision();
		
		/*DashATKCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);*/
	}
}

void ACBossEnemy::OffDashCollision()
{
	if (SpawnDashATKColli)
	{
		SpawnDashATKColli->OffDashCollision();

		/*DashATKCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/
	}
}

bool ACBossEnemy::OnGuardCollision ( )
{
	//공격이 현재 에너미가 바라보는 방향에서 어느 각도로 맞았는지 체크하게 만듦
	//SphereTrace와 내적을 사용해서 플레이어의 공격 각도를 체크하는 방식
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool bHit = GetWorld()->SweepSingleByChannel(Hit, GetActorLocation(), GetActorLocation(), FQuat::Identity, ECC_GameTraceChannel3 , FCollisionShape::MakeSphere(130.0f), Params);

	//일단 닿은게 플레이어 무기면 
	if ( bHit && Hit.GetActor()->IsA(/*ACAttachment*/ACPlayer::StaticClass()) )
	{
		//플레이어 무기와의 각도를 계산함
		FVector ToPlayer = (Hit.GetActor()->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
		
		float DotProduct = FVector::DotProduct(GetActorForwardVector(), ToPlayer);
		float AngleRad = FMath::Acos(DotProduct);
		float AngleDeg = FMath::RadiansToDegrees(AngleRad);

		//만약 각도가 100보다 크다면, 즉 후방 80도 사이에서 맞았을 경우
		 if ( AngleDeg >= 100.0f )
		{
			//자세가 흐트러지며 BREAK상태가 됨
			FSMComponent->State = EBossState::BREAK;
			FSMComponent->AttackState = EBossATTACKState::NONE;

			//가드 조건 초기화
			GuardGage = 0.0f;
			//뒤에 코드 작동이 안되게 리턴
			return false;
		}

		 //만약 각도가 전방 30도 사이라면, 정면에서 맞았을 경우
		 //그 외 각도에서 맞았을 경우
		 else
		 {
			 //바로 카운터 공격이 동작하게 만듦
		 /*	GEngine->AddOnScreenDebugMessage ( 111 , 1.0f , FColor::White , TEXT ( "Counter Attack!!!" ) );*/

			 UAnimMontage* NowMontage = GetMesh()->GetAnimInstance()->GetCurrentActiveMontage();

			 //현재 몽타주가 재생중인 섹션 확인
			 FName NowSection = GetMesh()->GetAnimInstance()->Montage_GetCurrentSection(NowMontage);

			 //카운터 공격 애니메이션이 나오도록 재생
			 //해당 몽타주 섹션으로 이동
			 AnimInstance->Montage_JumpToSection(FName("Counter"), NowMontage);

			 //가드 조건 초기화
			 GuardGage = 0.0f;
			 //뒤에 코드 작동이 안되게 리턴
			 return true;
		 }
	}

	return true;

// 	//만약 그냥 가드 시간이 끝났을 경우
// 	else
// 	{
// 		UAnimMontage* NowMontage = GetMesh()->GetAnimInstance ( )->GetCurrentActiveMontage ( );
// 		if ( !NowMontage )
// 		{
// 			return;
// 		}
// 
// 		//현재 몽타주가 재생중인 섹션 확인
// 		FName NowSection = GetMesh()->GetAnimInstance ( )->Montage_GetCurrentSection ( NowMontage );
// 
// 		//카운터 공격 애니메이션이 나오도록 재생
// 		//해당 몽타주 섹션으로 이동
// 		AnimInstance->Montage_JumpToSection(FName("Guard_End"), NowMontage);
// 
// 		/*GEngine->AddOnScreenDebugMessage ( 113 , 1.0f , FColor::White , TEXT ( "Guard Time Limite" ) );*/
// 		//그냥 공격상태를 NONE상태로 되돌림
// 		FSMComponent->AttackState = EBossATTACKState::NONE;
// 		//가드 조건 초기화
// 		GuardGage = 0.0f;
// 	}
}

void ACBossEnemy::OffGuardCollision ( )
{
	
}

bool ACBossEnemy::CheckPlayer()
{
	//애님 노티파이에서 해당 함수가 불림

	//플레이어와의 거리와 각도를 체크, 일단은 기존 에너미가 플레이어를 탐지하는 방식으로 진행
	//SphereTrace와 내적을 사용해서 플레이어를 감지하는 방식
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool bHit = GetWorld()->SweepSingleByChannel(Hit,GetActorLocation(), GetActorLocation(), FQuat::Identity, ECC_GameTraceChannel3 , FCollisionShape::MakeSphere(300.0f), Params);

	if (bHit && Hit.GetActor()->IsA(ACPlayer::StaticClass()))
	{
		FVector ToPlayer = (Hit.GetActor()->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
		
// 		float DotProduct = FVector::DotProduct(GetActorForwardVector(), ToPlayer);
// 		float AngleRad = FMath::Acos(DotProduct);
// 		float AngleDeg = FMath::RadiansToDegrees(AngleRad);
// 
// 		if (AngleDeg <= 40.0f)
// 		{
			//플레이어 탐지하는데 장애물이 있는지 확인함
			FHitResult LOSHit;
			bool bLOS = GetWorld()->LineTraceSingleByChannel(LOSHit, GetActorLocation(), Hit.GetActor()->GetActorLocation(), ECC_WorldDynamic , Params);

			if (bLOS && LOSHit.GetActor()->IsA( ACPlayer::StaticClass()))
			{
				//true로 값을 전달
				return true;
			}
		}
	/*}*/
	//아닐 경우 false로 값을 전달
	return false;
}

void ACBossEnemy::SetLocation()
{
	Attack3Location = GetActorLocation();
}

bool ACBossEnemy::CheckAttack1_3Player()
{
	//플레이어와의 거리와 각도를 체크, 일단은 기존 에너미가 플레이어를 탐지하는 방식으로 진행
	//SphereTrace와 내적을 사용해서 플레이어를 감지하는 방식
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool bHit = GetWorld()->SweepSingleByChannel(Hit, Attack3Location, Attack3Location, FQuat::Identity, ECC_GameTraceChannel3, FCollisionShape::MakeSphere(300.0f), Params);

	if (bHit && Hit.GetActor()->IsA(ACPlayer::StaticClass()))
	{
		FVector ToPlayer = (Hit.GetActor()->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();

// 		float DotProduct = FVector::DotProduct(GetActorForwardVector(), ToPlayer);
// 		float AngleRad = FMath::Acos(DotProduct);
// 		float AngleDeg = FMath::RadiansToDegrees(AngleRad);
// 
// 		if (AngleDeg <= 40.0f)
// 		{
			//플레이어 탐지하는데 장애물이 있는지 확인함
			FHitResult LOSHit;
			bool bLOS = GetWorld()->LineTraceSingleByChannel(LOSHit, GetActorLocation(), Hit.GetActor()->GetActorLocation(), ECC_WorldDynamic, Params);

			if (bLOS && LOSHit.GetActor()->IsA(ACPlayer::StaticClass()))
			{
				//true로 값을 전달
				return true;
			}
		/*}*/
	}
	//아닐 경우 false로 값을 전달
	return false;
}

void ACBossEnemy::AttackTurn()
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
	SetActorRotation (NewRotation);
}

void ACBossEnemy::ATK2_4Turn()
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
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 4.0f);

	// Pawn 회전 설정
	SetActorRotation(NewRotation);
}	

void ACBossEnemy::PlayNextSectionAttack ( UAnimMontage* CurrentMontage , FName CurrentSection )
{
	//없다면 실행하지 않음
	if ( !AnimInstance || !CurrentMontage)
	{
		return;
	}

	//다음 섹션의 이름을 현재 섹션이름을 갖고 가져옴
	FName NextSection = GetNextSection(CurrentSection);

	//다음 섹션 이름이 있음
	if ( NextSection != NAME_None )
	{
		//해당 몽타주 섹션으로 이동
		AnimInstance->Montage_JumpToSection(NextSection, CurrentMontage);
	}
}

FName ACBossEnemy::GetNextSection ( FName SectionName )
{
	//현재 섹션의 이름이 무엇인지 if문으로 체크
	//해당되는 if문으로 들어가 다음 섹션의 이름을 호출함

	if ( SectionName == FName ( "ComboAttack_01_01" ) )
	{
		return FName( "ComboAttack_01_02" );
	}
	else if ( SectionName == FName ( "ComboAttack_01_02" ) )
	{
		return FName("ComboAttack_01_03" );
	}
	else if ( SectionName == FName ( "ComboAttack_01_03" ) )
	{
		return FName ( "ComboAttack_01_04" );
	}
	else if ( SectionName == FName ( "ComboAttack_02_01" ) )
	{
		return FName ( "ComboAttack_02_02" );
	}
	else if ( SectionName == FName ( "ComboAttack_02_02" ) )
	{
		return FName ( "ComboAttack_02_03" );
	}
	else if ( SectionName == FName ( "ComboAttack_02_03" ) )
	{
		return FName ( "ComboAttack_02_04" );
	}

	return NAME_None;
}

void ACBossEnemy::RunCheckPlayerDist()
{
	//정해진 거리와의 거리를 측정
	float LocationToDist = FVector::Dist ( GetActorLocation ( ) , FSMComponent->CalculatedTargetLocation );

	UAnimMontage* NowMontage = GetMesh ( )->GetAnimInstance ( )->GetCurrentActiveMontage ( );

	//만약 일정 거리 안이면
	if ( LocationToDist <= 120.0f )
	{
		//대쉬 공격 애니메이션이 나오도록 만듦
		if ( !NowMontage )
		{
			return;
		}

		//현재 몽타주가 재생중인 섹션 확인
		FName NowSection = GetMesh ( )->GetAnimInstance ( )->Montage_GetCurrentSection ( NowMontage );

		//대쉬 공격 섹션이 아닐경우
		if(NowSection != "DashAttack" )
		{ 
			//돌진 공격 애니메이션이 나오도록 재생
			//해당 몽타주 섹션으로 이동
			AnimInstance->Montage_JumpToSection ( FName ( "DashAttack" ) , NowMontage );
		}
	}
}

void ACBossEnemy::DashAttackEnd()
{
	//돌진은 완료했다고 bool값 false로 전환
	FSMComponent->IsSetDashAttackLocation = false;
	//Navmesh를 이용하여 다시 이동하게 만듦
	GetCharacterMovement()->SetMovementMode ( MOVE_Walking );
	//이동 속도도 다시 원래 추적 속도로 되돌려줌
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	//추적 시간도 초기화
	FSMComponent->CurChaseTime = 0.0f;
	//대쉬 공격 준비 초기화
	FSMComponent->IsReadyDashAttack = false;
	//불 초기화
	FSMComponent->IsLowDist = false;
}

float ACBossEnemy::SetRateDown(UAnimMontage* CurrentMontage, FName CurrentSection)
{
	//몽타주에 해당하는 배열 찾기
	if (TArray<FMontageRateScale>* RateScales = MontageScaleMap.Find(CurrentMontage))
	{
		//섹션 이름에 맞는 Rate Scale값 찾기
		for (const FMontageRateScale& RateScale : *RateScales)
		{
			if (RateScale.SectionName == CurrentSection)
			{
				return RateScale.RateScale;
			}
		}
	}

	//매핑이 없으면 기본값 전환
	float DefaultRateScale = 1.0f;
	return	DefaultRateScale;
}

bool ACBossEnemy::SetGuardBool(UAnimMontage* CurrentMontage, FName CurrentSection)
{
	//몽타주에 해당하는 배열 찾기
	if (TArray<FMontageRateScale>* GuardBools = MontageScaleMap.Find(CurrentMontage))
	{
		//섹션 이름에 맞는 Rate Scale값 찾기
		for (const FMontageRateScale& IsGuard : *GuardBools)
		{
			if (IsGuard.SectionName == CurrentSection)
			{
				return IsGuard.IsGuard;
			}
		}
	}

	//매핑이 없으면 기본값 전환
	return true;
}

void ACBossEnemy::Hitted()
{
	//사망 상태면 안되게 막음
	if (FSMComponent->State == EBossState::DIE) return;

	//사이드 이동 중이면 해당 이동을 해제
	if (FSMComponent->IsSideMoveSetting)
	{
		/*FSMComponent->TotalMoveDistance = 0.0f;*/
		FSMComponent->CurSideMoveTime = 0.0f;
		FSMComponent->SideDirection = 0.0f;
		FSMComponent->IsSideMoveSetting = false;
	}
	
	if (!!Damage.Event && !!Damage.Event->HitData) {
		FHitData* data = Damage.Event->HitData;

		if (CurShieldAmount <= 0.0f)
		{
			if(ShieldBreakHit < 2) ++ShieldBreakHit;

			if(FSMComponent->State == EBossState::BREAK && ShieldBreakHit >= 2)
			{
				FSMComponent->State = EBossState::ATTACK;
				FSMComponent->AttackState = EBossATTACKState::NONE;
				FSMComponent->SetATKState= ESetATKState::SETATKNONE;
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

float ACBossEnemy::TakeDamage(float TakeDamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(TakeDamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Damage.Power = damage;
	Damage.Character = Cast<ACharacter>(EventInstigator->GetPawn());
	Damage.Causer = DamageCauser;
	Damage.Event = (FActionDamageEvent*)&DamageEvent;
	CLog::Log(Damage.Power);
	CLog::Log(Damage.Causer);

	bool IsKatana = Damage.Causer->GetName().Contains(TEXT("Katana"));

	if (IsKatana)
	{
		//사망 상태면 안되게 막음
		if (FSMComponent->State == EBossState::DIE) return 0;

		//카운터 가드 상태일때
		if (FSMComponent->AttackState == EBossATTACKState::COUNTERATTACK && AnimInstance->Montage_GetCurrentSection(AnimInstance->GetCurrentActiveMontage()) != FName("Counter"))
		{
			//정면에서 맞아서 카운터 공격이 나가면 뒤에 데미지 처리가 안되게 막음
			if (OnGuardCollision())
			{
				return 0;
			}
		}

		//만약 현재 필살기 준비 상태이면
		if (IsReadySPAttack)
		{
			//데미지 10의 비율로 체력 감소
			/*CurHP -= 10;*/
			SetHP(Damage.Power);
			FSMComponent->SetSPDamage(Damage.Power); //데미지 값 임시
		}

		//현재 쉴드가 있을 경우
		if (CurShieldAmount > 0)
		{
			//현재 어떠한 공격 애니메이션이 재생 중이라면
			if (AnimInstance->Montage_IsPlaying(AM_ComboAttack_01) || AnimInstance->Montage_IsPlaying(AM_ComboAttack_02) || AnimInstance->Montage_IsPlaying(AM_RangedAttack) || AnimInstance->Montage_IsPlaying(AM_DashAttack) || AnimInstance->Montage_IsPlaying(AM_SPAttack) || AnimInstance->Montage_IsPlaying(AM_Guard))
			{
				//쉴드와 체력의 감소를 4:2의 비율로 감소함
				/*ShieldAmount -= 10.0f * 0.4f;*/
				SetShieldAmount(Damage.Power);
				/*CurHP -= 10.0f * 0.2f;*/
				/*SetHP(10.0f * Damage.Power);*/

				//체력이 0이하가 됐을 경우
				if (CurHP <= 0)
				{
					//보스 사망
					FSMComponent->State = EBossState::DIE;

					return 0;
				}

				//쉴드가 0이하가 됐을 경우
				if (CurShieldAmount <= 0)
				{
					// -가 된 쉴드 게이지만큼 체력을 깎아줌
					/*CurHP += ShieldAmount;*/
					SetHP(-CurShieldAmount);

					if (FSMComponent->State != EBossState::BREAK)
					{
						//브레이크 상태로 됨
						FSMComponent->State = EBossState::BREAK;
					}
				}
			}

			//공격 모션이 재생중이 아니라면
		else
		{
				//쉴드가 있는 경우에는 검으로 막는 애니메이션 재생
				//브레이크 상태가 아니면 재생되게, 나중에 조건 바꾸기
				if (CurShieldAmount > 0 && AnimInstance->Montage_GetCurrentSection(AnimInstance->GetCurrentActiveMontage()) != FName("Counter") /*FSMComponent->AttackState != EBossATTACKState::COUNTERATTACK*/ && FSMComponent->State != EBossState::BREAK && !AnimInstance->Montage_IsPlaying(AM_Guard))
				{
					AnimInstance->Montage_Play(AM_ShieldHit);
					FSMComponent->CurSideMoveTime = 0.0f;
					FSMComponent->SideDirection = 0.0f;
					FSMComponent->SetATKState = ESetATKState::SETATKNONE;
				}

				//쉴드는 데미지의 값 만큼 감소
				/*ShieldAmount -= 5;*/
				SetShieldAmount(Damage.Power);

				//쉴드 카운터 공격 조건을 체크하는 카운트를 5초로 초기화
				GuardingTime = 5.0f;

				//가드 애니메이션이 재생 중이 아니라면 
				if (!AnimInstance->Montage_IsPlaying(AM_Guard))
				{
					//해당 상태에서 맞은 횟수를 카운트
					ShieldHitCount++;
				}

				//쉴드가 0이하가 됐을 경우
				if (CurShieldAmount <= 0)
				{
					// -가 된 쉴드 게이지만큼 체력을 깎아줌
					/*CurHP += ShieldAmount;*/
					SetHP(-CurShieldAmount);

					//체력이 0이하가 됐을 경우
					if (CurHP <= 0)
					{
						//보스 사망
						FSMComponent->State = EBossState::DIE;
						return 0;
					}

					if (FSMComponent->State != EBossState::BREAK)
					{
						//브레이크 상태로 됨
						FSMComponent->State = EBossState::BREAK;
					}
					//아래 코드가 실행 안되게 리턴
					return 0;
				}

				if (ShieldHitCount >= ShieldHitCounter)
				{
					//현재 몽타주가 재생중인 섹션 확인
					FName NowSection = AnimInstance->Montage_GetCurrentSection(AnimInstance->GetCurrentActiveMontage());

					if (NowSection != "Counter")
					{
						//가드 중 카운터 공격이 나오도록 섹션 점프
						AnimInstance->Montage_JumpToSection(FName("Counter"), AM_ShieldHit);
					}
				}
			}
		}

		//현재 쉴드가 없는 경우
		else
		{
			//체력이 데미지 10의 비율로 감소
			/*CurHP -= 20;*/
			SetHP(Damage.Power);

			//0이하가 됐을 경우
			if (CurHP <= 0.0f)
			{
				//사망 처리
				FSMComponent->State = EBossState::DIE;

				return 0;
			}
		}
	}

	else
	{
		bool IsGreatSword = Damage.Causer->GetName().Contains(TEXT("GreateSword"));

		if (IsGreatSword)
		{
			//사망 상태면 안되게 막음
			if (FSMComponent->State == EBossState::DIE) return 0;

			//카운터 가드 상태일때
			if (FSMComponent->AttackState == EBossATTACKState::COUNTERATTACK)
			{
				//정면에서 맞아서 카운터 공격이 나가면 뒤에 데미지 처리가 안되게 막음
				if (OnGuardCollision())
				{
					return 0;
				}
			}

			//만약 현재 필살기 준비 상태이면
			if (IsReadySPAttack)
			{
				//데미지 10의 비율로 체력 감소
				/*CurHP -= 10;*/
				SetHP(Damage.Power);
				FSMComponent->SetSPDamage(Damage.Power); //데미지 값 임시
			}

			//현재 쉴드가 있을 경우
			if (CurShieldAmount > 0)
			{
				//현재 어떠한 공격 애니메이션이 재생 중이라면
				if (AnimInstance->Montage_IsPlaying(AM_ComboAttack_01) || AnimInstance->Montage_IsPlaying(AM_ComboAttack_02) || AnimInstance->Montage_IsPlaying(AM_RangedAttack) || AnimInstance->Montage_IsPlaying(AM_DashAttack))
				{
					//쉴드와 체력의 감소를 4:2의 비율로 감소함
					/*ShieldAmount -= 10.0f * 0.8f;*/
					SetShieldAmount(Damage.Power);
					/*CurHP -= 10.0f * 0.2f;*/
					/*SetHP(10.0f * 0.2f);*/

					//체력이 0이하가 됐을 경우
					if (CurHP <= 0)
					{
						//보스 사망
						FSMComponent->State = EBossState::DIE;

						return 0;
					}

					//쉴드가 0이하가 됐을 경우
					if (CurShieldAmount <= 0)
					{
						// -가 된 쉴드 게이지만큼 체력을 깎아줌
						/*CurHP += ShieldAmount;*/
						SetHP(-CurShieldAmount);

						if (FSMComponent->State != EBossState::BREAK)
						{
							//브레이크 상태로 됨
							FSMComponent->State = EBossState::BREAK;
						}
					}
				}

				//공격 모션이 재생중이 아니라면
				else
				{
					//쉴드가 있는 경우에는 검으로 막는 애니메이션 재생
					//브레이크 상태가 아니면 재생되게, 나중에 조건 바꾸기
					if (CurShieldAmount > 0 && AnimInstance->Montage_GetCurrentSection(AnimInstance->GetCurrentActiveMontage()) != FName("Counter") && FSMComponent->State != EBossState::BREAK)
					{
						AnimInstance->Montage_Play(AM_ShieldHit);
					}

					//쉴드는 데미지의 값 만큼 감소
					SetShieldAmount(Damage.Power);

					//쉴드 카운터 공격 조건을 체크하는 카운트를 5초로 초기화
					GuardingTime = 5.0f;
					//해당 상태에서 맞은 횟수를 카운트
					ShieldHitCount++;

					//쉴드가 0이하가 됐을 경우
					if (CurShieldAmount <= 0)
					{
						// -가 된 쉴드 게이지만큼 체력을 깎아줌
						/*CurHP += ShieldAmount;*/
						SetHP(-CurShieldAmount);

						//체력이 0이하가 됐을 경우
						if (CurHP <= 0)
						{
							//보스 사망
							FSMComponent->State = EBossState::DIE;
							return 0;
						}

						if (FSMComponent->State != EBossState::BREAK)
						{
							//브레이크 상태로 됨
							FSMComponent->State = EBossState::BREAK;
						}
						//아래 코드가 실행 안되게 리턴
						return 0;
					}

					if (ShieldHitCount >= ShieldHitCounter)
					{
						//현재 몽타주가 재생중인 섹션 확인
						FName NowSection = AnimInstance->Montage_GetCurrentSection(AnimInstance->GetCurrentActiveMontage());

						if (NowSection != "Counter")
						{
							//가드 중 카운터 공격이 나오도록 섹션 점프
							AnimInstance->Montage_JumpToSection(FName("Counter"), AM_ShieldHit);
						}
					}
				}
			}

			//현재 쉴드가 없는 경우
			else
			{
				//체력이 데미지 10의 비율로 감소
				/*CurHP -= 5;*/
				SetHP(Damage.Power);

				//0이하가 됐을 경우
				if (CurHP <= 0.0f)
				{
					//사망 처리
					FSMComponent->State = EBossState::DIE;

					return 0;
				}
			}
		}
	}

	Hitted();
	return TakeDamageAmount;
}

void ACBossEnemy::Hit(FString Name)
{
// 	UE_LOG(LogTemp, Log, TEXT("Actor Name: %s"), *Name);
// 
// 	bool IsKatana = Name.Contains(TEXT("Katana"));
// 
// 	if (IsKatana)
// 	{
// 		//사망 상태면 안되게 막음
// 		if (FSMComponent->State == EBossState::DIE) return;
// 
// 		//카운터 가드 상태일때
// 		if (FSMComponent->AttackState == EBossATTACKState::COUNTERATTACK && AnimInstance->Montage_GetCurrentSection(AnimInstance->GetCurrentActiveMontage()) != FName("Counter"))
// 		{
// 			//정면에서 맞아서 카운터 공격이 나가면 뒤에 데미지 처리가 안되게 막음
// 			if (OnGuardCollision())
// 			{
// 				return;
// 			}
// 		}
// 
// 		//만약 현재 필살기 준비 상태이면
// 		if (IsReadySPAttack)
// 		{
// 			//데미지 10의 비율로 체력 감소
// 			/*CurHP -= 10;*/
// 			SetHP(10);
// 			FSMComponent->SetSPDamage(10); //데미지 값 임시
// 		}
// 
// 		//현재 쉴드가 있을 경우
// 		if (CurShieldAmount > 0)
// 		{
// 			//현재 어떠한 공격 애니메이션이 재생 중이라면
// 			if (AnimInstance->Montage_IsPlaying(AM_ComboAttack_01) || AnimInstance->Montage_IsPlaying(AM_ComboAttack_02) || AnimInstance->Montage_IsPlaying(AM_RangedAttack) || AnimInstance->Montage_IsPlaying(AM_DashAttack) || AnimInstance->Montage_IsPlaying(AM_SPAttack) || AnimInstance->Montage_IsPlaying(AM_Guard))
// 			{
// 				//쉴드와 체력의 감소를 4:2의 비율로 감소함
// 				/*ShieldAmount -= 10.0f * 0.4f;*/
// 				SetShieldAmount(10.0f * /*0.4f*/Damage.Power);
// 				/*CurHP -= 10.0f * 0.2f;*/
// 				/*SetHP(10.0f * Damage.Power);*/
// 
// 				//체력이 0이하가 됐을 경우
// 				if (CurHP <= 0)
// 				{
// 					//보스 사망
// 					FSMComponent->State = EBossState::DIE;
// 
// 					return;
// 				}
// 
// 				//쉴드가 0이하가 됐을 경우
// 				if (CurShieldAmount <= 0)
// 				{
// 					// -가 된 쉴드 게이지만큼 체력을 깎아줌
// 					/*CurHP += ShieldAmount;*/
// 					SetHP(-CurShieldAmount);
// 
// 					if (FSMComponent->State != EBossState::BREAK)
// 					{
// 						//브레이크 상태로 됨
// 						FSMComponent->State = EBossState::BREAK;
// 					}
// 				}
// 			}
// 
// 			//공격 모션이 재생중이 아니라면
// 			else
// 			{
// 				//쉴드가 있는 경우에는 검으로 막는 애니메이션 재생
// 				//브레이크 상태가 아니면 재생되게, 나중에 조건 바꾸기
// 				if (CurShieldAmount > 0 && AnimInstance->Montage_GetCurrentSection(AnimInstance->GetCurrentActiveMontage()) != FName("Counter") /*FSMComponent->AttackState != EBossATTACKState::COUNTERATTACK*/ && FSMComponent->State != EBossState::BREAK && !AnimInstance->Montage_IsPlaying(AM_Guard))
// 				{
// 					AnimInstance->Montage_Play(AM_ShieldHit);
// 				}
// 
// 				//쉴드는 데미지의 값 만큼 감소
// 				/*ShieldAmount -= 5;*/
// 				SetShieldAmount(1.0f * Damage.Power);
// 
// 				//쉴드 카운터 공격 조건을 체크하는 카운트를 5초로 초기화
// 				GuardingTime = 5.0f;
// 
// 				//가드 애니메이션이 재생 중이 아니라면 
// 				if (!AnimInstance->Montage_IsPlaying(AM_Guard))
// 				{
// 					//해당 상태에서 맞은 횟수를 카운트
// 					ShieldHitCount++;
// 				}
// 
// 				//쉴드가 0이하가 됐을 경우
// 				if (CurShieldAmount <= 0)
// 				{
// 					// -가 된 쉴드 게이지만큼 체력을 깎아줌
// 					/*CurHP += ShieldAmount;*/
// 					SetHP(-CurShieldAmount);
// 
// 					//체력이 0이하가 됐을 경우
// 					if (CurHP <= 0)
// 					{
// 						//보스 사망
// 						FSMComponent->State = EBossState::DIE;
// 						return;
// 					}
// 
// 					if (FSMComponent->State != EBossState::BREAK)
// 					{
// 						//브레이크 상태로 됨
// 						FSMComponent->State = EBossState::BREAK;
// 					}
// 					//아래 코드가 실행 안되게 리턴
// 					return;
// 				}
// 
// 				if (ShieldHitCount >= ShieldHitCounter)
// 				{
// 					//현재 몽타주가 재생중인 섹션 확인
// 					FName NowSection = AnimInstance->Montage_GetCurrentSection(AnimInstance->GetCurrentActiveMontage());
// 
// 					if (NowSection != "Counter")
// 					{
// 						//가드 중 카운터 공격이 나오도록 섹션 점프
// 						AnimInstance->Montage_JumpToSection(FName("Counter"), AM_ShieldHit);
// 					}
// 				}
// 			}
// 		}
// 
// 		//현재 쉴드가 없는 경우
// 		else
// 		{
// 			//체력이 데미지 10의 비율로 감소
// 			/*CurHP -= 20;*/
// 			SetHP(20);
// 
// 			//0이하가 됐을 경우
// 			if (CurHP <= 0.0f)
// 			{
// 				//사망 처리
// 				FSMComponent->State = EBossState::DIE;
// 
// 				return;
// 			}
// 		}
// 	}
// 
// 	else
// 	{
// 		bool IsGreatSword = Name.Contains(TEXT("GreateSword"));
// 
// 		if (IsGreatSword)
// 		{
// 			GEngine->AddOnScreenDebugMessage(80, 1.0f, FColor::Red, TEXT("Greate Sword Hitted"));
// 
// 			//사망 상태면 안되게 막음
// 			if (FSMComponent->State == EBossState::DIE) return;
// 
// 			//카운터 가드 상태일때
// 			if (FSMComponent->AttackState == EBossATTACKState::COUNTERATTACK)
// 			{
// 				//정면에서 맞아서 카운터 공격이 나가면 뒤에 데미지 처리가 안되게 막음
// 				if (OnGuardCollision())
// 				{
// 					return;
// 				}
// 			}
// 
// 			//만약 현재 필살기 준비 상태이면
// 			if (IsReadySPAttack)
// 			{
// 				//데미지 10의 비율로 체력 감소
// 				/*CurHP -= 10;*/
// 				SetHP(10);
// 				FSMComponent->SetSPDamage(10); //데미지 값 임시
// 			}
// 
// 			//현재 쉴드가 있을 경우
// 			if (CurShieldAmount > 0)
// 			{
// 				//현재 어떠한 공격 애니메이션이 재생 중이라면
// 				if (AnimInstance->Montage_IsPlaying(AM_ComboAttack_01) || AnimInstance->Montage_IsPlaying(AM_ComboAttack_02) || AnimInstance->Montage_IsPlaying(AM_RangedAttack) || AnimInstance->Montage_IsPlaying(AM_DashAttack))
// 				{
// 					//쉴드와 체력의 감소를 4:2의 비율로 감소함
// 					/*ShieldAmount -= 10.0f * 0.8f;*/
// 					SetShieldAmount(10.0f * 0.8f);
// 					/*CurHP -= 10.0f * 0.2f;*/
// 					SetHP(10.0f * 0.2f);
// 
// 					//체력이 0이하가 됐을 경우
// 					if (CurHP <= 0)
// 					{
// 						//보스 사망
// 						FSMComponent->State = EBossState::DIE;
// 
// 						return;
// 					}
// 
// 					//쉴드가 0이하가 됐을 경우
// 					if (CurShieldAmount <= 0)
// 					{
// 						// -가 된 쉴드 게이지만큼 체력을 깎아줌
// 						/*CurHP += ShieldAmount;*/
// 						SetHP(-CurShieldAmount);
// 
// 						if (FSMComponent->State != EBossState::BREAK)
// 						{
// 							//브레이크 상태로 됨
// 							FSMComponent->State = EBossState::BREAK;
// 						}
// 					}
// 				}
// 
// 				//공격 모션이 재생중이 아니라면
// 				else
// 				{
// 					//쉴드가 있는 경우에는 검으로 막는 애니메이션 재생
// 					//브레이크 상태가 아니면 재생되게, 나중에 조건 바꾸기
// 					if (CurShieldAmount > 0 && AnimInstance->Montage_GetCurrentSection(AnimInstance->GetCurrentActiveMontage()) != FName("Counter") && FSMComponent->State != EBossState::BREAK)
// 					{
// 						AnimInstance->Montage_Play(AM_ShieldHit);
// 					}
// 
// 					//쉴드는 데미지의 값 만큼 감소
// 					SetShieldAmount(15.0f);
// 
// 					//쉴드 카운터 공격 조건을 체크하는 카운트를 5초로 초기화
// 					GuardingTime = 5.0f;
// 					//해당 상태에서 맞은 횟수를 카운트
// 					ShieldHitCount++;
// 
// 					//쉴드가 0이하가 됐을 경우
// 					if (CurShieldAmount <= 0)
// 					{
// 						// -가 된 쉴드 게이지만큼 체력을 깎아줌
// 						/*CurHP += ShieldAmount;*/
// 						SetHP(-CurShieldAmount);
// 
// 						//체력이 0이하가 됐을 경우
// 						if (CurHP <= 0)
// 						{
// 							//보스 사망
// 							FSMComponent->State = EBossState::DIE;
// 							return;
// 						}
// 
// 						if (FSMComponent->State != EBossState::BREAK)
// 						{
// 							//브레이크 상태로 됨
// 							FSMComponent->State = EBossState::BREAK;
// 						}
// 						//아래 코드가 실행 안되게 리턴
// 						return;
// 					}
// 
// 					if (ShieldHitCount >= ShieldHitCounter)
// 					{
// 						//현재 몽타주가 재생중인 섹션 확인
// 						FName NowSection = AnimInstance->Montage_GetCurrentSection(AnimInstance->GetCurrentActiveMontage());
// 
// 						if (NowSection != "Counter")
// 						{
// 							//가드 중 카운터 공격이 나오도록 섹션 점프
// 							AnimInstance->Montage_JumpToSection(FName("Counter"), AM_ShieldHit);
// 						}
// 					}
// 				}
// 			}
// 
// 			//현재 쉴드가 없는 경우
// 			else
// 			{
// 				//체력이 데미지 10의 비율로 감소
// 				/*CurHP -= 5;*/
// 				SetHP(5);
// 
// 				//0이하가 됐을 경우
// 				if (CurHP <= 0.0f)
// 				{
// 					//사망 처리
// 					FSMComponent->State = EBossState::DIE;
// 
// 					return;
// 				}
// 			}
// 		}
// 	}

	


	//구버전
// 	//가드가 성공한 경우에는 데미지 처리 및 피격 애니메이션이 안 나오도록 만듦
// 	if (IsGuardSucssess)
// 		return;
// 
// // 	if (Weapon)
// // 	{
// 		GEngine->AddOnScreenDebugMessage(80, 1.0f, FColor::Red, TEXT("Hit Boss"));
// 
// 		//현재 필살기를 공격하려고 준비중이라면
// 		if (IsReadySPAttack)
// 		{
// 			//데미지를 저장
// 			FSMComponent->SetSPDamage(10.0f);
// 		}
// 
// 		//공격을 맞았을 때 쉴드 게이지가 있다면
// 		if (ShieldAmount > 0.0f)
// 		{
// 			//쉴드게이지가 감소하도록 설정
// 			ShieldAmount -= /*Damage*/10.0f;
// 
// 			AnimInstance->Montage_Play(AM_ShieldHit);
// 
// 			//쉴드 상태에서 몇번 맞았는지 체크함
// 			ShieldHitCount++;
// 
// 			//만약 일정 횟수 이상 맞았을 경우
// 			if (ShieldHitCount >= ShieldHitCounter)
// 			{
// 				AnimInstance->Montage_JumpToSection(FName("Counter"), AM_ShieldHit);
// 
// 				//맞은 횟수 초기화
// 				ShieldHitCount = 0;
// 			}
// 
// 			//만약 쉴드게이지 감소되어서 0이 된다면
// 			if (ShieldAmount <= 0.0f)
// 			{
// 				//에너미가 휘청거리는 애니메이션 출력? / 단 필살기 준비중에는 일어나지 않게 / 브레이크 상태에서도 나오지 않게
// 				if (FSMComponent->AttackState != EBossATTACKState::SPATTACK || FSMComponent->State != EBossState::BREAK)
// 				{
// 					GEngine->AddOnScreenDebugMessage(81, 1.0f, FColor::Red, TEXT("Boss Shield Break!!"));
// 				}
// 
// 				//쉴드 게이지가 -의 값이 되면 해당 값 만큼 체력을 깎게 만듦
// 				CurHP += ShieldAmount;
// 
// 				return;
// 			}
// 
// 			//가드 애니메이션이 나오도록 만들어주기 / 단 필살기 준비중에는 일어나지 않게 / 브레이크 상태에서도 나오지 않게
// 			if (FSMComponent->AttackState != EBossATTACKState::SPATTACK || FSMComponent->State != EBossState::BREAK)
// 			{
// 				/*GEngine->AddOnScreenDebugMessage ( 82 , 1.0f , FColor::Red , TEXT ( "Boss Gard Animation" ) );*/
// 
// 
// 			}
// 		}
// 
// 		//쉴드 게이지가 없는 상태에서 맞았을 경우
// 		else
// 		{
// 			//체력을 깎게 만들어줌
// 			CurHP -= /*Damage*/10.0f;
// 
// 			//체력이 깎였는데 0 이하가 된다면
// 			if (CurHP <= 0.0f)
// 			{
// 				//사망 상태로 만듦
// 				/*FSMComponent->State = EBossState::DIE;*/
// 
// 				//만약 에너미 매니저가 있다면
// 				if (Manager)
// 				{
// 					Manager->RemoveEnemiesList(MyUniqeID, IsCanAttack);
// 				}
// 
// 				GEngine->AddOnScreenDebugMessage(83, 1.0f, FColor::Red, TEXT("Boss is Dead"));
// 			}
// 
// 			//피격 애니메이션이 나오게 만듦 / 단 필살기 준비중에는 일어나지 않게 / 브레이크 상태에서도 나오지 않게
// 			if (FSMComponent->AttackState != EBossATTACKState::SPATTACK || FSMComponent->State != EBossState::BREAK)
// 			{
// 				GEngine->AddOnScreenDebugMessage(84, 1.0f, FColor::Red, TEXT("Boss Hit Damage"));
// 			}
// 		}
// 	/*}*/
}

void ACBossEnemy::OnPlayerHealed()
{
	CLog::Log("OnPlayerHealed");

	if(FSMComponent->State == EBossState::IDLE) return;

	//다른 공격 모션을 하고 있다면 실행 안되게 막아주기
	if(FSMComponent->AttackState == EBossATTACKState::NONE || FSMComponent->SetATKState == ESetATKState::SETATKNONE)
	{ 
		FSMComponent->State = EBossState::ATTACK;
		FSMComponent->AttackState = EBossATTACKState::RANGEDATTACK;
	}
}

void ACBossEnemy::Start(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (BossStart)
	return;

	ACPlayer* Player = Cast<ACPlayer>(OtherActor);
	
	if(Player) 
	{
		BossStart = true;;
		if(AnimInstance)
			AnimInstance->IsStartBoss = true;
		
		if (!BossUIClass)return;
		BossUI=CreateWidget<UBossWidget>(GetWorld(),BossUIClass);
		BossUI->SetOwner(this);
		if(BossUI)
		BossUI->AddToViewport();
		
		if (StartCollision)
			StartCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}


// void ACBossEnemy::DashPlayerHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	UE_LOG(LogTemp, Warning, TEXT("OtherActor Class: %s"), *GetNameSafe(OtherActor));
// 
// 	ACPlayer* Player = Cast<ACPlayer>(OtherActor);
// 
// 	if (Player)
// 	{
// 		SpawnWeapon->DashAttackHitCheck();
// 	}
// }

void ACBossEnemy::SetHP(float value)
{
	CurHP = CurHP - value;

	OnDelegateHP.Broadcast(CurHP);
}

void ACBossEnemy::SetShieldAmount(float value)
{
	CurShieldAmount = CurShieldAmount - value;

	OnDelegateShield.Broadcast(CurShieldAmount);
}

void ACBossEnemy::RestartGame()
{
	if (BossUI->IsInViewport())
	{
		BossUI->RemoveFromParent();
	}

	/*SpawnWeapon->SetActorEnableCollision(ECollisionEnabled::NoCollision);*/

	SetActorLocation(FirstLocation);
	SetActorRotation(FirstRotation);

	StartCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BossStart = false;

	FSMComponent->State = EBossState::IDLE;
	FSMComponent->AttackState = EBossATTACKState::NONE;
	FSMComponent->SetATKState = ESetATKState::SETATKNONE;

	//모든 몽타주 재생을 멈춤
	AnimInstance->StopAllMontages(0.4f);
	
	AnimInstance->IsStartBoss = false;
	AnimInstance->IsPlayingIdle = false;
	AnimInstance->MoveDirection = 0;
	AnimInstance->Speed = 0;

	const FCEnemyStats& Stats = StatsAsset->Stats;

	CurHP = Stats.MaxHP;
	CurShieldAmount = Stats.ShieldAmount;
	CurBreakTime = 0.0f;
	IsDashAttackHit = false;
	GuardGage = 0.0f;
	IsGuardSucssess = false;
	IsReadySPAttack = false;
	ShieldHitCount = 0;
	ShieldBreakHit = 0;
	IsSPRun = false;
	IsSPFirstATKHit = false;
	GuardingTime = 0.0f;
	BackstepPercent = 60;

	FSMComponent->CurChaseTime = 0.0f;
	FSMComponent->TotalMoveDistance = 0.0f;
	FSMComponent->LastLocation = GetActorLocation();
	FSMComponent->IsSideMoveSetting = false;
	FSMComponent->RangedAttackCount = 0;
	FSMComponent->IsSetDashAttackLocation = false;
	FSMComponent->IsReadyDashAttack = false;
	FSMComponent->IsLowDist = false;
	FSMComponent->DashTimer = 0.0f;
	FSMComponent->OnSpDamageAmount = 0.0f;
	FSMComponent->IsFirstSPAttack = false;
	FSMComponent->CurAttackStack = 0;
	FSMComponent->CurComboAttackTime = 0.0f;
	FSMComponent->CurSideMoveTime = 0.0f;
	FSMComponent->SideDirection = 0.0f;
}

void ACBossEnemy::LoadStatsFromAsset ( )
{
	Super::LoadStatsFromAsset();

}

void ACBossEnemy::SetSPColli()
{
	//보스의 전방 방향을 받아와 현재 위치에서 전방으로 떨어진 위치에 소환
	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 350.0f;

	SpawnSpColli->SetActive(true, SpawnLocation);
}

void ACBossEnemy::SPAttack()
{
	//장판 내에 있는 엑터들 담아두기
	TArray<AActor*> OverlappingActors;
	SpawnSpColli->GetOverlappingActors(OverlappingActors, ACharacter::StaticClass());

	for (AActor* Actor : OverlappingActors)
	{	
		//담아두고 여기에 플레이어가 있을 경우
		ACPlayer* Player = Cast<ACPlayer>(Actor);
		if (Player)
		{
			//플레이어에게 데미지 주기
			HitData->HitDatas[SpawnWeapon->HitNumber].SendDamage(this, SpawnWeapon, Player);
		}
	}
}

void ACBossEnemy::EndSPAttack()
{
	//콜리전 비활성화
	SpawnSpColli->SetActive(false, GetActorLocation());
}

void ACBossEnemy::InitAttackTMap()
{
	AttackSectionToEnumMap = {
		{ FName("ComboAttack_01_01"), EAttackType::Attack01_01},
		{ FName("ComboAttack_01_02"), EAttackType::Attack01_02},
		{ FName("ComboAttack_01_03"), EAttackType::Attack01_03},
		{ FName("ComboAttack_01_04"), EAttackType::Attack01_04},
		{ FName("ComboAttack_02_01"), EAttackType::Attack02_01},
		{ FName("ComboAttack_02_02"), EAttackType::Attack02_02},
		{ FName("ComboAttack_02_03"), EAttackType::Attack02_03},
		{ FName("ComboAttack_02_04"), EAttackType::Attack02_04}
	};
}