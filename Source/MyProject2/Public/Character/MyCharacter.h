// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "UI/Widget/AuraUserWidget.h"
#include "MyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT2_API AMyCharacter : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	AMyCharacter();
	// Callback function to be called when the character is possessed by a controller
	// PossessedBy is called on the server when a controller possesses this character, 
	// and OnRep_PlayerState is called on clients when the PlayerState is replicated to them.
	// Both functions are used to set up the ability system component and attribute set for the character, 
	// ensuring that they are properly initialized and ready to use when the character is controlled by a player.
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	virtual int32 GetPlayerLevel() override;
	

	
	protected:
	virtual void InitAbilityActorInfo() override;

private:

};
