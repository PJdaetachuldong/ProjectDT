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

ACBossEnemy::ACBossEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(FName("TestEnemy"));
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACBossEnemy::EnemyHitDamage);

	FSMComponent = CreateDefaultSubobject<UCBossFSM>(TEXT("FSMComponent"));

	ConstructorHelpers::FObjectFinder<UAnimBlueprint> TempAnim (L"/Script/Engine.AnimBlueprint'/Game/ODH/Animation/Boss/ABP_BossAnim.ABP_BossAnim'");
	if ( TempAnim.Succeeded() )
	{
		GetMesh()->SetAnimInstanceClass(TempAnim.Object->GeneratedClass);
	}

	//일단 임시로 하는 발사 위치 설정
	ThrowPosition = CreateDefaultSubobject<USceneComponent>(L"ThrowPosition");
	ThrowPosition->SetupAttachment(RootComponent);
	//일단 임시로 하는 발사 위치 설정

	GuardCollComp = CreateDefaultSubobject<UBoxComponent>(L"GuardCollision");
	GuardCollComp->SetupAttachment(GetMesh());
	GuardCollComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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

// 	ConstructorHelpers::FObjectFinder<UAnimMontage> TempRanged ( L"/Script/Engine.AnimMontage'/Game/ODH/Animation/Boss/Montage/ComboAttack_01/AM_ComboAttack_01_01.AM_ComboAttack_01_01'" );
// 	if ( TempRanged.Succeeded ( ) )
// 	{
// 		AM_RangedAttack = TempRanged.Object;
// 	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> TempGuard ( L"/Script/Engine.AnimMontage'/Game/ODH/Animation/Boss/Montage/Guard/AM_Guard.AM_Guard'" );
	if ( TempGuard.Succeeded ( ) )
	{
		AM_Guard = TempGuard.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> TempSP ( L"/Script/Engine.AnimMontage'/Game/ODH/Animation/Boss/Montage/SPAttack/AM_SPAttack.AM_SPAttack'" );
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

	ConstructorHelpers::FObjectFinder<UHitDataAsset_BossToPlayer> TempHitData (L"/Script/Project_DT.HitDataAsset_BossToPlayer'/Game/ODH/DataAsset/DA_Hit.DA_Hit'");
	if (TempHitData.Succeeded())
	{
		HitData = TempHitData.Object;
	}
}

void ACBossEnemy::BeginPlay()
{
	Super::BeginPlay();

	//임시로 타겟을 그냥 플레이어로 지정
	Target = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	//임시로 타겟을 그냥 플레이어로 지정

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

	//원거리 공격을 최대 설정 값만큼 미리 생성함
	for ( int32 i = 0; i < MaxRangedAttackCount; ++i )
	{
		//스폰할 객체에 대한 스폰 옵션을 설정하는 구조체
		FActorSpawnParameters Params;
		//스폰 과정에 충돌이 생겨도 제자리에서 스폰할 수 있게 만듦
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//원거리 공격 오브젝트를 월드에 소환
		ACRangeAttack* RangedAttackObject = GetWorld()->SpawnActor<ACRangeAttack>(RangedAttackFactory, Params);
		//해당 원거리 공격 오브젝트 비활성화 처리
		RangedAttackObject->SetActive(false, FVector(0));
		//생성한 것을 오브젝트 풀에 넣음
		RangedAttackList.Add(RangedAttackObject);
	}

	AnimInstance = GetMesh()->GetAnimInstance();

	LoadStatsFromAsset();

	//몽타주 매핑 초기화
	InitializeMontageMap();
}

void ACBossEnemy::InitializeMontageMap()
{
	if (AM_ComboAttack_01)
	{
		TArray<FMontageRateScale> ComboAttack01Rates;
		ComboAttack01Rates.Add(FMontageRateScale{FName("ComboAttack_01_01"), 0.5f});
		ComboAttack01Rates.Add(FMontageRateScale{FName("ComboAttack_01_02"), 0.7f});
		ComboAttack01Rates.Add(FMontageRateScale{FName("ComboAttack_01_03"), 0.7f});
		ComboAttack01Rates.Add(FMontageRateScale{FName("ComboAttack_01_04"), 0.7f});
		MontageScaleMap.Add(AM_ComboAttack_01, ComboAttack01Rates);
	}
	
	if (AM_ComboAttack_02)
	{
		TArray<FMontageRateScale> ComboAttack02Rates;
		ComboAttack02Rates.Add(FMontageRateScale{FName("ComboAttack_02_01"), 0.7f});
		ComboAttack02Rates.Add(FMontageRateScale{FName("ComboAttack_02_02"), 0.7f});
		ComboAttack02Rates.Add(FMontageRateScale{FName("ComboAttack_02_03"), 0.7f});
		ComboAttack02Rates.Add(FMontageRateScale{FName("ComboAttack_02_04"), 0.7f});
		MontageScaleMap.Add(AM_ComboAttack_02, ComboAttack02Rates);
	}

	if (AM_DashAttack)
	{
		TArray<FMontageRateScale> DashAttackRates;
		DashAttackRates.Add(FMontageRateScale{FName("DashAttackReady"), 0.7f});
		DashAttackRates.Add(FMontageRateScale{FName("Run"), 0.7f});
		DashAttackRates.Add(FMontageRateScale{FName("DashAttack"), 0.7f});
		MontageScaleMap.Add(AM_DashAttack, DashAttackRates);
	}

// 	if (AM_RangedAttack)
// 	{
// 		TArray<FMontageRateScale> RangedAttackRates;
// 		RangedAttackRates.Add(FMontageRateScale{ FName("DashAttackReady"), 0.7f });
// 		RangedAttackRates.Add(FMontageRateScale{ FName("Run"), 0.7f });
// 		RangedAttackRates.Add(FMontageRateScale{ FName("DashAttack"), 0.7f });
//		MontageScaleMap.Add(AM_RangedAttack, RangedAttackRates);
// 	}

	if (AM_Guard)
	{
		TArray<FMontageRateScale> GuardRates;
		GuardRates.Add(FMontageRateScale{ FName("Guard_Start"), 0.7f });
		GuardRates.Add(FMontageRateScale{ FName("GuardStarting"), 0.7f });
		GuardRates.Add(FMontageRateScale{ FName("Guard_End"), 0.7f });
		GuardRates.Add(FMontageRateScale{ FName("Counter"), 0.7f });
		MontageScaleMap.Add(AM_Guard, GuardRates);
	}

	if (AM_SPAttack)
	{
		TArray<FMontageRateScale> SPAttackRates;
		SPAttackRates.Add(FMontageRateScale{ FName("SPAttackStart"), 0.7f });
		SPAttackRates.Add(FMontageRateScale{ FName("SPAttackLoop"), 0.7f });
		SPAttackRates.Add(FMontageRateScale{ FName("SPAttack"), 0.7f });
		MontageScaleMap.Add(AM_SPAttack, SPAttackRates);
	}

	if (AM_ShieldHit)
	{
		TArray<FMontageRateScale> GuardRates;
		GuardRates.Add(FMontageRateScale{ FName("Hit"), 0.7f });
		GuardRates.Add(FMontageRateScale{ FName("Interaction"), 0.7f });
		GuardRates.Add(FMontageRateScale{ FName("Counter"), 0.7f });
		GuardRates.Add(FMontageRateScale{ FName("End"), 0.7f });
		MontageScaleMap.Add(AM_ShieldHit, GuardRates);
	}
}

void ACBossEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

void ACBossEnemy::OnGuardCollision ( )
{
	//공격이 현재 에너미가 바라보는 방향에서 어느 각도로 맞았는지 체크하게 만듦
	//SphereTrace와 내적을 사용해서 플레이어의 공격 각도를 체크하는 방식
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool bHit = GetWorld()->SweepSingleByChannel(Hit, GetActorLocation(), GetActorLocation(), FQuat::Identity, ECC_GameTraceChannel4 , FCollisionShape::MakeSphere(130.0f), Params);

	DrawDebugSphere ( GetWorld ( ) , GetActorLocation ( ) , 130.0f , 21 , FColor::Green , false , 0.1f );

	//일단 닿은게 플레이어 무기면 
	if ( bHit && Hit.GetActor()->IsA(ACAttachment::StaticClass()) )
	{
		//플레이어 무기와의 각도를 계산함
		FVector ToPlayer = (Hit.GetActor()->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
		
		float DotProduct = FVector::DotProduct(GetActorForwardVector(), ToPlayer);
		float AngleRad = FMath::Acos(DotProduct);
		float AngleDeg = FMath::RadiansToDegrees(AngleRad);

		//만약 각도가 전방 30도 사이라면, 정면에서 맞았을 경우
		if ( AngleDeg <= 30.0f )
		{
			//바로 카운터 공격이 동작하게 만듦
		/*	GEngine->AddOnScreenDebugMessage ( 111 , 1.0f , FColor::White , TEXT ( "Counter Attack!!!" ) );*/
			
			UAnimMontage* NowMontage = GetMesh()->GetAnimInstance ( )->GetCurrentActiveMontage ( );
			if ( !NowMontage )
			{
				return;
			}

			//현재 몽타주가 재생중인 섹션 확인
			FName NowSection = GetMesh()->GetAnimInstance ( )->Montage_GetCurrentSection ( NowMontage );

			//카운터 공격 애니메이션이 나오도록 재생
			//해당 몽타주 섹션으로 이동
			AnimInstance->Montage_JumpToSection(FName("Counter") , NowMontage);

			//혹시 모르니 데미지 처리 못하게 bool을 true로 변경해줌
			IsGuardSucssess = true;
			//공격 상태 NONE으로 변경
			FSMComponent->AttackState = EBossATTACKState::NONE;
			//시간 초기화
			FSMComponent->CurGuardTime = 0.0f;
			//가드 조건 초기화
			GuardGage = 0.0f;
			//뒤에 코드 작동이 안되게 리턴
			return;
		}

		//만약 각도가 100보다 크다면, 즉 후방 80도 사이에서 맞았을 경우
		else if ( AngleDeg >= 100.0f )
		{
			//자세가 흐트러지며 BREAK상태가 됨
			FSMComponent->AttackState = EBossATTACKState::NONE;
			FSMComponent->State = EBossState::BREAK;
			//시간 초기화
			FSMComponent->CurGuardTime = 0.0f;
			//가드 조건 초기화
			GuardGage = 0.0f;
			//뒤에 코드 작동이 안되게 리턴
			return;
		}
	}

	//만약 그냥 가드 시간이 끝났을 경우
	if ( FSMComponent->CurGuardTime >= FSMComponent->LimiteGuardTime )
	{
		UAnimMontage* NowMontage = GetMesh()->GetAnimInstance ( )->GetCurrentActiveMontage ( );
		if ( !NowMontage )
		{
			return;
		}

		//현재 몽타주가 재생중인 섹션 확인
		FName NowSection = GetMesh()->GetAnimInstance ( )->Montage_GetCurrentSection ( NowMontage );

		//카운터 공격 애니메이션이 나오도록 재생
		//해당 몽타주 섹션으로 이동
		AnimInstance->Montage_JumpToSection(FName("Guard_End"), NowMontage);

		/*GEngine->AddOnScreenDebugMessage ( 113 , 1.0f , FColor::White , TEXT ( "Guard Time Limite" ) );*/
		//그냥 공격상태를 NONE상태로 되돌림
		FSMComponent->AttackState = EBossATTACKState::NONE;
		//시간 초기화
		FSMComponent->CurGuardTime = 0.0f;
		//가드 조건 초기화
		GuardGage = 0.0f;
	}
}

void ACBossEnemy::OffGuardCollision ( )
{
	if ( GuardCollComp )
	{
		//공격 판정 콜리전 비활성화
		/*SwordCollComp->SetCollisionEnabled ( ECollisionEnabled::NoCollision );*/
	}
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

	DrawDebugSphere(GetWorld(), GetActorLocation(), 300.0f, 21, FColor::Green, false, 3.0f);

	if (bHit && Hit.GetActor()->IsA(ACPlayer::StaticClass()))
	{
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
				//true로 값을 전달
				return true;
			}
		}
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
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 40.0f);

	// Pawn 회전 설정
	SetActorRotation (NewRotation);
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

void ACBossEnemy::EnemyHitDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//가드가 성공한 경우에는 데미지 처리 및 피격 애니메이션이 안 나오도록 만듦
	if(IsGuardSucssess )
	return;

	ACAttachment* Weapon = Cast<ACAttachment> ( OtherActor );

	//맞은 위치를 기준으로 어떤 피격 애니메이션이 재생될지 방향값을 구하는 코드, 가드, 피격 상태일때 해당 코드를 써서 봐야함
	FVector HitLocation = SweepResult.Location;
    FVector EnemyLocation = GetActorLocation();
    FVector EnemyRightVector = GetActorRightVector();
    FVector ToHitLocation = (HitLocation -EnemyLocation).GetSafeNormal();
    float DotProduct = FVector::DotProduct(ToHitLocation, EnemyRightVector);
	//맞은 위치를 기준으로 어떤 피격 애니메이션이 재생될지 방향값을 구하는 코드, 가드, 피격 상태일때 해당 코드를 써서 봐야함

	if ( Weapon )
	{
		GEngine->AddOnScreenDebugMessage ( 80 , 1.0f , FColor::Red , TEXT ( "Hit Boss" ) );

		//현재 필살기를 공격하려고 준비중이라면
		if ( IsReadySPAttack )
		{
			//데미지를 저장
			FSMComponent->SetSPDamage(10.0f);
		}

		//공격을 맞았을 때 쉴드 게이지가 있다면
		if ( ShieldAmount > 0.0f )
		{
			//쉴드게이지가 감소하도록 설정
			ShieldAmount -= /*Damage*/10.0f;

			AnimInstance->Montage_Play ( AM_ShieldHit );

			//쉴드 상태에서 몇번 맞았는지 체크함
			ShieldHitCount++;

			//만약 일정 횟수 이상 맞았을 경우
			if ( ShieldHitCount >= ShieldHitCounter )
			{
				AnimInstance->Montage_JumpToSection ( FName ( "Counter" ) , AM_ShieldHit );

				//맞은 횟수 초기화
				ShieldHitCount = 0;
			}

			//만약 쉴드게이지 감소되어서 0이 된다면
			if ( ShieldAmount <= 0.0f )
			{
				//에너미가 휘청거리는 애니메이션 출력? / 단 필살기 준비중에는 일어나지 않게 / 브레이크 상태에서도 나오지 않게
				if ( FSMComponent->AttackState != EBossATTACKState::SPATTACK || FSMComponent->State != EBossState::BREAK )
				{
					GEngine->AddOnScreenDebugMessage ( 81 , 1.0f , FColor::Red , TEXT ( "Boss Shield Break!!" ) );
				}

				//쉴드 게이지가 -의 값이 되면 해당 값 만큼 체력을 깎게 만듦
				CurHP += ShieldAmount;

				return;
			}

			//가드 애니메이션이 나오도록 만들어주기 / 단 필살기 준비중에는 일어나지 않게 / 브레이크 상태에서도 나오지 않게
			if ( FSMComponent->AttackState != EBossATTACKState::SPATTACK || FSMComponent->State != EBossState::BREAK )
			{
				/*GEngine->AddOnScreenDebugMessage ( 82 , 1.0f , FColor::Red , TEXT ( "Boss Gard Animation" ) );*/

				
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
				/*FSMComponent->State = EBossState::DIE;*/

				//만약 에너미 매니저가 있다면
				if ( Manager )
				{
					Manager->RemoveEnemiesList ( MyUniqeID , IsCanAttack );
				}

				GEngine->AddOnScreenDebugMessage ( 83 , 1.0f , FColor::Red , TEXT ( "Boss is Dead" ) );
			}

			//피격 애니메이션이 나오게 만듦 / 단 필살기 준비중에는 일어나지 않게 / 브레이크 상태에서도 나오지 않게
			if ( FSMComponent->AttackState != EBossATTACKState::SPATTACK || FSMComponent->State != EBossState::BREAK )
			{
				GEngine->AddOnScreenDebugMessage ( 84 , 1.0f , FColor::Red , TEXT ( "Boss Hit Damage" ) );
			}
		}
	}
}

void ACBossEnemy::Hitted()
{
	Damage.Power = 0;
	
	if (!!Damage.Event && !!Damage.Event->HitData) {
		FHitData* data = Damage.Event->HitData;
		data->PlayMontage(this);
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
	Hitted();
	return TakeDamageAmount;
}

void ACBossEnemy::LoadStatsFromAsset ( )
{
	Super::LoadStatsFromAsset();

}
