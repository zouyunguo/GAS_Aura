// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interface/CombatInterface.h"

/**
 * 属性捕获声明。只会有一个静态实例（见下面的 DamageStatics()）——
 * 捕获定义是纯静态数据，每次结算重建纯属浪费。这是 GAS 的固定写法。
 */
struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);

	AuraDamageStatics()
	{
		// 参数：AttributeSet 类、属性名、捕获来源(Source/Target)、是否快照
		//
		// bSnapshot = false：在 GE 实际结算那一刻读值，而不是 Spec 创建时。
		// 瞬时伤害差别不大，但持续型 GE 很重要——中途穿上护甲应立刻生效。
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	// 不加进这个数组，AttemptCalculate... 拿不到值。
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	const int32 SourceLevel = SourceCombatInterface ? SourceCombatInterface->GetPlayerLevel() : 1;
	const int32 TargetLevel = TargetCombatInterface ? TargetCombatInterface->GetPlayerLevel() : 1;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// 系数曲线表
	const UCharacterClassInfo* ClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	if (ClassInfo == nullptr || ClassInfo->DamageCalculationCoefficients == nullptr) return;

	// ---------- 1. 原始伤害 ----------
	float Damage = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::GetSingleton().Damage, false, 0.f);

	// ---------- 2. 格挡 ----------
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(0.f, TargetBlockChance);

	const bool bBlocked = FMath::RandRange(1, 100) <= TargetBlockChance;
	if (bBlocked)
	{
		Damage *= 0.5f;
	}
	// 第 14 章：这里会把 bBlocked 写进自定义 EffectContext

	// ---------- 3. 护甲减免 ----------
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(0.f, TargetArmor);

	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(0.f, SourceArmorPenetration);

	const FRealCurve* ArmorPenetrationCurve =
		ClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	if (ArmorPenetrationCurve == nullptr) return;
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceLevel);

	// 护甲穿透按百分比削弱目标护甲
	const float EffectiveArmor =
		TargetArmor * (100.f - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;

	const FRealCurve* EffectiveArmorCurve =
		ClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	if (EffectiveArmorCurve == nullptr) return;
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetLevel);

	// 剩余护甲按百分比减伤
	Damage *= (100.f - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;

	// ---------- 4. 暴击 ----------
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(0.f, SourceCriticalHitChance);

	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(0.f, SourceCriticalHitDamage);

	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().CriticalHitResistanceDef, EvaluationParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(0.f, TargetCriticalHitResistance);

	const FRealCurve* CriticalHitResistanceCurve =
		ClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	if (CriticalHitResistanceCurve == nullptr) return;
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetLevel);

	const float EffectiveCriticalHitChance =
		SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;

	const bool bCriticalHit = FMath::RandRange(1, 100) <= EffectiveCriticalHitChance;
	if (bCriticalHit)
	{
		Damage = Damage * 2.f + SourceCriticalHitDamage;
	}
	// 第 14 章：bCriticalHit 同样会写进 EffectContext

	// ---------- 5. 输出到元属性 ----------
	// 改的是 IncomingDamage 而不是 Health，实际扣血在 PostGameplayEffectExecute 里。
	const FGameplayModifierEvaluatedData EvaluatedData(
		UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
