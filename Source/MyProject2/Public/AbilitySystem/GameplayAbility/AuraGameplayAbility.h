// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT2_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	//绑定的输入键
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;
	
	/** 按技能等级取值的基础伤害，通过 SetByCaller 注入 GE。 */
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FScalableFloat Damage;

};
