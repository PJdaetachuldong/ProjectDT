// Fill out your copyright notice in the Description page of Project Settings.


#include "Familiar/CFamiliarWolf.h"
#include "Familiar/CWolfFSM.h"
#include "Familiar/CWolfAnimInstance.h"
#include "Engine/SkeletalMesh.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

ACFamiliarWolf::ACFamiliarWolf ( )
{
	PrimaryActorTick.bCanEverTick = true;

#pragma region Components

	WolfComponent = GetMesh ( );
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TmpBody ( TEXT ("/Script/Engine.SkeletalMesh'/Game/KJY/Wolf/Mesh/SKM_Wolf.SKM_Wolf'") );
	if ( TmpBody.Succeeded ( ) )
	{
		WolfComponent->SetSkeletalMesh ( TmpBody.Object );
		WolfComponent->SetRelativeLocation ( FVector ( -20.f, 0.f, -48.f ) );
		WolfComponent->SetRelativeRotation ( FRotator ( 0.f, -90.f, 0.f ) );

		WolfComponent->SetRelativeScale3D ( FVector ( 1.0f ) );
		WolfComponent->SetCollisionProfileName ( TEXT ( "WolfBody" ) );
	}
	

	FSM = CreateDefaultSubobject<UCWolfFSM> ( TEXT ( "WolfFSM" ) );

	/*
	USkeletalMeshComponent* SkeletalMeshComp = GetMesh ( );
	if ( SkeletalMeshComp )
	{
		SkeletalMeshComp->bEnablePerPolyCollision = true;
		SkeletalMeshComp->SetCollisionEnabled ( ECollisionEnabled::QueryAndPhysics );
		SkeletalMeshComp->SetCollisionResponseToAllChannels ( ECR_Overlap );
		SkeletalMeshComp->SetCollisionProfileName ( TEXT ( "WolfSkeleton" ) );
	}
	*/

#pragma endregion Components

	InitBoxes();
	AttCollisionBite->OnComponentBeginOverlap.AddDynamic ( this , &ACFamiliarWolf::OnAttackOverlapBegin );
}

void ACFamiliarWolf::BeginPlay ( )
{
	Super::BeginPlay ( );

	Anim = Cast<UCWolfAnimInstance> ( GetMesh ( )->GetAnimInstance ( ) );
}

void ACFamiliarWolf::Tick ( float DeltaTime )
{
	Super::Tick ( DeltaTime );

	if ( IsOnBiteAtt == true)
	{ AttCollisionBite->SetCollisionEnabled ( ECollisionEnabled::QueryAndPhysics ); }
	else 
	{ 
		HitPawn.Empty ( );	// 공격 사이클이 끝났으니 HitPawn 지워주기
		AttCollisionBite->SetCollisionEnabled ( ECollisionEnabled::NoCollision ); 
	}
	

	if ( IsOnSpecialAtt == true )
	{ GetCapsuleComponent ( )->SetCollisionEnabled ( ECollisionEnabled::NoCollision ); }
	else
	{ GetCapsuleComponent ( )->SetCollisionEnabled ( ECollisionEnabled::QueryAndPhysics ); }

}

void ACFamiliarWolf::InitBoxes ( )
{

	AttCollisionBite = CreateDefaultSubobject<UBoxComponent>(TEXT("AttCollisionBite"));	// 머리
	AttCollisionBite->SetupAttachment( WolfComponent , TEXT("Socket_BiteDMGBox"));
	AttCollisionBite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttCollisionBite->SetCollisionResponseToAllChannels(ECR_Overlap);
	AttCollisionBite->SetCollisionProfileName(TEXT("Socket_BiteDMGBox"));
	AttCollisionBite->SetCollisionObjectType(ECC_Pawn);
	AttCollisionBite->SetRelativeLocation(FVector(90.f, 0.f, 0.f));
	AttCollisionBite->SetBoxExtent(FVector(30.f, 30.f, 30.f));

}

void ACFamiliarWolf::SetOnDesPawn ( )
{
	IsCanAttack = false;

}

void ACFamiliarWolf::OnAttOffProcess ( )
{
	IsOnBiteAtt = false;
	// Me->IsOnSpecialAtt = false;
}

void ACFamiliarWolf::Landed ( const FHitResult& Hit )
{
	// FSM->UpdateState(EUpperState::Idle);

	Anim->IsJumping = false;

	FSM->UpdateState(EJumpState::EndJump);

}

void ACFamiliarWolf::OnAttackOverlapBegin ( class UPrimitiveComponent* OverlappedComp , class AActor* OtherActor , class UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{
	ACEnemyBase* target = Cast<ACEnemyBase> ( OtherActor );
	// if ( !target || HitPawn.Contains ( target ) ) { return; }
	if ( !target ) { return; }

	if (IsOnBiteAtt == true)
	{
		//if ( HitPawn.Num ( ) > 0 ){ return; }
		UE_LOG ( LogTemp , Warning , TEXT ( "DMG_Test" ) );
		UGameplayStatics::ApplyDamage ( OtherActor , MeleeBiteDMG , nullptr , this , nullptr );
		HitPawn.AddUnique ( target );
	}
}
