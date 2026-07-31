// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interface/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	//设置要获取的属性相关信息
	IntelligenceDef.AttributeToCapture=UAuraAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource=EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot=false;
	
	RelevantAttributesToCapture.Add(IntelligenceDef);
	
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceContainer = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetContainer = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags=SourceContainer;
	EvaluationParameters.TargetTags=TargetContainer;
	
	float Intelligence=0;
	//获取spec的目标的Intelligence属性值
	GetCapturedAttributeMagnitude(IntelligenceDef,Spec, EvaluationParameters,Intelligence);
	Intelligence=FMath::Max<float>(Intelligence,0.f);
		
	ICombatInterface* CombatInterface=	Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 level=CombatInterface->GetPlayerLevel();
	
	return 50.f+Intelligence*2.5f+15.f*level;
}
