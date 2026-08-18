// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage.generated.h"

/**
 * 伤害结算。执行顺序：
 *   1. 取出 SetByCaller 传入的原始伤害
 *   2. 格挡判定 —— 命中则伤害减半
 *   3. 护甲减免（受护甲穿透削弱）
 *   4. 暴击判定 —— 暴击则 伤害*2 + 暴击加成
 *   5. 写入目标的 IncomingDamage 元属性
 *
 * 系数来自 CharacterClassInfo 的 DamageCalculationCoefficients 曲线表，按等级取值。
 */
UCLASS()
class MYPROJECT2_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UExecCalc_Damage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
