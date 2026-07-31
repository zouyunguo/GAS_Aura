// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interface/CombatInterface.h"
#include "CharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
UCLASS(Abstract)
class MYPROJECT2_API ACharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> weapon;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttribute;
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;
	void InitializeAttributes();
	
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GE, float level);
};
