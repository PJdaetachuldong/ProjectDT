// Fill out your copyright notice in the Description page of Project Settings.


#include "Familiar/CFamiliarDeer.h"
#include "Familiar/CDeerAnimInstance.h"
#include "Familiar/CDeerFSM.h"
#include "Engine/SkeletalMesh.h"

ACFamiliarDeer::ACFamiliarDeer ( )
{
	PrimaryActorTick.bCanEverTick = true;

#pragma region Components

	DeerComponent = GetMesh ( );

	ConstructorHelpers::FObjectFinder<USkeletalMesh>TmpBody ( TEXT ( "/Script/Engine.SkeletalMesh'/Game/KJY/Deer_Boss/Meshes/SKM_DeerBoss.SKM_DeerBoss'" ) );
	if ( TmpBody.Succeeded ( ) )
	{
		DeerComponent->SetSkeletalMesh ( TmpBody.Object );
		DeerComponent->SetRelativeLocation ( FVector ( 0.f ) );
		DeerComponent->SetRelativeRotation ( FRotator ( 0.f , -90.f , 0.f ) );

		DeerComponent->SetRelativeScale3D ( FVector ( 1.0f ) );
		DeerComponent->SetCollisionProfileName ( TEXT ( "DeerBody" ) );
	}


	DeerFSM = CreateDefaultSubobject<UCDeerFSM> ( TEXT ( "DeerFSM" ) );

	DeerAnim = Cast<UCDeerAnimInstance> ( GetMesh ( )->GetAnimInstance ( ) );

#pragma endregion Components
}

void ACFamiliarDeer::BeginPlay ( )
{
	Super::BeginPlay ( );

}

// Called every frame
void ACFamiliarDeer::Tick ( float DeltaTime )
{
	Super::Tick ( DeltaTime );

}