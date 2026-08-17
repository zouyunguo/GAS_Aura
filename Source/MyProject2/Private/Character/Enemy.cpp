// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy.h"

#include "AuraGameplayTags.h"
#include "FractureEditorModeToolkit.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/Widget/DamageTextComponent.h"
#include "UI/WidgetController/AuraWidgetController.h"

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
	GetCharacterMovement()->MaxWalkSpeed=bHitReacting?0.f:BaseWalkSpeed;

	GetCapsuleComponent()->SetCollisionProfileName(FName("BlockAll"));
	InitAbilityActorInfo(); 
	InitializeHealthBar();
}

void AEnemy::Die()
{
	SetLifeSpan(LifeSpanAfterDeath);
	if (healthBar)
	{
		healthBar->SetVisibility(false);
	}
	Super::Die();
}


void AEnemy::InitAbilityActorInfo()
{	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	InitializeAttributes();  
	UAuraAbilitySystemLibrary::GiveStartUpAbilities(this,AbilitySystemComponent);
}

void AEnemy::InitializeHealthBar()
{	
	if (UWidgetComponent* HealthBarComp = FindComponentByClass<UWidgetComponent>())
	{
		if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBarComp->GetUserWidgetObject()))
		{
			AuraUserWidget->SetWidgetController(this);
		}
	}

	if (const UAuraAttributeSet* AuraAS = Cast<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute())
			.AddLambda([this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			});

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute())
			.AddLambda([this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			});

		AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::GetSingleton().Effects_HitReact,EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,&AEnemy::HitReactTagChanged);
		
		// 广播初始值（此时 GE 初始化已完成）
		OnHealthChanged.Broadcast(AuraAS->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());
	}
}







void AEnemy::HitReactTagChanged(const FGameplayTag callbackTag, int32 newCount)
{ 
	 bHitReacting = newCount>0;
	
	GetCharacterMovement()->MaxWalkSpeed=bHitReacting?0.f:BaseWalkSpeed;
	
}

void AEnemy::InitializeAttributes()
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this,CharacterClass,Level,AbilitySystemComponent);
	
}

AEnemy::AEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));
	
	healthBar=CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	healthBar->SetupAttachment(GetRootComponent());
	
}


