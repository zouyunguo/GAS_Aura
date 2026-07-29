// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyCharacter.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AuraPlayerState.h"
#include "Player/AuraPlayerController.h"
#include	"UI/HUD/AuraHud.h"
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
	InitAbilityActorInfo();
}

void AMyCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();

}

void AMyCharacter::InitAbilityActorInfo()
{
	//get playerstate and initialize the ability system component
	AAuraPlayerState* playerstate = GetPlayerState<AAuraPlayerState>();
 
	check(playerstate);
	//set the owner and avatar for the ability system component to the playerstate and character respectively
	playerstate->GetAbilitySystemComponent()->InitAbilityActorInfo(playerstate, this);
	Cast<UAuraAbilitySystemComponent>(playerstate->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = playerstate->GetAbilitySystemComponent();
	AttributeSet = playerstate->GetAttributeSet();

	if (AAuraPlayerController* playerController = Cast<AAuraPlayerController>(GetController())) {
		
		if (AAuraHUD* AuraHud = Cast<AAuraHUD>(playerController->GetHUD())) {
			AuraHud->InitOverlay(playerController, playerstate, AbilitySystemComponent, AttributeSet);
		}
	}

}
