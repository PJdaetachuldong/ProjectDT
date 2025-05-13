// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CAttachment.h"
#include "Global.h"
#include "GameFramework/Character.h"
// Sets default values
ACAttachment::ACAttachment()
{
	CHelpers::CreateComponent ( this , &Root , "Root" );
}

// Called when the game starts or when spawned
void ACAttachment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter> ( GetOwner ( ) );
	Super::BeginPlay();

}

void ACAttachment::AttachTo ( FName InSocketName )
{
	AttachToComponent ( OwnerCharacter->GetMesh ( ) , FAttachmentTransformRules ( EAttachmentRule::KeepRelative , true ) , InSocketName );
}
