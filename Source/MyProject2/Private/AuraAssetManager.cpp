// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAssetManager.h"


#include "AuraAssetManager.h"
#include "AuraGameplayTags.h"
#include "AbilitySystemGlobals.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);
	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::InitializeNativeGameplayTags();

	// Required to use Target Data (added in the course during Section 11 "Ability Tasks",
	// harmless to have it here already).
	UAbilitySystemGlobals::Get().InitGlobalData();
}
