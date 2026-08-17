// Fill out your copyright notice in the Description page of Project Settings.


#include "Character\CharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AI/NavigationModifier.h"
#include "Components/CapsuleComponent.h"
#include "MyProject2/MyProject2.h"
#include "UI/Widget/DamageTextComponent.h"
// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile,ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	weapon->SetupAttachment(GetMesh(), FName("WeaponHandsSocket"));
	weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}



void ACharacterBase::MulticastHandleDeath_Implementation()
{// 武器 ragdoll
	weapon->SetSimulatePhysics(true);
	weapon->SetEnableGravity(true);
	weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	// 角色 ragdoll
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	// 尸体不再挡路，也不再被火球命中
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Ignore);

	
	Dissolve();
	/*bDead = true;*/
}

void ACharacterBase::Die()
{
	// 武器脱手，改为物理模拟掉在地上
	weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
}

UAnimMontage* ACharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}



FVector ACharacterBase::GetCombatSocketLocation()
{
	return weapon->GetSocketLocation(WeaponTipSocketName);
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

}

void ACharacterBase::InitAbilityActorInfo()
{
}



void ACharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GE, float level)
{
	FGameplayEffectContextHandle ContextHandle= GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle SpecHandle=GetAbilitySystemComponent()->MakeOutgoingSpec(GE,level,ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),GetAbilitySystemComponent());
	

}

void ACharacterBase::AddCharacterAbilities()
{
	if (!HasAuthority()) return;
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraASC->AddCharacterAbilities(StartupAbilities);
}

void ACharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* dynamicInstance= UMaterialInstanceDynamic::Create(DissolveMaterialInstance,this);
		GetMesh()->SetMaterial(0,dynamicInstance);
		StartDissolveTimeline(dynamicInstance);
		
	}

}

void ACharacterBase::InitializeAttributes()
{
	ApplyEffectToSelf(DefaultPrimaryAttribute,1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes,1.f);
	ApplyEffectToSelf(DefaultVitalAttributes,1.f);
}


UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


