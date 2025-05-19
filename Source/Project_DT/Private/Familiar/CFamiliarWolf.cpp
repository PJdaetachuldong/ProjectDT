// Fill out your copyright notice in the Description page of Project Settings.


#include "Familiar/CFamiliarWolf.h"
#include "Familiar/CWolfFSM.h"

ACFamiliarWolf::ACFamiliarWolf ( )
{
	PrimaryActorTick.bCanEverTick = true;

#pragma region Components

	WolfComponent = GetMesh ( );
	/*
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TmpBody ( TEXT ( "경로" ) );
	if ( TmpBody.Succeeded ( ) )
	{
		WolfComponent->SetSkeletalMesh ( TmpBody.Object );
		WolfComponent->SetRelativeLocation ( FVector ( 0.f ) );
		WolfComponent->SetRelativeRotation ( FRotator ( 0.f) );

		WolfComponent->SetRelativeScale3D ( FVector ( 1.f ) );
		WolfComponent->SetCollisionProfileName ( TEXT ( "WolfBody" ) );
	}
	*/

	WolfFSM = CreateDefaultSubobject<UCWolfFSM> ( TEXT ( "WolfFSM" ) );
	
	// Anim = Cast<UCSafiAnimInstance> ( GetMesh ( )->GetAnimInstance ( ) );

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

/*
void ACFamiliarWolf::Landed ( const FHitResult& Hit )
{
	WolfFSM->mUpState = EUpperState::Idle;
}
*/