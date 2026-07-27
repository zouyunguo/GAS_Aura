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
	if (InfiniteApplicationPolicy==EEffectApplicationPolicy::ApplyOnOverLap)
	{
		ApplyEffectToTarget(TargetActor,InfiniteGameplayEffectClass);
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
	if (InfiniteApplicationPolicy==EEffectApplicationPolicy::ApplyOnEndOverLap)
	{
		ApplyEffectToTarget(TargetActor,InfiniteGameplayEffectClass);
	}
	if (InfiniteRemovalPolicy==EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC= UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if ( !IsValid(TargetASC) )
		{
			return;
		}
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (auto pair: ActiveEffectHandles)
		{
			if (pair.Value==TargetASC)
			{
				TargetASC->RemoveActiveGameplayEffect(pair.Key,1);
				HandlesToRemove.Add(pair.Key);
				
			}
		}
		for (auto& pair: HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(pair); 
		}
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
	FGameplayEffectSpecHandle EffectSpecHandle =TargetASC->MakeOutgoingSpec(GameplayEffectClass,ActorLevel ,EffectContextHandle);
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle=TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		
	const bool bIsInfinite= EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy==EGameplayEffectDurationType::Infinite;
	
	if (bIsInfinite&&InfiniteRemovalPolicy==EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveGameplayEffectHandle,TargetASC);;
	}

}



