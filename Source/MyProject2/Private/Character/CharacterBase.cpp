// Fill out your copyright notice in the Description page of Project Settings.


#include "Character\CharacterBase.h"
#include "AbilitySystemComponent.h"
// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	weapon->SetupAttachment(GetMesh(), FName("WeaponHandsSocket"));
	weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

}

void ACharacterBase::InitAbilityActorInfo()
{
}

void ACharacterBase::InitializePrimaryAttribute() const
{
	FGameplayEffectContextHandle ContextHandle= GetAbilitySystemComponent()->MakeEffectContext();
	
	FGameplayEffectSpecHandle SpecHandle=GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultPrimaryAttribute,1.0f,ContextHandle);

	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),GetAbilitySystemComponent());
	
}

void ACharacterBase::InitializeSecondaryAttributes() const
{
	FGameplayEffectContextHandle ContextHandle= GetAbilitySystemComponent()->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle=GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultSecondaryAttributes,1.0f,ContextHandle);

	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),GetAbilitySystemComponent());
	

}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


