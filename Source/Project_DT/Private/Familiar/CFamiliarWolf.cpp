// Fill out your copyright notice in the Description page of Project Settings.


#include "Familiar/CFamiliarWolf.h"
#include "Familiar/CWolfFSM.h"
#include "Familiar/CWolfAnimInstance.h"
#include "Engine/SkeletalMesh.h"

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
	

	WolfFSM = CreateDefaultSubobject<UCWolfFSM> ( TEXT ( "WolfFSM" ) );
	WolfAnim = Cast<UCWolfAnimInstance> ( GetMesh ( )->GetAnimInstance ( ) );

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
}

void ACFamiliarWolf::BeginPlay ( )
{
	Super::BeginPlay ( );

}

void ACFamiliarWolf::Tick ( float DeltaTime )
{
	Super::Tick ( DeltaTime );

}

void ACFamiliarWolf::SetOnDesPawn ( )
{
	IsCanAttack = false;

}

/*
void ACFamiliarWolf::Landed ( const FHitResult& Hit )
{
	WolfFSM->mUpState = EUpperState::Idle;
}
*/