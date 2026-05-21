// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyCharacter.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AuraPlayerState.h"
AMyCharacter::AMyCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


}

void AMyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	SetAbilitySystemAndAttributeSet();
}

void AMyCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	SetAbilitySystemAndAttributeSet();

}

void AMyCharacter::SetAbilitySystemAndAttributeSet()
{
	//get playerstate and initialize the ability system component
	AAuraPlayerState* playerstate = GetPlayerState<AAuraPlayerState>();

	check(playerstate);
	//set the owner and avatar for the ability system component to the playerstate and character respectively
	playerstate->GetAbilitySystemComponent()->InitAbilityActorInfo(playerstate, this);
	AbilitySystemComponent = playerstate->GetAbilitySystemComponent();
	AttributeSet = playerstate->GetAttributeSet();
}
