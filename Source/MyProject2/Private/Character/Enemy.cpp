// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
void AEnemy::Highlight()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(250);
}

void AEnemy::UnHighlight()
{
	GetMesh()->SetRenderCustomDepth(false);
}

int32 AEnemy::GetPlayerLevel()
{
	return Level;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->SetCollisionProfileName(FName("BlockAll"));
	InitAbilityActorInfo(); 

}

void AEnemy::InitAbilityActorInfo()
{	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
}

AEnemy::AEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));
	
}
