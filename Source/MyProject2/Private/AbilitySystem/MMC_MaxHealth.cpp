// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interface/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	//设置要获取的属性相关信息
	VigorDef.AttributeToCapture=UAuraAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource=EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot=false;
	
	RelevantAttributesToCapture.Add(VigorDef);
	
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceContainer = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetContainer = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags=SourceContainer;
	EvaluationParameters.TargetTags=TargetContainer;
	
	float vigor=0;
	//获取spec的目标的vigor属性值
	GetCapturedAttributeMagnitude(VigorDef,Spec, EvaluationParameters,vigor);
	vigor=FMath::Max<float>(vigor,0.f);
		
	ICombatInterface* CombatInterface=	Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 level=CombatInterface->GetPlayerLevel();
	
	return 80.f+vigor*2.5f+10.f*level;
}
