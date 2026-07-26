// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHUD::getOverlayWidgetController(const FwidgetControllerParams& controllerparams)
{
	if (!OverlayWidgetController) {
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(controllerparams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWdigetClass uninitialized"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetcontrollerclass uninitialized"));

	OverlayWidget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass);

	FwidgetControllerParams wcparams(PC, PS, ASC, AS);

	OverlayWidgetController = getOverlayWidgetController(wcparams);
	OverlayWidget->SetWidgetController(OverlayWidgetController);

	OverlayWidgetController->BroadcastInitialValues();

	OverlayWidget->AddToViewport();
}

