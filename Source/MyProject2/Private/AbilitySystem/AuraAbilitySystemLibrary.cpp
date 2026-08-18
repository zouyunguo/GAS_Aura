// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AuraPlayerState.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AbilitySystem/GameplayAbility/AuraGameplayAbility.h"
#include "Game/AuraGameModeBase.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FwidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return AuraHUD->getOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FwidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,ECharacterClass CharacterClass, float level,UAbilitySystemComponent* ASC)
{
	AAuraGameModeBase* AuraGameMode=Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));	
	if (!AuraGameMode)return;
	UCharacterClassInfo* ClassInfo=AuraGameMode->CharacterClassInfo;
	FCharacterClassDefaultInfo classDefaultInfo = ClassInfo->GetClassDefaultInfo(CharacterClass);
	
	AActor* AvatarActor=ASC->GetAvatarActor();
	
	FGameplayEffectContextHandle PrimaryAttributeContextHandle=ASC->MakeEffectContext();
	PrimaryAttributeContextHandle.AddSourceObject(AvatarActor);
	
	FGameplayEffectSpecHandle PrimaryAttributeHandle=ASC->MakeOutgoingSpec(classDefaultInfo.PrimaryAttributes,level,PrimaryAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeHandle.Data.Get());
	
	FGameplayEffectSpecHandle SecondaryAttributeHandle=ASC->MakeOutgoingSpec(ClassInfo->SecondaryAttributes,level,PrimaryAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeHandle.Data.Get());
	
	FGameplayEffectSpecHandle VitalAttributeHandle=ASC->MakeOutgoingSpec(ClassInfo->VitalAttributes,level,PrimaryAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeHandle.Data.Get());
	
}

void UAuraAbilitySystemLibrary::GiveStartUpAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	AAuraGameModeBase* AuraGameMode=Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));	
	if (!AuraGameMode)return;
	
	UCharacterClassInfo* ClassInfo=AuraGameMode->CharacterClassInfo;
	for (auto AbilityClass: ClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{const AAuraGameModeBase* AuraGameMode =
		Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr) return nullptr;
	return AuraGameMode->CharacterClassInfo;
}
