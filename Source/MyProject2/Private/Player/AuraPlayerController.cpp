// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interface/IEnemyInterface.h"
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

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::move);
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