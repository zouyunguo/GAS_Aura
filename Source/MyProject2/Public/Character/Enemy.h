// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/CharacterBase.h"
#include "Interface/IEnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Enemy.generated.h"

enum class ECharacterClass : uint8;
class UWidgetComponent;
class UAuraUserWidget;
/**
 * 
 */
UCLASS()
class MYPROJECT2_API AEnemy : public ACharacterBase , public IIEnemyInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	
	virtual void Highlight() override;
	
	virtual void UnHighlight() override;
	
	/** combat interface **/
	virtual int32 GetPlayerLevel() override;
	
	virtual void BeginPlay() override;
	
	/** 覆盖以便隐藏血条并在若干秒后清理尸体。 */
	virtual void Die() override;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> healthBar;
	
	/** Broadcast to the enemy's health bar widget. */
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
	UPROPERTY(BlueprintReadOnly,Category="combat")
	bool bHitReacting;
	
	UPROPERTY(BlueprintReadOnly,Category="combat")
	float BaseWalkSpeed=250.f;
	float LifeSpanAfterDeath=5.f;
	
	
	void HitReactTagChanged(const FGameplayTag callbackTag,int32 newCount);
protected:
virtual void InitAbilityActorInfo() override;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Character Class Defaults")
	int32 Level=1;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Character Class Defaults")
	ECharacterClass CharacterClass= ECharacterClass::Warrior;
	
	void InitializeHealthBar();
	
	virtual void InitializeAttributes() override;
	


};
