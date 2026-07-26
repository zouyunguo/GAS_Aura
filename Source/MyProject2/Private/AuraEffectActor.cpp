// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraEffectActor.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}



// Called when the game starts or when spawned
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();


}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy==EEffectApplicationPolicy::ApplyOnOverLap)
	{
		ApplyEffectToTarget(TargetActor,InstantGameplayEffectClass);
	}
	
	if (DurationApplicationPolicy==EEffectApplicationPolicy::ApplyOnOverLap)
	{
		ApplyEffectToTarget(TargetActor,DurationGameplayEffectClass);
	}
	
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy==EEffectApplicationPolicy::ApplyOnEndOverLap)
	{
		ApplyEffectToTarget(TargetActor,InstantGameplayEffectClass);
	}
	if (DurationApplicationPolicy==EEffectApplicationPolicy::ApplyOnEndOverLap)
	{
		ApplyEffectToTarget(TargetActor,DurationGameplayEffectClass);
	}
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{	
	UAbilitySystemComponent* TargetASC=	UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (!TargetASC)
	{
		return ;
	}
	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle=TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle =TargetASC->MakeOutgoingSpec(GameplayEffectClass,1.0f,EffectContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		


}



