// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Interface/IEnemyInterface.h"
#include "Input/AuraEnhancedInputComponent.h"
AAuraPlayerController::AAuraPlayerController()
{

}
void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	cursurTrace();
}
void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem) {
		Subsystem->AddMappingContext(AuraContext, 0);

	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI inputModeData;
	inputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	inputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(inputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* EnhancedInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::move);
	EnhancedInputComponent->BindAbilityActions(InputConfig, this,
		&ThisClass::AbilityInputTagPressed,
		&ThisClass::AbilityInputTagReleased,
		&ThisClass::AbilityInputTagHeld);
}
void AAuraPlayerController::move(const FInputActionValue& Value)
{
	FVector2D movementVector = Value.Get<FVector2D>();
	if (APawn* pawn = GetPawn())
	{

		const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		pawn->AddMovementInput(ForwardDirection, movementVector.Y);
		pawn->AddMovementInput(RightDirection, movementVector.X);
	}
}

void AAuraPlayerController::cursurTrace()
{
	FHitResult hitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, hitResult);
	if (!hitResult.bBlockingHit) return;
	
		lastActor = currentActor;
		currentActor = Cast<IIEnemyInterface>(hitResult.GetActor());

	if(lastActor && currentActor && lastActor != currentActor)
	{

		lastActor->UnHighlight();
		currentActor->Highlight();

	}
	else if (!lastActor && currentActor) {
		currentActor->Highlight();
	}
	else if (lastActor && !currentActor) {
		lastActor->UnHighlight();
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	GetASC();
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{	
	if (!GetASC())return;
	GetASC()->AbilityInputTagReleased(InputTag);
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!GetASC())return;
	GetASC()->AbilityInputTagHeld(InputTag);
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (!AbilitySystemComponent)
	{
		AbilitySystemComponent=	Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
		
	}
	return AbilitySystemComponent;
}
