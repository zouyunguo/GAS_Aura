// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbility/AuraGameplayAbility.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT2_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	

	/**
 * 伤害类型 -> 数值（按技能等级取值），通过 SetByCaller 注入 GE。
 *
 * 第 13 章这里是单个 FScalableFloat；改成 Map 之后一个技能可以同时
 * 造成多种元素伤害（比如"炎爆"= Fire 30 + Physical 10），
 * 每种独立走目标对应的抗性。
 */
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
